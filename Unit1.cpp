//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#include <tlhelp32.h>
#include <Lmcons.h>
#include <iostream.h>
#include <ctype.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include "Unit1.h"
#include "Unit2.h"//Register
#include "Unit3.h"//Quick Button
#include "Unit5.h"//FormShowProcess
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ScktComp"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
#define LoadError Application->MessageBox("Cannot load settings program! Install program once again!","SDC",MB_ICONERROR);
#define NoMatch Application->MessageBox("The Value isn't match! Install program once again!","SDC",MB_ICONERROR);
//---------------------------------------------------------------------------
TAutoEnableBox *AutoEnableBox;

//Zmienne na potrzeby wyswietlania czasu oraz 'Auto mode'
//Zmienne u¿ywane do wyswietlania czasu
int uHour = 0;
int uMinute = 0;
int uSecond = 0;
//==========================================================================  'ZMIENNE DLA QUICK BUTTONS'
char BntI[100], BntII[100], BntIII[100], BntIV[100];

//==========================================================================  'ZMIENNE DLA 'AUTO MODE'
//Zmienna informujaca funkcjê czasowa czy wlaczyc tryb 'Auto Mode'.
//Domyslnie ustawiona na false
bool RunAutoMode = false;

//Zmienna kontrolna dla 'auto mode'. Jesli bedzie wieksza niz '1', program
//wyswietli ostrzezenie.
short int AutoModeWarning = 0;

//Zmienne do zapisania czasu ustawionego przez uzytkownika dla trybu 'Auto Mode'
int AutoModeLogOutHour, AutoModeLogOutMinute;
int AutoModeRestartHour, AutoModeRestartMinute;
int AutoModeShutDownHour, AutoModeShutDownMinute;

//Zmienne do zapisania czasu ustawionego przez uzytkownika dla trybu 'Auto Mode'
//które posluza do zapisania do rejestru systemowego
char SAutoModeLogOutHour[3], SAutoModeLogOutMinute[3];
char SAutoModeRestartHour[3], SAutoModeRestartMinute[3];
char SAutoModeShutDownHour[3], SAutoModeShutDownMinute[3];

//Zmienne informujace czy wykonac dana czynnosc dla trybu 'Auto Mode'. Po zmianie
//flagi jej odpowiednik po prawej stronie rowniez sie zmieni aby dana czynnosc
//nie wykonywala sie w nieskonczonosc ale tylko jeden raz!
//Domyslnie ustawione na false
bool AutoModeLogOutNow = false;         bool AutoModeLogOutNowDone = false;
bool AutoModeRestartNow = false;        bool AutoModeRestartNowDone = false;
bool AutoModeShutDownNow = false;       bool AutoModeShutDownNowDone = false;

//Zmienna dla trybu 'Auto mode'. Informuje czy wyswietlic komunikat.
bool AutoModeShowMessageNow = false;    bool AutoModeShowMessageDone = false;

//Zmienne przechowuj¹ce komunikaty dla trybu 'Auto Mode', które bêd¹ wyswietlane
char AutoModeLogOutShowMsg[200];
char AutoModeRestartShowMsg[200];
char AutoModeShutDownShowMsg[200];

//==========================================================================  'ZMIENNE DLA 'AUTO EXIT MODE'
//Zmienna informuj¹ca czy w³¹czyc tryb 'Auto Exit Mode'. Domyslnie ustawiona na false;
bool RunAutoExitMode = false;

//Zmienne przechowuj¹ce komunikaty dla trybu 'Auto Exit Mode', które bêd¹ wyswietlane
char AutoExitModeLogOutShowMsg[200];
char AutoExitModeRestartShowMsg[200];
char AutoExitModeShutDownShowMsg[200];

/*
Zmienne dla komunikatow. Jesli ktorys sie wyswietli flaga zmienia stan na true. Podobnie jak w
'AUTO MODE' jest to forma przed kilkukrotnym wyswietlaniu tego samego komunikatu.
*/
bool AutoExitModeLogOutNowDone = false;
bool AutoExitModeRestartNowDone = false;
bool AutoExitModeShutDownNowDone = false;

//Zmienna przechowujaca nazwe pliku exe
char AutoModeExitExeName[51];

//Zmienna informuj¹ca czy program exe zostal wylaczony
bool EndExe = false;

//==========================================================================  'ZMIENNE DLA 'NET MODE'
//Zmienne dla wczytywanych ustawien
bool IOnet;     //czy aktywowac Net Mode
char IpCom[17]; //wczytanie adresu IP
char sA[5];     //Port (connect)
char sB[5];     //Port (listenning)

//Zmienna przechowuj¹ca adres IP
String Server;

//Zmienne przechowuj¹ce polecenia
String NetLogOut = "@LO842@";         //Wyloguj siê
String NetRestart = "@RE124@";        //Restart
String NetShutDown = "@SH600@";       //Wy³¹cz komputer
String NetCommand = "@CM134@";        //Wykonaj komendê systemow¹
String NetMessage = "@MG700@";        //Wyswietl komunikat

//Flagi informuj¹ce które polecenie wykonac
bool NetLogOutD = false;
bool NetRestartD = false;
bool NetShutDownD = false;
bool NetCommandD = false;
bool NetMessageD = false;

//Zmienna przechowuj¹ce przes³ane polecenie
String ReceivedCommand;

//Zmienna informuj¹ca NetTimer, ¿e nadesz³o pocelenie do wykonania
bool CanI = false;

//WskaŸniki do zmiennych
char *PtrNetA, *PtrNetB;

//Tablica przechowuj¹ca nades³any komunikat lub komendê do wykonania
char NetIns[101];

//==========================================================================

//Lista funkcji
void ShutDownTheComputer(int option);
bool CheckCommands(char *CMa, char *CMb, int HowMany);

//---------------------------------------------------------------------------
//Funkcja wylaczajaca komputer
void ShutDownTheComputer(int option){
        OSVERSIONINFO vi;
        HANDLE hToken;
        TOKEN_PRIVILEGES tkp;

        vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

        if(GetVersionEx(&vi)){
                if(vi.dwPlatformId == VER_PLATFORM_WIN32_NT){
                        if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken)){
                                LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME,&tkp.Privileges[0].Luid);
                                tkp.PrivilegeCount = 1;
                                tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
                                AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,(PTOKEN_PRIVILEGES)NULL, 0);
                                        if(GetLastError() != ERROR_SUCCESS){
                                                CloseHandle(hToken);
                                                exit(EXIT_SUCCESS);
                                        }
                        }
        }
}
switch(option){
          case 0 : ExitWindowsEx(EWX_POWEROFF | EWX_FORCE,0);break;//wy³¹czenie komputera
          case 1 : ExitWindowsEx(EWX_LOGOFF,0);break;//wylogowanie
          case 2 : ExitWindowsEx(EWX_REBOOT,0);break;//restart
          case 3 : ExitWindowsEx(EWX_SHUTDOWN,0);//stan wstrzymania
}
}
//---------------------------------------------------------------------------
//Funkcja g³ówna
__fastcall TAutoEnableBox::TAutoEnableBox(TComponent* Owner)
        : TForm(Owner)
{
//Wczytujemy ustawienia programu

//Uchwyt do klucza
HKEY LoadKey;

//Rozmiar wartosci
DWORD EnabledControlSize = sizeof(DWORD);
DWORD mWindowSize = sizeof(DWORD);
DWORD SizeValueBntI = sizeof(BntI);
DWORD SizeValueBntII = sizeof(BntII);
DWORD SizeValueBntIII = sizeof(BntIII);
DWORD SizeValueBntIV = sizeof(BntIV);
DWORD AutoModeLogOutSize = sizeof(DWORD);
DWORD AutoModeLogOutTimeHSize = 3;
DWORD AutoModeLogOutTimeMSize = 3;
DWORD AutoModeLogOutMsgSize = 200;
DWORD AutoModeRestartSize = sizeof(DWORD);
DWORD AutoModeRestartTimeHSize = 3;
DWORD AutoModeRestartTimeMSize = 3;
DWORD AutoModeRestartMsgSize = 200;
DWORD AutoModeShutDownSize = sizeof(DWORD);
DWORD AutoModeShutDownTimeHSize = 3;
DWORD AutoModeShutDownTimeMSize = 3;
DWORD AutoModeShutDownMsgSize = 200;
DWORD AutoModeM1Size = sizeof(DWORD);
DWORD AutoModeM3Size = sizeof(DWORD);
DWORD AutoModeM5Size = sizeof(DWORD);
DWORD AutoModeMessagesSize = sizeof(DWORD);
DWORD AutoExitModeSize = sizeof(DWORD);
DWORD AutoExitModeLogOutSize = sizeof(DWORD);
DWORD AutoExitModeRestartSize = sizeof(DWORD);
DWORD AutoExitModeShutDownSize = sizeof(DWORD);
DWORD AutoExitModeExeNameSize = 51;
DWORD AutoExitModeMessagesSize = sizeof(DWORD);
DWORD AutoExitModeLogOutMsgSize = 200;
DWORD AutoExitModeRestartMsgSize = 200;
DWORD AutoExitModeShutDownMsgSize = 200;
DWORD NetIpSize = 17;
DWORD NetSocketASize = 5;
DWORD NetSocketBSize = 5;
DWORD NetLoadSettingsSize = sizeof(DWORD);


//Odczytana wartosc
long loadIO;

//Otwieramy klucz, w którym program przechowuje ustawienia
if((RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\SDCsettings",0,KEY_READ,&LoadKey)) == ERROR_SUCCESS){

        //Blokada kontroli ca³ego programu
        if((RegQueryValueEx(LoadKey,"EnabledControl",0,0,(LPBYTE)&loadIO,&EnabledControlSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        AutoEnableBox->Enabled = true;
                }
                else if(loadIO == 0x00000000){
                        AutoEnableBox->Enabled = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //Czy zminimalizowac okno
        if((RegQueryValueEx(LoadKey,"mWindow",0,0,(LPBYTE)&loadIO,&mWindowSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        AutoEnableBox->WindowState = wsMinimized;
                }
                else if(loadIO == 0x00000000){
                        AutoEnableBox->WindowState = wsNormal;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //Przyciski 'Quick Button'
        if((RegQueryValueEx(LoadKey,"QBn1",0,0,(LPBYTE)&BntI,&SizeValueBntI)) != ERROR_SUCCESS){
                NoMatch;
                exit(EXIT_FAILURE);
        }

        if((RegQueryValueEx(LoadKey,"QBn2",0,0,(LPBYTE)&BntII,&SizeValueBntII)) != ERROR_SUCCESS){
                NoMatch;
                exit(EXIT_FAILURE);
        }
        if((RegQueryValueEx(LoadKey,"QBn3",0,0,(LPBYTE)&BntIII,&SizeValueBntIII)) != ERROR_SUCCESS){
                NoMatch;
                exit(EXIT_FAILURE);
        }
        if((RegQueryValueEx(LoadKey,"QBn4",0,0,(LPBYTE)&BntIV,&SizeValueBntIV)) != ERROR_SUCCESS){
                NoMatch;
                exit(EXIT_FAILURE);
        }
        //Sprawdzamy ustawienia Auto Mode

        //Enable auto log out

        if((RegQueryValueEx(LoadKey,"AutoModeLogOut",0,0,(LPBYTE)&loadIO,&AutoModeLogOutSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        CheckAutoLog->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        CheckAutoLog->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }

        //Czas dla auto log out (godzina)
        if((RegQueryValueEx(LoadKey,"AutoModeLogOutTimeH",0,0,(LPBYTE)&SAutoModeLogOutHour,&AutoModeLogOutTimeHSize)) == ERROR_SUCCESS){
                HtimeAutoLog->Text = SAutoModeLogOutHour;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        //Czas dla auto log out (minuta)
        if((RegQueryValueEx(LoadKey,"AutoModeLogOutTimeM",0,0,(LPBYTE)&SAutoModeLogOutMinute,&AutoModeLogOutTimeMSize)) == ERROR_SUCCESS){
                MtimeAutoLog->Text = SAutoModeLogOutMinute;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        //Komunikat dla auto log out
        if((RegQueryValueEx(LoadKey,"AutoModeLogOutMsg",0,0,(LPBYTE)&AutoModeLogOutShowMsg,&AutoModeLogOutMsgSize)) == ERROR_SUCCESS){
                TextLogOut->Text = AutoModeLogOutShowMsg;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }

        //Enable auto restart
        if((RegQueryValueEx(LoadKey,"AutoModeRestart",0,0,(LPBYTE)&loadIO,&AutoModeRestartSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        CheckAutoRestart->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        CheckAutoRestart->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //Czas dla auto restart (godzina)
        if((RegQueryValueEx(LoadKey,"AutoModeRestartTimeH",0,0,(LPBYTE)&SAutoModeRestartHour,&AutoModeRestartTimeHSize)) == ERROR_SUCCESS){
                HtimeAutoRestart->Text = SAutoModeRestartHour;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        //Czas dla auto restart (minuta)
        if((RegQueryValueEx(LoadKey,"AutoModeRestartTimeM",0,0,(LPBYTE)&SAutoModeRestartMinute,&AutoModeRestartTimeMSize)) == ERROR_SUCCESS){
                MtimeAutoRestart->Text = SAutoModeRestartMinute;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        //Komunikat dla auto restart
        if((RegQueryValueEx(LoadKey,"AutoModeRestartMsg",0,0,(LPBYTE)&AutoModeRestartShowMsg,&AutoModeRestartMsgSize)) == ERROR_SUCCESS){
                TextRestart->Text = AutoModeRestartShowMsg;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }

        //Enable shut down
        if((RegQueryValueEx(LoadKey,"AutoModeShutDown",0,0,(LPBYTE)&loadIO,&AutoModeShutDownSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        CheckAutoShutDown->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        CheckAutoShutDown->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //Czas dla auto shut down (godzina)
        if((RegQueryValueEx(LoadKey,"AutoModeShutDownTimeH",0,0,(LPBYTE)&SAutoModeShutDownHour,&AutoModeShutDownTimeHSize)) == ERROR_SUCCESS){
                HtimeAutoShutDown->Text = SAutoModeShutDownHour;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        //Czas dla auto shut down (minuta)
        if((RegQueryValueEx(LoadKey,"AutoModeShutDownTimeM",0,0,(LPBYTE)&SAutoModeShutDownMinute,&AutoModeShutDownTimeMSize)) == ERROR_SUCCESS){
                MtimeAutoShutDown->Text = SAutoModeShutDownMinute;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        //Komunikat dla auto shut down
        if((RegQueryValueEx(LoadKey,"AutoModeShutDownMsg",0,0,(LPBYTE)&AutoModeShutDownShowMsg,&AutoModeShutDownMsgSize)) == ERROR_SUCCESS){
                TextShutDown->Text = AutoModeShutDownShowMsg;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        //Set time

        //1 minuta
        if((RegQueryValueEx(LoadKey,"AutoModeM1",0,0,(LPBYTE)&loadIO,&AutoModeM1Size)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        Check1minute->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        Check1minute->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //3 minuta
        if((RegQueryValueEx(LoadKey,"AutoModeM3",0,0,(LPBYTE)&loadIO,&AutoModeM3Size)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        Check3minutes->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        Check3minutes->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //5 minuta
        if((RegQueryValueEx(LoadKey,"AutoModeM5",0,0,(LPBYTE)&loadIO,&AutoModeM5Size)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        Check5minutes->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        Check5minutes->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //Instrukcja kontroluj¹ca
        if((Check1minute->Checked == false) && (Check3minutes->Checked == false) && (Check5minutes->Checked == false)){
                        NoMatch;
                        exit(EXIT_FAILURE);
        }

        //Show messages
        if((RegQueryValueEx(LoadKey,"AutoModeMessages",0,0,(LPBYTE)&loadIO,&AutoModeMessagesSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        ChechShowMessages->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        ChechShowMessages->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }

        //Sprawdzamy ustawienia dla Auto Exit Mode

        //Enable Auto Exit Mode
        if((RegQueryValueEx(LoadKey,"AutoExitMode",0,0,(LPBYTE)&loadIO,&AutoExitModeSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        CheckEnableAutoExit->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        CheckEnableAutoExit->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //Set actions

        //Log out user
        if((RegQueryValueEx(LoadKey,"AutoExitModeLogOut",0,0,(LPBYTE)&loadIO,&AutoExitModeLogOutSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        AcLogOut->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        AcLogOut->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //Restart user
        if((RegQueryValueEx(LoadKey,"AutoExitModeRestart",0,0,(LPBYTE)&loadIO,&AutoExitModeRestartSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        AcRestart->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        AcRestart->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //Shut down user
        if((RegQueryValueEx(LoadKey,"AutoExitModeShutDown",0,0,(LPBYTE)&loadIO,&AutoExitModeShutDownSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        AcShutDown->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        AcShutDown->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
        //Instrukcja kontroluj¹ca
        if((AcLogOut->Checked == false) && (AcRestart->Checked == false) && (AcShutDown->Checked == false)){
                        NoMatch;
                        exit(EXIT_FAILURE);
        }

        //Nazwa pliku exe
        if((RegQueryValueEx(LoadKey,"AutoExitModeExeName",0,0,(LPBYTE)&AutoModeExitExeName,&AutoExitModeExeNameSize)) == ERROR_SUCCESS){
                ProgramName->Text = AutoModeExitExeName;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }

        //Show message
        if((RegQueryValueEx(LoadKey,"AutoExitModeMessages",0,0,(LPBYTE)&loadIO,&AutoExitModeMessagesSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        MsgActionAEM->Checked = true;
                }
                else if(loadIO == 0x00000000){
                        MsgActionAEM->Checked = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }

        //Log out
        if((RegQueryValueEx(LoadKey,"AutoExitModeLogOutMsg",0,0,(LPBYTE)&AutoExitModeLogOutShowMsg,&AutoExitModeLogOutMsgSize)) == ERROR_SUCCESS){
                TextLogOutDB->Text = AutoExitModeLogOutShowMsg;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }

        //Restart
        if((RegQueryValueEx(LoadKey,"AutoExitModeRestartMsg",0,0,(LPBYTE)&AutoExitModeRestartShowMsg,&AutoExitModeRestartMsgSize)) == ERROR_SUCCESS){
                TextRestartDB->Text = AutoExitModeRestartShowMsg;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }

        //Shut down
        if((RegQueryValueEx(LoadKey,"AutoExitModeShutDownMsg",0,0,(LPBYTE)&AutoExitModeShutDownShowMsg,&AutoExitModeShutDownMsgSize)) == ERROR_SUCCESS){
                TextShutDownDB->Text = AutoExitModeShutDownShowMsg;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }

        //Wczytujemy ustawienia dla 'Net'

        if((RegQueryValueEx(LoadKey,"NetIp",0,0,(LPBYTE)&IpCom,&NetIpSize)) == ERROR_SUCCESS){
                IpAdress->Text = IpCom;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        if((RegQueryValueEx(LoadKey,"NetSocketA",0,0,(LPBYTE)&sA,&NetSocketASize)) == ERROR_SUCCESS){
                PortP->Text = sA;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        if((RegQueryValueEx(LoadKey,"NetSocketB",0,0,(LPBYTE)&sB,&NetSocketBSize)) == ERROR_SUCCESS){
                PortS->Text = sB;
        }else{
                NoMatch;
                exit(EXIT_FAILURE);
        }
        if((RegQueryValueEx(LoadKey,"NetLoadSettings",0,0,(LPBYTE)&loadIO,&NetLoadSettingsSize)) == ERROR_SUCCESS){
                if(loadIO == 0x00000001){
                        IOnet = true;
                }
                else if(loadIO == 0x00000000){
                        IOnet = false;
                }
                else{
                        NoMatch;
                        exit(EXIT_FAILURE);
                }
        }else{
                LoadError;
                exit(EXIT_FAILURE);
        }
}else{
        LoadError;
        exit(EXIT_FAILURE);
}

RegCloseKey(LoadKey);

//Teraz sprawdzamy ktory tryb wlaczyc

//Tryb 'Auto Mode'
if((CheckAutoLog->Checked == true) || (CheckAutoRestart->Checked == true) || (CheckAutoShutDown->Checked == true)){
        AutoModeRun->Click();
}

//Tryb 'Auto Exit Mode'
if(CheckEnableAutoExit->Checked == true){
        Run->Click();
}

//Tryb 'Net Mode'
if(IOnet){
        OKlis->Click();
}

//Kontynuujemy wykonywanie programu

//Wyswietlamy aktualnie zalogowanego uzytkownika
TCHAR UserName [UNLEN + 1];
DWORD SizeUserName = UNLEN + 1;
GetUserName((TCHAR*)UserName, &SizeUserName );
UserLoggedView->Caption = UserName;

//Wyswietlamy nazwe komputera
TCHAR ComputerName[MAX_COMPUTERNAME_LENGTH + 1];
DWORD SizeComputerName = sizeof(ComputerName) / sizeof(ComputerName[0]);
GetComputerName(ComputerName, &SizeComputerName);
ComputerNameView->Caption = ComputerName;
}
//Przycisk Wyloguj siê
void __fastcall TAutoEnableBox::LogOutButtonClick(TObject *Sender)
{
ShutDownTheComputer(1);
}
//---------------------------------------------------------------------------
//Przycisk Restart
void __fastcall TAutoEnableBox::RestartButtonClick(TObject *Sender)
{
ShutDownTheComputer(2);
}
//---------------------------------------------------------------------------
//Przycisk Wylacz
void __fastcall TAutoEnableBox::ShutDownButtonClick(TObject *Sender)
{
ShutDownTheComputer(0);
}
//---------------------------------------------------------------------------
//Funkcja licz¹ca czas oraz wykonujaca instrukcje 'Auto Mode'
void __fastcall TAutoEnableBox::TimeTimer(TObject *Sender)
{
//Instrukcje dla zegara programu
TSecond->Caption = uSecond;
TMinute->Caption = uMinute;
THour->Caption = uHour;

if(uSecond == 59){
        uSecond = 0;
        uMinute++;
        if(uMinute == 60){
                uMinute = 0;
                uHour++;
        }
}else{
     uSecond++;
}

//Instrukcje dla trybu 'Auto Mode'
//Sprawdzamy czy w³aczono ten tryb
if(RunAutoMode == true){
        //Sprawdzamy czy zaznaczono opcje 'enable' dla 'LOG OUT'
        if(CheckAutoLog->Checked == true){
                //Na poczatek sprawdzamy czy wlaczono komunikaty i jak ustawiono 'set time'.
                //Jesli wlaczono i osiagnieto odpowiedni czas - wyswietlamy komunikat
                if(ChechShowMessages->Checked == true){
                        //Jesli zaznaczono jedna minute
                        if(Check1minute->Checked == true){
                                //Obliczamy i jesli sie zgadza - zapisujemy to. Idea obliczen jest taka, ze
                                //zamieniamy godziny na minuty i dodajemy do tego minuty ze zmiennej ktore je
                                //przechowuje. Od tego odejmujemy 1 i porownujemy czy zegar osiagnal juz taki czas.
                                if((((AutoModeLogOutHour*60) + AutoModeLogOutMinute) - 1) == ((uHour*60) + uMinute)){
                                        AutoModeShowMessageNow = true;
                                }

                        }
                        //Jesli zaznaczono trzy minuty
                        if(Check3minutes->Checked == true){
                                if((((AutoModeLogOutHour*60) + AutoModeLogOutMinute) - 3) == ((uHour*60) + uMinute)){
                                        AutoModeShowMessageNow = true;
                                }

                        }
                        //Jesli zaznaczono piêc minut
                        if(Check5minutes->Checked == true){
                                if((((AutoModeLogOutHour*60) + AutoModeLogOutMinute) - 5) == ((uHour*60) + uMinute)){
                                        AutoModeShowMessageNow = true;
                                }

                        }
                }

                //Nastepnie sprawdzamy czy osiagnieto odpowiedni czas na wykonanie zadania 'LOG OUT'
                if((AutoModeLogOutHour == uHour) && (AutoModeLogOutMinute == uMinute)){
                        //Jesli tak zapisujemy to
                        AutoModeLogOutNow = true;
                }
                        
        }

        //Sprawdzamy czy zaznaczono opcje 'enable' dla 'RESTART'
        if(CheckAutoRestart->Checked == true){
                //Na poczatek sprawdzamy czy wlaczono komunikaty i jak ustawiono 'set time'.
                //Jesli wlaczono i osiagnieto odpowiedni czas - wyswietlamy komunikat
                if(ChechShowMessages->Checked == true){
                        //Jesli zaznaczono jedna minute
                        if(Check1minute->Checked == true){
                                //Obliczamy i jesli sie zgadza - zapisujemy to. Idea obliczen jest taka, ze
                                //zamieniamy godziny na minuty i dodajemy do tego minuty ze zmiennej ktore je
                                //przechowuje. Od tego odejmujemy 1 i porownujemy czy zegar osiagnal juz taki czas.
                                if((((AutoModeRestartHour*60) + AutoModeRestartMinute) - 1) == ((uHour*60) + uMinute)){
                                        AutoModeShowMessageNow = true;
                                }

                        }
                        //Jesli zaznaczono trzy minuty
                        if(Check3minutes->Checked == true){
                                if((((AutoModeRestartHour*60) + AutoModeRestartMinute) - 3) == ((uHour*60) + uMinute)){
                                        AutoModeShowMessageNow = true;
                                }

                        }
                        //Jesli zaznaczono piêc minut
                        if(Check5minutes->Checked == true){
                                if((((AutoModeRestartHour*60) + AutoModeRestartMinute) - 5) == ((uHour*60) + uMinute)){
                                        AutoModeShowMessageNow = true;
                                }

                        }
                }

                //Nastepnie sprawdzamy czy osiagnieto odpowiedni czas na wykonanie zadania 'RESTART'
                if((AutoModeRestartHour == uHour) && (AutoModeRestartMinute == uMinute)){
                        //Jesli tak zapisujemy to
                        AutoModeRestartNow = true;
                }
                        
        }

        //Sprawdzamy czy zaznaczono opcje 'enable' dla 'SHUT DOWN'
        if(CheckAutoShutDown->Checked == true){
                //Na poczatek sprawdzamy czy wlaczono komunikaty i jak ustawiono 'set time'.
                //Jesli wlaczono i osiagnieto odpowiedni czas - wyswietlamy komunikat
                if(ChechShowMessages->Checked == true){
                        //Jesli zaznaczono jedna minute
                        if(Check1minute->Checked == true){
                                //Obliczamy i jesli sie zgadza - zapisujemy to. Idea obliczen jest taka, ze
                                //zamieniamy godziny na minuty i dodajemy do tego minuty ze zmiennej ktore je
                                //przechowuje. Od tego odejmujemy 1 i porownujemy czy zegar osiagnal juz taki czas.
                                if((((AutoModeShutDownHour*60) + AutoModeShutDownMinute) - 1) == ((uHour*60) + uMinute)){
                                        AutoModeShowMessageNow = true;
                                }

                        }
                        //Jesli zaznaczono trzy minuty
                        if(Check3minutes->Checked == true){
                                if((((AutoModeShutDownHour*60) + AutoModeShutDownMinute) - 3) == ((uHour*60) + uMinute)){
                                        AutoModeShowMessageNow = true;
                                }

                        }
                        //Jesli zaznaczono piêc minut
                        if(Check5minutes->Checked == true){
                                if((((AutoModeShutDownHour*60) + AutoModeShutDownMinute) - 5) == ((uHour*60) + uMinute)){
                                        AutoModeShowMessageNow = true;
                                }

                        }
                }

                //Nastepnie sprawdzamy czy osiagnieto odpowiedni czas na wykonanie zadania 'SHUT DOWN'
                if((AutoModeShutDownHour == uHour) && (AutoModeShutDownMinute == uMinute)){
                        //Jesli tak zapisujemy to
                        AutoModeShutDownNow = true;
                }
                        
        }


        //Teraz sprawdzamy czy nalezy wykonac zadanie 'LOG OUT'
        if((AutoModeLogOutNow == true) && (AutoModeLogOutNowDone == false)){
                //Zapisujemy ze polecenie wykonano
                AutoModeLogOutNowDone = true;

                //Wykonujemy polecenie
                ShutDownTheComputer(1);

        }

        //Teraz sprawdzamy czy nalezy wykonac zadanie 'RESTART'
        if((AutoModeRestartNow == true) && (AutoModeRestartNowDone == false)){
                //Zapisujemy ze polecenie wykonano
                AutoModeRestartNowDone = true;
                
                //Wykonujemy polecenie
                ShutDownTheComputer(2);

        }

        //Teraz sprawdzamy czy nalezy wykonac zadanie 'SUT DOWN'
        if((AutoModeShutDownNow == true) && (AutoModeShutDownNowDone == false)){
                //Zapisujemy ze polecenie wykonano
                AutoModeShutDownNowDone = true;

                //Wykonujemy polecenie
                ShutDownTheComputer(0);

        }

        //Sprawdzamy czy wyswietlic juz komunikat
        if((AutoModeShowMessageNow == true) && (AutoModeShowMessageDone == false)){
                //Zapisujemy, ze wykonano wyswietlenie komunikatu
                AutoModeShowMessageDone = true;

                //Sprawdzamy czy wyswietlic komunikat dla 'LOG OUT'
                if(CheckAutoLog->Checked == true){
                        //Wyswietlamy komunikat
                        Application->MessageBox(AutoModeLogOutShowMsg,"SDC",MB_ICONWARNING);
                }
                //Sprawdzamy czy wyswietlic komunikat dla 'RESTART'
                if(CheckAutoRestart->Checked == true){
                        //Wyswietlamy komunikat
                        Application->MessageBox(AutoModeRestartShowMsg,"SDC",MB_ICONWARNING);
                }
                //Sprawdzamy czy wyswietlic komunikat dla 'SHUT DOWN'
                if(CheckAutoShutDown->Checked == true){
                        //Wyswietlamy komunikat
                        Application->MessageBox(AutoModeShutDownShowMsg,"SDC",MB_ICONWARNING);
                }

        }
}

}
//---------------------------------------------------------------------------
//Przycisk wy³¹czaj¹cy program
void __fastcall TAutoEnableBox::ExitButtonClick(TObject *Sender)
{
exit(EXIT_SUCCESS);        
}
//---------------------------------------------------------------------------
/*
Funkcja aktywuj¹ca i dezaktywuj¹ca pola w zak³adce Auto Mode. Funkcja ta dzia³a
dopóki nie zostanie w³¹czony tryp auto mode. Po wy³¹czeniu trybu 'auto mode'
funkcja ta zacznie dzia³ac ponownie.
*/
void __fastcall TAutoEnableBox::ActivAutoModeTimer(TObject *Sender)
{
//Je¿eli pole 'Enable auto log out' bêdzie zaznaczone
if(CheckAutoLog->Checked == true){
            //Aktywujemy pola
            HtimeAutoLog->Enabled = true;
            MtimeAutoLog->Enabled = true;

            //Zmieniamy kolory na bia³y
            HtimeAutoLog->Color = clWhite;
            MtimeAutoLog->Color = clWhite;
}
//Je¿eli nie
else{
        //Blokujemy pola
        HtimeAutoLog->Enabled = false;
        MtimeAutoLog->Enabled = false;

        //Zmieniamy kolory na szary
        MtimeAutoLog->Color = clSilver;
        HtimeAutoLog->Color = clSilver;
}

//Je¿eli pole 'Enable auto restart' bêdzie zaznaczone
if(CheckAutoRestart->Checked == true){
            //Aktywujemy pola
            HtimeAutoRestart->Enabled = true;
            MtimeAutoRestart->Enabled = true;

            //Zmieniamy kolory na bia³y
            HtimeAutoRestart->Color = clWhite;
            MtimeAutoRestart->Color = clWhite;
}
//Je¿eli nie
else{
        //Blokujemy pola
        HtimeAutoRestart->Enabled = false;
        MtimeAutoRestart->Enabled = false;

        //Zmieniamy kolory na szary
        HtimeAutoRestart->Color = clSilver;
        MtimeAutoRestart->Color = clSilver;
}

//Je¿eli pole 'Enable auto shut down' bêdzie zaznaczone
if(CheckAutoShutDown->Checked == true){
            //Aktywujemy pola
            HtimeAutoShutDown->Enabled = true;
            MtimeAutoShutDown->Enabled = true;

            //Zmieniamy kolory na bia³y
            HtimeAutoShutDown->Color = clWhite;
            MtimeAutoShutDown->Color = clWhite;
}
//Je¿eli nie
else{
        //Blokujemy pola
        HtimeAutoShutDown->Enabled = false;
        MtimeAutoShutDown->Enabled = false;

        //Zmieniamy kolory na szary
        HtimeAutoShutDown->Color = clSilver;
        MtimeAutoShutDown->Color = clSilver;

}
//Jesli zosta³ zaznaczony 'Show messages'
if(ChechShowMessages->Checked == true){
        //Jesli zostal zaznaczony 'Enable auto log out'
        if(CheckAutoLog->Checked == true){
                TextLogOut->Enabled = true;
                TextLogOut->Color = clWhite;
        }
        //Je¿eli nie
        else{
                TextLogOut->Enabled = false;
                TextLogOut->Color = clSilver;
        }
        //Jesli zostal zaznaczony 'Enable auto restart'
        if(CheckAutoRestart->Checked == true){
                TextRestart->Enabled = true;
                TextRestart->Color = clWhite;
        }
        //Je¿eli nie
        else{
                TextRestart->Enabled = false;
                TextRestart->Color = clSilver;
        }
        //Jesli zostal zaznaczony 'Enable shut down'
        if(CheckAutoShutDown->Checked == true){
                TextShutDown->Enabled = true;
                TextShutDown->Color = clWhite;
        }
        //Je¿eli nie
        else{
                TextShutDown->Enabled = false;
                TextShutDown->Color = clSilver;
        }

        //Odblokowujemy pola z 'Set time'
        Check1minute->Enabled = true;
        Check3minutes->Enabled = true;
        Check5minutes->Enabled = true;

}
//Je¿eli nie zaznaczono 'Show messages' zablokuj pola i zmien kolor
else{
        TextLogOut->Enabled = false;
        TextLogOut->Color = clSilver;

        TextRestart->Enabled = false;
        TextRestart->Color = clSilver;

        TextShutDown->Enabled = false;
        TextShutDown->Color = clSilver;

        //Odblokowujemy pola z 'Set time'
        Check1minute->Enabled = false;
        Check3minutes->Enabled = false;
        Check5minutes->Enabled = false;
}
//Je¿eli nie zaznaczono której kolwiek opcji z 'Set Auto Mode' blokuj
//'Show messages'
if((CheckAutoLog->Checked == false) && (CheckAutoRestart->Checked == false) && (CheckAutoShutDown->Checked == false)){
        ChechShowMessages->Enabled = false;
        ChechShowMessages->Checked = false;
}else{
        ChechShowMessages->Enabled = true;
}
//Je¿eli nie zaznaczono której kolwiek opcji z 'Set Auto Mode' blokuj
//'Run'
if((CheckAutoLog->Checked == true) || (CheckAutoRestart->Checked == true) || (CheckAutoShutDown->Checked == true)){
        AutoModeRun->Enabled = true;
}else{
        AutoModeRun->Enabled = false;
}
}
//---------------------------------------------------------------------------
//Przycisk 'Run'
void __fastcall TAutoEnableBox::AutoModeRunClick(TObject *Sender)
{
bool ControlLogOut, ControlLogOutMessage;
bool ControlRestart, ControlRestartMessage;
bool ControlShutDown, ControlShutDownMessage;

bool CanIone, CanItwo;

//Sprawdzamy czy wype³niono poprawnie wszystkie pola 'AUTO LOG'
if(CheckAutoLog->Checked == true){
        //Zapisujemy informacje, ze to pole zostalo aktywowane
        AutoModeWarning++;

        //Sprawdzamy czy wpisano godzinê i minuty
        if((HtimeAutoLog->Text == "") || (MtimeAutoLog->Text == "")){
                ControlLogOut = false;
        }else{
                ControlLogOut = true;
        }
        //Je¿eli zaznaczono 'Show messages' sprawdzamy czy wpisano komunikat
        if(ChechShowMessages->Checked == true){
                if(TextLogOut->Text == ""){
                        ControlLogOutMessage = false;
                }else{
                        ControlLogOutMessage = true;
                }
        }
        //Je¿eli nie zaznaczono zapisujemy, ¿e spr. zakoñczy³o siê prawid³owo
        else{
             ControlLogOutMessage = true;
        }
        //Jesli wpisano godzine i minute - konwertujemy je na liczby i
        //zapisujemy w odpowiednich zmiennych. Inna funkcja posluzy sie nimi.
        //Wczesniej jednak sprawdzimy czy podano prawidlowe wartosci
        if(ControlLogOut == true){
                        AutoModeLogOutHour = StrToInt(HtimeAutoLog->Text);
                        AutoModeLogOutMinute = StrToInt(MtimeAutoLog->Text);
        }
        //Analogicznie czynimy w stosunku do komunikatow. Konwertujemy je na typ
        //char aby zostaly wyswietlone przez MessageBox()
        if(ControlLogOutMessage == true){
                strcpy(AutoModeLogOutShowMsg,TextLogOut->Text.c_str());
        }

}
//Je¿eli nie zaznaczono zapisujemy, ¿e spr. zakoñczy³o siê prawid³owo
else{
        ControlLogOut = true;
        ControlLogOutMessage = true;
}
//Sprawdzamy czy wype³niono poprawnie wszystkie pola 'RESTART'
if(CheckAutoRestart->Checked == true){
        //Zapisujemy informacje, ze to pole zostalo aktywowane
        AutoModeWarning++;

        //Sprawdzamy czy wpisano godzinê i minuty
        if((HtimeAutoRestart->Text == "") || (MtimeAutoRestart->Text == "")){
                ControlRestart = false;
        }else{
                ControlRestart = true;
        }
        //Je¿eli zaznaczono 'Show messages' sprawdzamy czy wpisano komunikat
        if(ChechShowMessages->Checked == true){
                if(TextRestart->Text == ""){
                        ControlRestartMessage = false;
                }else{
                        ControlRestartMessage = true;
                }
        }
        //Je¿eli nie zaznaczono zapisujemy, ¿e spr. zakoñczy³o siê prawid³owo
        else{
             ControlRestartMessage = true;
        }
        //Jesli poprawnie wpisano godzine i minute - konwertujemy je na liczby i
        //zapisujemy w odpowiednich zmiennych. Inna funkcja posluzy sie nimi
        if(ControlRestart == true){
                AutoModeRestartHour = StrToInt(HtimeAutoRestart->Text);
                AutoModeRestartMinute = StrToInt(MtimeAutoRestart->Text);
        }
        //Analogicznie czynimy w stosunku do komunikatow. Konwertujemy je na typ
        //char aby zostaly wyswietlone przez MessageBox()
        if(ControlRestartMessage == true){
                strcpy(AutoModeRestartShowMsg,TextRestart->Text.c_str());
        }
}
//Je¿eli nie zaznaczono zapisujemy, ¿e spr. zakoñczy³o siê prawid³owo
else{
        ControlRestart = true;
        ControlRestartMessage = true;
}
//Sprawdzamy czy wype³niono poprawnie wszystkie pola 'SHUT DOWN'
if(CheckAutoShutDown->Checked == true){
        //Zapisujemy informacje, ze to pole zostalo aktywowane
        AutoModeWarning++;

        //Sprawdzamy czy wpisano godzinê i minuty
        if((HtimeAutoShutDown->Text == "") || (MtimeAutoShutDown->Text == "")){
                ControlShutDown = false;
        }else{
                ControlShutDown = true;
        }
        //Je¿eli zaznaczono 'Show messages' sprawdzamy czy wpisano komunikat
        if(ChechShowMessages->Checked == true){
                if(TextShutDown->Text == ""){
                        ControlShutDownMessage = false;
                }else{
                        ControlShutDownMessage = true;
                }
        }
        //Je¿eli nie zaznaczono zapisujemy, ¿e spr. zakoñczy³o siê prawid³owo
        else{
             ControlShutDownMessage = true;
        }
        //Jesli poprawnie wpisano godzine i minute - konwertujemy je na liczby i
        //zapisujemy w odpowiednich zmiennych. Inna funkcja posluzy sie nimi
        if(ControlShutDown == true){
                AutoModeShutDownHour = StrToInt(HtimeAutoShutDown->Text);
                AutoModeShutDownMinute = StrToInt(MtimeAutoShutDown->Text);
        }
        //Analogicznie czynimy w stosunku do komunikatow. Konwertujemy je na typ
        //char aby zostaly wyswietlone przez MessageBox()
        if(ControlShutDownMessage == true){
                strcpy(AutoModeShutDownShowMsg,TextShutDown->Text.c_str());
        }
}
//Je¿eli nie zaznaczono zapisujemy, ¿e spr. zakoñczy³o siê prawid³owo
else{
        ControlShutDown = true;
        ControlShutDownMessage = true;
}
//Kontrolujemy pola z 'set auto mode'. Jesli bedzie ich zaznaczonych wiecej niz jedno - wyswietlimy ostrzezenie
if(AutoModeWarning > 1){
        Application->MessageBoxA("WARNING: You have selected more than one field! Errors may occur!","SDC",MB_ICONWARNING);

        //Resetujemy licznik w przeciwnym razie komunikat bedzie pokazywal sie dopoki program nie zostanie wylaczony
        AutoModeWarning = 0;
}
//Kontrolujemy jeszcze pole z 'Set time'.


//Kontrolujemy powy¿sze wyniki sprawdzania (czy nie wyst¹pi³ b³¹d)
if((ControlLogOut == false) || (ControlRestart == false) || (ControlShutDown == false)){
        CanIone = false;
}else{
        CanIone = true;
}
//Analogicznie sprawdzamy komunikaty
if((ControlLogOutMessage == false) || (ControlRestartMessage == false) || (ControlShutDownMessage == false)){
        CanItwo = false;
}else{
        CanItwo = true;
}

//Zapada ostateczna decyzja o wykonaniu polecenia b¹dŸ odrzuceniu
if((CanIone == false) || (CanItwo == false)){
        Application->MessageBoxA("ERROR: Please fill in all fields correctly!","SDC",MB_ICONERROR);
}
//Je¿eli nie wyst¹pi³ ¿aden b³¹d przygotowujemy program do wejscia w tryp 'auto mode' i
//nastepnie wlaczamy ten tryb - 'RunAutoMode'
else{
     //Zatrzymujemy funkcje 'ActivAutoMode', poniewaz program zablokuje wszystkie pola
     ActivAutoMode->Enabled = false;

     //Blokujemy wszystkie opcje
     CheckAutoLog->Enabled = false;
     HtimeAutoLog->Enabled = false;
     HtimeAutoLog->Color = clSilver;
     MtimeAutoLog->Enabled = false;
     MtimeAutoLog->Color = clSilver;

     CheckAutoRestart->Enabled = false;
     HtimeAutoRestart->Enabled = false;
     HtimeAutoRestart->Color = clSilver;
     MtimeAutoRestart->Enabled = false;
     MtimeAutoRestart->Color = clSilver;

     CheckAutoShutDown->Enabled = false;
     HtimeAutoShutDown->Enabled = false;
     HtimeAutoShutDown->Color = clSilver;
     MtimeAutoShutDown->Enabled = false;
     MtimeAutoShutDown->Color = clSilver;

     ChechShowMessages->Enabled = false;

     Check1minute->Enabled = false;
     Check3minutes->Enabled = false;
     Check5minutes->Enabled = false;

     TextLogOut->Enabled = false;
     TextLogOut->Color = clSilver;
     TextRestart->Enabled = false;
     TextRestart->Color = clSilver;
     TextShutDown->Enabled = false;
     TextShutDown->Color = clSilver;

     //Blokujemy przycisk Save
     AutoModeSave->Enabled = false;

     //Wylaczamy i ukrywamy przycisk 'Run'
     AutoModeRun->Enabled = false;
     AutoModeRun->Visible = false;

     //W jego miejsce wstawiamy przycisk 'STOP'
     AutoModeStop->Enabled = true;
     AutoModeStop->Visible = true;

     Application->MessageBoxA("Auto Mode is working!","SDC",MB_ICONINFORMATION);

     //W³¹czamy funkcje Auto Mode
     RunAutoMode = true;

     //Informujemy na pierwszej stronie, ze ten tryb dzia³a
     iAutoMode->Caption = "Working";
     iAutoMode->Font->Color = clGreen;
}
}
//---------------------------------------------------------------------------

//Przycisk 'Stop' zatrzymuje tryb 'Auto Mode'
void __fastcall TAutoEnableBox::AutoModeStopClick(TObject *Sender)
{
//W³¹czamy funkcjê 'ActivAutoMode' oraz odblokowujemy wszystkie tryby
ActivAutoMode->Enabled = true;

CheckAutoLog->Enabled = true;
CheckAutoRestart->Enabled = true;
CheckAutoShutDown->Enabled = true;

//Wylaczamy i ukrywamy przycisk 'STOP'
AutoModeStop->Enabled = false;
AutoModeStop->Visible = false;

//W jego miejsce wstawiamy przycisk 'RUN'
AutoModeRun->Enabled = true;
AutoModeRun->Visible = true;

//Oblokowujemy przycisk Save
AutoModeSave->Enabled = true;

//Przywracamy wartosci domyslne dla zmiennych uzywanych przez 'Auto Mode'

//Resetujemy licznik 'enable'
AutoModeWarning = 0;

//Resetujemy zmienne do zapisania czasu ustawionego przez uzytkownika
AutoModeLogOutHour = 0;   AutoModeLogOutMinute = 0;
AutoModeRestartHour = 0;  AutoModeRestartMinute = 0;
AutoModeShutDownHour = 0; AutoModeShutDownMinute = 0;

//Resetujemy zmienne informujace czy wykonac dana czynnosc
AutoModeLogOutNow = false;    AutoModeLogOutNowDone = false;
AutoModeRestartNow = false;   AutoModeRestartNowDone = false;
AutoModeShutDownNow = false;  AutoModeShutDownNowDone = false;

//Resetujemy zmienne informuje czy wyswietlic komunikat.
AutoModeShowMessageNow = false;  AutoModeShowMessageDone = false;

//Wy³¹czamy tryb 'Auto Mode'
RunAutoMode = false;

//Informujemy na pierwszej stronie, ze ten tryb nie dzia³a
iAutoMode->Caption = "Stop";
iAutoMode->Font->Color = clRed;

Application->MessageBoxA("Done!","SDC",MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
//Przycisk 'Save' dla 'Auto Mode'. Zapisuje ustawienia 'Auto Mode' do rejestru systemowego by mogly
//pozniej zostac automatycznie wczytane przez program
void __fastcall TAutoEnableBox::AutoModeSaveClick(TObject *Sender)
{
//Uchwyt do klucza
HKEY hKey;

//Licznik b³êdów zapisu. Jesli bedzie rozny od '0' to znaczy, ze wystapily bledy w zapisie;
short int SaveStatus = 0;

//Licznik ostrzezen. Jesli bedzie rozny od '0' to znaczy, ze wystapily zastrzezenia co do zapisu;
short int SaveWarning = 0;

//Zmienne logiczne
DWORD sOn = 1;
DWORD sOff = 0;

//Otwieramy klucz, w którym program przechowuje ustawienia
if((RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\SDCsettings",0,KEY_ALL_ACCESS,&hKey)) == ERROR_SUCCESS){
        //Zapisujemy kolejno ustawienia dla 'Enable' oraz ustawienia czasu.

        //Opcja 'Enable Auto Log out'. Jesli wlaczona zapisz '1', w przeciwnym razie zapisz '0'. Zapisujemy
        //rowniez dane dotyczace czasu
        if(CheckAutoLog->Checked == true){
                if((RegSetValueEx(hKey,"AutoModeLogOut",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
                //Sprawdzamy czy to pole jest puste
                if(HtimeAutoLog->Text == ""){
                        SaveWarning++;
                }
                //Konwertujemy pole TEXT do char
                strcpy(SAutoModeLogOutHour,HtimeAutoLog->Text.c_str());
                //Zapisujemy w rejestrze
                if((RegSetValueEx(hKey,"AutoModeLogOutTimeH",0,REG_SZ,(LPBYTE)&SAutoModeLogOutHour,sizeof(SAutoModeLogOutHour))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
                if(MtimeAutoLog->Text == ""){
                        SaveWarning++;
                }
                strcpy(SAutoModeLogOutMinute,MtimeAutoLog->Text.c_str());
                if((RegSetValueEx(hKey,"AutoModeLogOutTimeM",0,REG_SZ,(LPBYTE)&SAutoModeLogOutMinute,sizeof(SAutoModeLogOutMinute))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
                //Sprawdzamy komunitaty
                if(ChechShowMessages->Checked == true){
                        //Sprawdzamy czy pole jest puste
                        if(TextLogOut->Text == ""){
                                SaveWarning++;
                        }
                        //Konwertujemy je na typ char
                        strcpy(AutoModeLogOutShowMsg,TextLogOut->Text.c_str());
                        //Zapisujemy do rejestru
                        if((RegSetValueEx(hKey,"AutoModeLogOutMsg",0,REG_SZ,(LPBYTE)&AutoModeLogOutShowMsg,sizeof(AutoModeLogOutShowMsg))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }

        }else{
                if((RegSetValueEx(hKey,"AutoModeLogOut",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
        }

        //Opcja 'Enable Auto Restart'.
        if(CheckAutoRestart->Checked == true){
                if((RegSetValueEx(hKey,"AutoModeRestart",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
                //Sprawdzamy czy to pole jest puste
                if(HtimeAutoRestart->Text == ""){
                        SaveWarning++;
                }
                strcpy(SAutoModeRestartHour,HtimeAutoRestart->Text.c_str());
                if((RegSetValueEx(hKey,"AutoModeRestartTimeH",0,REG_SZ,(LPBYTE)&SAutoModeRestartHour,sizeof(SAutoModeRestartHour))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
                if(MtimeAutoRestart->Text == ""){
                        SaveWarning++;
                }
                strcpy(SAutoModeRestartMinute,MtimeAutoRestart->Text.c_str());
                if((RegSetValueEx(hKey,"AutoModeRestartTimeM",0,REG_SZ,(LPBYTE)&SAutoModeRestartMinute,sizeof(SAutoModeRestartMinute))) != ERROR_SUCCESS){
                        SaveStatus++;
                }

                //Sprawdzamy komunitaty
                if(ChechShowMessages->Checked == true){
                        //Sprawdzamy czy pole jest puste
                        if(TextRestart->Text == ""){
                                SaveWarning++;
                        }
                        //Konwertujemy je na typ char
                        strcpy(AutoModeRestartShowMsg,TextRestart->Text.c_str());
                        //Zapisujemy do rejestru
                        if((RegSetValueEx(hKey,"AutoModeRestartMsg",0,REG_SZ,(LPBYTE)&AutoModeRestartShowMsg,sizeof(AutoModeRestartShowMsg))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }
        }else{
                if((RegSetValueEx(hKey,"AutoModeRestart",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
        }

        //Opcja 'Enable Auto Shut Down'.
        if(CheckAutoShutDown->Checked == true){
                if((RegSetValueEx(hKey,"AutoModeShutDown",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
                //Sprawdzamy czy to pole jest puste
                if(HtimeAutoShutDown->Text == ""){
                        SaveWarning++;
                }
                strcpy(SAutoModeShutDownHour,HtimeAutoShutDown->Text.c_str());
                if((RegSetValueEx(hKey,"AutoModeShutDownTimeH",0,REG_SZ,(LPBYTE)&SAutoModeShutDownHour,sizeof(SAutoModeShutDownHour))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
                if(MtimeAutoShutDown->Text == ""){
                        SaveWarning++;
                }
                strcpy(SAutoModeShutDownMinute,MtimeAutoShutDown->Text.c_str());
                if((RegSetValueEx(hKey,"AutoModeShutDownTimeM",0,REG_SZ,(LPBYTE)&SAutoModeShutDownMinute,sizeof(SAutoModeShutDownMinute))) != ERROR_SUCCESS){
                        SaveStatus++;
                }

                //Sprawdzamy komunitaty
                if(ChechShowMessages->Checked == true){
                        //Sprawdzamy czy pole jest puste
                        if(TextShutDown->Text == ""){
                                SaveWarning++;
                        }
                        //Konwertujemy je na typ char
                        strcpy(AutoModeShutDownShowMsg,TextShutDown->Text.c_str());
                        //Zapisujemy do rejestru
                        if((RegSetValueEx(hKey,"AutoModeShutDownMsg",0,REG_SZ,(LPBYTE)&AutoModeShutDownShowMsg,sizeof(AutoModeShutDownShowMsg))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }
        }else{
                if((RegSetValueEx(hKey,"AutoModeShutDown",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
        }

        //Opcja 'Show messages'.
        if(ChechShowMessages->Checked == true){
                                if((RegSetValueEx(hKey,"AutoModeMessages",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                                        SaveStatus++;
                                }
        }else{
                if((RegSetValueEx(hKey,"AutoModeMessages",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
        }

        //Opcja 'SetTime'
        if(Check1minute->Checked == true){
                                if((RegSetValueEx(hKey,"AutoModeM1",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                                        SaveStatus++;
                                }
        }else{
                if((RegSetValueEx(hKey,"AutoModeM1",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
        }
        if(Check3minutes->Checked == true){
                                if((RegSetValueEx(hKey,"AutoModeM3",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                                        SaveStatus++;
                                }
        }else{
                if((RegSetValueEx(hKey,"AutoModeM3",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
        }
        if(Check5minutes->Checked == true){
                                if((RegSetValueEx(hKey,"AutoModeM5",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                                        SaveStatus++;
                                }
        }else{
                if((RegSetValueEx(hKey,"AutoModeM5",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
        }

}
//Jesli nie uda sie otworzyc klucza, w którym program przechowuje ustawienia...
else{
     SaveStatus++;
}

//Zamykamy klucz
RegCloseKey(hKey);

//Wyswietlamy komuniat informujacy o pomyslnosci zapisu
if(SaveWarning){
        Application->MessageBoxA("WARNING: Errors may occur!","SDC",MB_ICONWARNING);
}

if(SaveStatus){
        Application->MessageBoxA("Error!","SDC",MB_ICONERROR);
}else{
        Application->MessageBoxA("Changes has been saved!","SDC",MB_ICONINFORMATION);
}
SaveWarning = 0;
SaveStatus = 0;
        
}
//---------------------------------------------------------------------------

//Przycisk 'Show process' dla Auto Exit Mode
void __fastcall TAutoEnableBox::RefreshClick(TObject *Sender)
{
FormShowPR->Show();
}
//---------------------------------------------------------------------------
/*
Funkcja aktywuj¹ca i dezaktywuj¹ca pola w zak³adce Auto Exit Mode. Funkcja ta dzia³a
dopóki nie zostanie w³¹czony ten tryb. Po wy³¹czeniu trybu 'auto exit mode'
funkcja ta zacznie dzia³ac ponownie.
*/

void __fastcall TAutoEnableBox::AutoExitModeFieldsTimer(TObject *Sender)
{
/*
Jesli pole 'Enable Auto Exit Mode' bedzie zaznaczone aktywuj pola i przyciski oraz
zmien ich kolor. W przeciwnym razie zablokuj oraz zmien kolor.
*/
if(CheckEnableAutoExit->Checked == true){

        ProgramName->Enabled = true;
        ProgramName->Color = clWhite;

        AcLogOut->Enabled = true;
        AcRestart->Enabled = true;
        AcShutDown->Enabled = true;

        MsgActionAEM->Enabled = true;

        //Tutaj sprawdzamy czy 'Show message' jest zaznaczony. Jesli tak
        //udostêpniamy pola 'Set message', jesli nie - blokujemy je
        if(MsgActionAEM->Checked == true){

                //Sprawdzamy dla pola 'Auto Log out' itd
                if(AcLogOut->Checked == true){
                        TextLogOutDB->Enabled = true;
                        TextLogOutDB->Color = clWhite;
                }else{
                        TextLogOutDB->Enabled = false;
                        TextLogOutDB->Color = clSilver;
                }

                if(AcRestart->Checked == true){
                        TextRestartDB->Enabled = true;
                        TextRestartDB->Color = clWhite;
                }else{
                        TextRestartDB->Enabled = false;
                        TextRestartDB->Color = clSilver;
                }

                if(AcShutDown->Checked == true){
                        TextShutDownDB->Enabled = true;
                        TextShutDownDB->Color = clWhite;
                }else{
                        TextShutDownDB->Enabled = false;
                        TextShutDownDB->Color = clSilver;
                }

        }
        //W przeciwnym razie zaciemniamy i blokujemy pola
        else{
                        TextLogOutDB->Enabled = false;
                        TextLogOutDB->Color = clSilver;

                        TextRestartDB->Enabled = false;
                        TextRestartDB->Color = clSilver;

                        TextShutDownDB->Enabled = false;
                        TextShutDownDB->Color = clSilver;
        }

        Run->Enabled = true;
        Stop->Enabled = true;
        Refresh->Enabled = true;

}else{
                ProgramName->Enabled = false;
                ProgramName->Color = clSilver;

                AcLogOut->Enabled = false;
                AcRestart->Enabled = false;
                AcShutDown->Enabled = false;

                MsgActionAEM->Enabled = false;
                MsgActionAEM->Checked = false;

                TextLogOutDB->Enabled = false;
                TextLogOutDB->Color = clSilver;

                TextRestartDB->Enabled = false;
                TextRestartDB->Color = clSilver;

                TextShutDownDB->Enabled = false;
                TextShutDownDB->Color = clSilver;



                Run->Enabled = false;
                Stop->Enabled = false;
                Refresh->Enabled = false;
}
}
//---------------------------------------------------------------------------

//Przycisk 'Run'
void __fastcall TAutoEnableBox::RunClick(TObject *Sender)
{
//Zmienna przychowujaca informacje czy pole exe name zostalo poprawnie wypelnione
bool ExeNameControl;

//Zmienne przechwujace informacje czy wypelniono pola Message
bool ControlLogOutMessage;
bool ControlRestartMessage;
bool ControlShutDownMessage;

//Kontrolujemy pole Exe Name
if(ProgramName->Text == ""){
        ExeNameControl = false;
}else{
        ExeNameControl = true;
}

//Sprawdzamy czy zosta³o aktywowane pole 'Show message'. Jesli tak sprawdzamy komunikaty
if(MsgActionAEM->Checked == true){

        //Sprawdzamy pole 'Log out user'
        if(AcLogOut->Checked == true){
                if(TextLogOutDB->Text == ""){
                        ControlLogOutMessage = false;
                }else{
                        ControlLogOutMessage = true;
                }
        }else{
                ControlLogOutMessage = true;
        }

        //Sprawdzamy pole 'Restart Computer'
        if(AcRestart->Checked == true){
                if(TextRestartDB->Text == ""){
                        ControlRestartMessage = false;
                }else{
                        ControlRestartMessage = true;
                }
        }else{
                ControlRestartMessage = true;
        }

        //Sprawdzamy pole 'Shut down Computer'
        if(AcShutDown->Checked == true){
                if(TextShutDownDB->Text == ""){
                        ControlShutDownMessage = false;
                }else{
                        ControlShutDownMessage = true;
                }
        }else{
                ControlShutDownMessage = true;
        }
}
//Jesli to pole nie zostalo aktywowane flagi ustawiamy na 'true'
else{
        ControlLogOutMessage = true;
        ControlRestartMessage = true;
        ControlShutDownMessage = true;
}

//Kontrolujemy pola. Jeli ktores z pol nie zostanie wypelnione prawidlowo - pokazujemy komunikat
if((ExeNameControl == false) || (ControlLogOutMessage == false) || (ControlRestartMessage == false) || (ControlShutDownMessage == false)){
        Application->MessageBoxA("ERROR: Please fill in all fields correctly!","SDC",MB_ICONERROR);
}
//W przeciwnym razie kontynuuj
else{
        //Wylaczamy Timer 'AutoExitModeFields'
        AutoExitModeFields->Enabled = false;

        //Blokujemy wszystkie pola (jesli trzeba zmieniamy tez kolory)
        CheckEnableAutoExit->Enabled = false;

        ProgramName->Enabled = false;
        ProgramName->Color = clSilver;

        AcLogOut->Enabled = false;
        AcRestart->Enabled = false;
        AcShutDown->Enabled = false;

        MsgActionAEM->Enabled = false;

        //Kontrolujemy przyciski
        Run->Visible = false;
        Stop->Visible = true;

        Refresh->Enabled = false;
        Save->Enabled = false;

        //Blokujemy pola tekstowe
        TextLogOutDB->Enabled = false;
        TextLogOutDB->Color = clSilver;

        TextRestartDB->Enabled = false;
        TextRestartDB->Color = clSilver;

        TextShutDownDB->Enabled = false;
        TextShutDownDB->Color = clSilver;

        //Kopiujemy nazwe pliku do tablicy
        strcpy(AutoModeExitExeName,ProgramName->Text.c_str());

        //Kopiujemy komunikaty do tablic
        if((AcLogOut->Checked == true) && (MsgActionAEM->Checked == true)){strcpy(AutoExitModeLogOutShowMsg,TextLogOutDB->Text.c_str());}
        if((AcRestart->Checked == true) && (MsgActionAEM->Checked == true)){strcpy(AutoExitModeRestartShowMsg,TextRestartDB->Text.c_str());}
        if((AcShutDown->Checked == true) && (MsgActionAEM->Checked == true)){strcpy(AutoExitModeShutDownShowMsg,TextShutDownDB->Text.c_str());}

        //Aktywujemy tryb 'Auto Exit Mode'
        AutoExitMode->Enabled = true;

        //Informujemy na pierwszej stronie, ze ten tryb dzia³a
        iAutoExitMode->Caption = "Working";
        iAutoExitMode->Font->Color = clGreen;

        //Wyswietlamy komunikat
        Application->MessageBoxA("Auto Exit Mode is working!","SDC",MB_ICONINFORMATION);

}
}
//---------------------------------------------------------------------------

/*
Funkcja 'Auto Exit Mode'. Idea tej funkcji jest taka, ¿e pobieramy nazwy wszystkich
uruchomionych plików exe. Kazda nazwa jest porównywana z t¹, któr¹ wpisa³
u¿ytkownik. Jesli taka nazwa nie zostanie znaleziona, program przyjmie, ze program
zostal wylaczony i wykona odpowiednia akcje 'Set action'. Jesli zas znajdzie taka
nazwe uzna, ze program jest uruchomiony i nie wykona zadnej czynnosci.
*/
void __fastcall TAutoEnableBox::AutoExitModeTimer(TObject *Sender)
{
PROCESSENTRY32 proc32;  //deklaracja struktury
HANDLE hSnapshot;       //uchwyt na CreateToolhelp32Snapshot

hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

//ustawiamy rozmiar struktury
proc32.dwSize = sizeof(PROCESSENTRY32);

//pierwsze wywolanie Process32First
if(Process32First(hSnapshot, &proc32)){
        //glowna petla wyœwietlaj¹ca procesy przez Process32Next
        while(Process32Next(hSnapshot, &proc32)){
                /*
                Porównujemy nazy plików exe. Jesli nazwa wystapi, ustawiamy flage
                na true i opuszczamy petle (dla pewnosci funkcja sprawdzi raz jeszcze czy
                nazwa rzeczywiscie jest czy jej nie ma)
                */
                if((strcmp(AutoModeExitExeName,proc32.szExeFile)) == 0){
                        EndExe = true;
                        break;
                }else{
                        EndExe = false;
                }
        }
}
CloseHandle(hSnapshot);

//Jesli program exe nie zostanie znaleziony wylaczamy funkcjê, a dopiero potem wykonujemy czynnosc
if(EndExe == false){

        /*
        Sprawdzamy jaka czynnosc mamy wykonac 'Set action'. Zasada dzialania jest taka:
        Sprawdzamy, ktora opcja jest zaznaczona z grupy 'Set Action'. W dalszej kolejnosci sprawdzamy czy
        wyswietlic komunikat. Jesli tak sprawdzamy czy nie byl on wczesniej wyswietlany. Jesli nie, pokazujemy go
        a przy nastepnym sprawdzaniu juz wykonujemy konkretna czynnosc (czyli wyloguj sie, restart itp.). Jesli
        nie wyswietlamy komunikatu od razu wykonujemy konkretna czynnosc.
        */

        //Wylogowac sie
        if(AcLogOut->Checked == true){
                //Sprawdzamy czy wyswietlic komunikat
                if(MsgActionAEM->Checked == true){
                        //Sprawdzamy czy zostal juz pokazany
                        if(AutoExitModeLogOutNowDone == false){
                                //Ustawiamy zmienna by wiecej tego komunikatu juz nie wyswietlac
                                AutoExitModeLogOutNowDone = true;
                                //Pokazujemy komunikat
                                Application->MessageBox(AutoExitModeLogOutShowMsg,"SDC",MB_ICONWARNING);
                        }else{
                                //Wylaczamy tryb
                                AutoExitMode->Enabled = false;
                                //Czekamy 6 sekund
                                Sleep(6000);
                                //Wykonujemy funkcje
                                ShutDownTheComputer(1);
                                //Wylaczamy program
                                exit(EXIT_SUCCESS);
                        }
                }else{
                        AutoExitMode->Enabled = false;
                        ShutDownTheComputer(1);
                        exit(EXIT_SUCCESS);
                }
        }

        //Restart komputera
        if(AcRestart->Checked == true){
                //Sprawdzamy czy wyswietlic komunikat
                if(MsgActionAEM->Checked == true){
                        //Sprawdzamy czy zostal juz pokazany
                        if(AutoExitModeRestartNowDone == false){
                                //Ustawiamy zmienna by wiecej tego komunikatu juz nie wyswietlac
                                AutoExitModeRestartNowDone = true;
                                //Pokazujemy komunikat
                                Application->MessageBox(AutoExitModeRestartShowMsg,"SDC",MB_ICONWARNING);
                        }else{
                                //Wylaczamy tryb
                                AutoExitMode->Enabled = false;
                                //Czekamy 6 sekund
                                Sleep(6000);
                                //Wykonujemy funkcje
                                ShutDownTheComputer(2);
                                //Wylaczamy program
                                exit(EXIT_SUCCESS);
                        }
                }else{
                        AutoExitMode->Enabled = false;
                        ShutDownTheComputer(2);
                        exit(EXIT_SUCCESS);
                }
        }

        //Zamknij komputer
        if(AcShutDown->Checked == true){
                //Sprawdzamy czy wyswietlic komunikat
                if(MsgActionAEM->Checked == true){
                        //Sprawdzamy czy zostal juz pokazany
                        if(AutoExitModeShutDownNowDone == false){
                                //Ustawiamy zmienna by wiecej tego komunikatu juz nie wyswietlac
                                AutoExitModeShutDownNowDone = true;
                                //Pokazujemy komunikat
                                Application->MessageBox(AutoExitModeShutDownShowMsg,"SDC",MB_ICONWARNING);
                        }else{
                                //Wylaczamy tryb
                                AutoExitMode->Enabled = false;
                                //Czekamy 6 sekund
                                Sleep(6000);
                                //Wykonujemy funkcje
                                ShutDownTheComputer(0);
                                //Wylaczamy program
                                exit(EXIT_SUCCESS);
                        }
                }else{
                        AutoExitMode->Enabled = false;
                        ShutDownTheComputer(0);
                        exit(EXIT_SUCCESS);
                }
        }
}
}
//---------------------------------------------------------------------------

//Pole 'filter' dla Auto Exit Mode
void __fastcall TAutoEnableBox::ExeNameFilterChange(TObject *Sender)
{
//ProcList->Mask=ExeNameFilter->Text;
}
//---------------------------------------------------------------------------

//Przycisk 'STOP' dla funkcji Auto Exit Mode
void __fastcall TAutoEnableBox::StopClick(TObject *Sender)
{
//Wy³¹czamy tryb Auto Exit Mode
AutoExitMode->Enabled = false;

//Informujemy na pierwszej stronie, ze ten tryb nie dzia³a
iAutoExitMode->Caption = "Stop";
iAutoExitMode->Font->Color = clRed;

//Aktywujemy pole 'Enable Auto Exit Mode'
CheckEnableAutoExit->Enabled = true;

//W³¹czamy Timer 'AutoExitModeFields' odpowiedzialny za blokade poszczegolnych pol i zmiane kolorow
AutoExitModeFields->Enabled = true;

//Chowamy przycisk 'STOP' i w jego miejsce wstawiamy przycisk 'Run'
Stop->Visible = false;
Run->Visible = true;

//Odblokowujemy przycisk 'Save'
Save->Enabled = true;

Application->MessageBoxA("Done!","SDC",MB_ICONINFORMATION);
}
//---------------------------------------------------------------------------
//Przycisk Save
void __fastcall TAutoEnableBox::SaveClick(TObject *Sender)
{
//Uchwyt do klucza
HKEY hKey;

//Licznik b³êdów zapisu. Jesli bedzie rozny od '0' to znaczy, ze wystapily bledy w zapisie;
short int SaveStatus = 0;

//Licznik ostrzezen. Jesli bedzie rozny od '0' to znaczy, ze wystapily zastrzezenia co do zapisu;
short int SaveWarning = 0;

//Zmienne logiczne
DWORD sOn = 1;
DWORD sOff = 0;

//Otwieramy klucz, w którym program przechowuje ustawienia
if((RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\SDCsettings",0,KEY_ALL_ACCESS,&hKey)) == ERROR_SUCCESS){

        //Zapisujemy kolejno ustawienia. sOn == 1, sOff == 0 (W³¹czony, wy³¹czony).

        //Opcja 'Enable Auto Exit Mode'
        if(CheckEnableAutoExit->Checked == true){

                //'Enable Auto Exit Mode' w³¹czone
                if((RegSetValueEx(hKey,"AutoExitMode",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                        SaveStatus++;
                }

                //'Log out user'
                if(AcLogOut->Checked == true){
                        if((RegSetValueEx(hKey,"AutoExitModeLogOut",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }else{
                        if((RegSetValueEx(hKey,"AutoExitModeLogOut",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }

                //'Restart computer'
                if(AcRestart->Checked == true){
                        if((RegSetValueEx(hKey,"AutoExitModeRestart",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }else{
                        if((RegSetValueEx(hKey,"AutoExitModeRestart",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }

                //'Shut down computer'
                if(AcShutDown->Checked == true){
                        if((RegSetValueEx(hKey,"AutoExitModeShutDown",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }else{
                        if((RegSetValueEx(hKey,"AutoExitModeShutDown",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }

                //'Set name'
                if(ProgramName->Text == ""){
                        SaveWarning++;
                }
                strcpy(AutoModeExitExeName,ProgramName->Text.c_str());
                if((RegSetValueEx(hKey,"AutoExitModeExeName",0,REG_SZ,(LPBYTE)&AutoModeExitExeName,sizeof(AutoModeExitExeName))) != ERROR_SUCCESS){
                        SaveStatus++;
                }

                //'Show message'
                if(MsgActionAEM->Checked == true){
                        if((RegSetValueEx(hKey,"AutoExitModeMessages",0,REG_DWORD,(LPBYTE)&sOn,sizeof(sOn))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }

                        //'Set Messages' - Log out
                        if(AcLogOut->Checked == true){
                                if(TextLogOutDB->Text == ""){
                                        SaveWarning++;
                                }
                                strcpy(AutoExitModeLogOutShowMsg,TextLogOutDB->Text.c_str());
                                if((RegSetValueEx(hKey,"AutoExitModeLogOutMsg",0,REG_SZ,(LPBYTE)&AutoExitModeLogOutShowMsg,sizeof(AutoExitModeLogOutShowMsg))) != ERROR_SUCCESS){
                                        SaveStatus++;
                                }
                        }

                        //'Set Messages' - Restart
                        if(AcRestart->Checked == true){
                                if(TextRestartDB->Text == ""){
                                        SaveWarning++;
                                }
                                strcpy(AutoExitModeRestartShowMsg,TextRestartDB->Text.c_str());
                                if((RegSetValueEx(hKey,"AutoExitModeRestartMsg",0,REG_SZ,(LPBYTE)&AutoExitModeRestartShowMsg,sizeof(AutoExitModeRestartShowMsg))) != ERROR_SUCCESS){
                                        SaveStatus++;
                                }
                        }

                        //'Set Messages' - Shut down
                        if(AcShutDown->Checked == true){
                                if(TextShutDownDB->Text == ""){
                                        SaveWarning++;
                                }
                                strcpy(AutoExitModeShutDownShowMsg,TextShutDownDB->Text.c_str());
                                if((RegSetValueEx(hKey,"AutoExitModeShutDownMsg",0,REG_SZ,(LPBYTE)&AutoExitModeShutDownShowMsg,sizeof(AutoExitModeShutDownShowMsg))) != ERROR_SUCCESS){
                                        SaveStatus++;
                                }
                        }
                }else{
                        if((RegSetValueEx(hKey,"AutoExitModeMessages",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                                SaveStatus++;
                        }
                }
                
        }else{
                //'Enable Auto Exit Mode' wy³¹czone
                if((RegSetValueEx(hKey,"AutoExitMode",0,REG_DWORD,(LPBYTE)&sOff,sizeof(sOff))) != ERROR_SUCCESS){
                        SaveStatus++;
                }
        }

}
//Jesli nie uda sie otworzyc klucza, w którym program przechowuje ustawienia...
else{
     SaveStatus++;
}

//Zamykamy klucz
RegCloseKey(hKey);

//Wyswietlamy komuniat informujacy o pomyslnosci zapisu
if(SaveWarning){
        Application->MessageBoxA("WARNING: Errors may occur!","SDC",MB_ICONWARNING);
}

if(SaveStatus){
        Application->MessageBoxA("Error!","SDC",MB_ICONERROR);
}else{
        Application->MessageBoxA("Changes has been saved!","SDC",MB_ICONINFORMATION);
}
SaveWarning = 0;
SaveStatus = 0;

}
//---------------------------------------------------------------------------

//£¹cze - klient: KOMUNIKAT SUKCESU
void __fastcall TAutoEnableBox::ClientSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
//Wyswietlamy odpowiednia informacje
InfoClient->Font->Color = clGreen;
InfoClient->Caption = "Connect to: " + Socket->RemoteHost;

//Informujemy na pierwszej stronie, ze ten tryb dzia³a
iNetMode->Caption = "Working";
iNetMode->Font->Color = clGreen;

//Odblokowujemy przycisk 'Discconect' abysmy mogli sie roz³¹czyc
DiscNet->Enabled = true;

//Aktywujemy Panel Control
NowLogOut->Enabled = true;
NowRestart->Enabled = true;
NowShutDown->Enabled = true;

NowMessage->Enabled = true;
NowMessage->Color = clWhite;
NowSendOne->Enabled = true;

NowCommandText->Enabled = true;
NowCommandText->Color = clWhite;
NowSendTwo->Enabled = true;

LogShow->Color = clWhite;
}
//---------------------------------------------------------------------------
//£¹cze - klient: KOMUNIKAT B£ÊDU
void __fastcall TAutoEnableBox::ClientSocketError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
//Wyswietlamy odpowiednia informacje
InfoClient->Font->Color = clRed;
InfoClient->Caption = "Error connecting to " + Server;

//Informujemy na pierwszej stronie, ze ten tryb nie dzia³a
iNetMode->Caption = "Stop";
iNetMode->Font->Color = clRed;

//Odblokowujemy pola i przycisk 'OK' oraz 'Listenning'
IpAdress->Enabled = true;
PortP->Enabled = true;
Lis->Enabled = true;

//Blokujemy przycisk 'OK'
OkNet->Enabled = true;

//Zapisujemy kod b³êdu
ErrorCode = 0;
}
//---------------------------------------------------------------------------
//£¹cze - klient: ODEBRANO KOMUNIKAT
void __fastcall TAutoEnableBox::ClientSocketRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
//Wyswietlamy odebrany komunikat w 'Log'
LogShow->Lines->Add(Socket->ReceiveText());
}
//---------------------------------------------------------------------------
//£¹cze - klient: KOMUNIKAT ROZ£¥CZY£ SIÊ
void __fastcall TAutoEnableBox::ClientSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
//Blokujemy Panel Control
NowLogOut->Enabled = false;
NowRestart->Enabled = false;
NowShutDown->Enabled = false;

NowMessage->Enabled = false;
NowMessage->Color = clSilver;
NowSendOne->Enabled = false;

NowCommandText->Enabled = false;
NowCommandText->Color = clSilver;
NowSendTwo->Enabled = false;

LogShow->Color = clSilver;

//Wyswietlamy komunikat
InfoClient->Font->Color = clRed;
InfoClient->Caption = Socket->RemoteHost + " disconnected!";

//Informujemy na pierwszej stronie, ze ten tryb nie dzia³a
iNetMode->Caption = "Stop";
iNetMode->Font->Color = clRed;
}
//---------------------------------------------------------------------------
//Przycisk 'OK' dla Connect
void __fastcall TAutoEnableBox::OkNetClick(TObject *Sender)
{
//Automatycznie zapisujemy IP i Port do rejestru (jesli to sie nie uda - milczymy)

//Uchwyt do klucza
HKEY hKey;

//Otwieramy klucz, w którym program przechowuje ustawienia
if((RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\SDCsettings",0,KEY_ALL_ACCESS,&hKey)) == ERROR_SUCCESS){

        //IP
        strcpy(IpCom,IpAdress->Text.c_str());
        if((RegSetValueEx(hKey,"NetIp",0,REG_SZ,(LPBYTE)&IpCom,sizeof(IpCom))) != ERROR_SUCCESS){

        }
        //Port
        strcpy(sA,PortP->Text.c_str());
        if((RegSetValueEx(hKey,"NetSocketA",0,REG_SZ,(LPBYTE)&sA,sizeof(sA))) != ERROR_SUCCESS){

        }
}

RegCloseKey(hKey);

//Sprawdzamy czy pola zostaly wypelnione
if((IpAdress->Text == "") || (PortP->Text == "")){
        Application->MessageBoxA("ERROR: Please fill in all fields correctly!","SDC",MB_ICONERROR);
}else{
        //Blokujemy pola
        IpAdress->Enabled = false;
        PortP->Enabled = false;

        //Blokujemy przycisk 'OK' oraz 'Listening
        OkNet->Enabled = false;
        Lis->Enabled = false;

        //Wyswietlamy odpowiednia informacje
        InfoClient->Font->Color = clTeal;
        InfoClient->Caption = "Connecting...";

        //Przypisujemy wprowadzony port
        ClientSocket->Port = StrToInt(PortP->Text);

        //Ustawiamy gniazdo
        if (ClientSocket->Active){
                ClientSocket->Active = false;
        }

        //Kopiujemy adres IP komputera
        Server = IpAdress->Text.c_str();

        //Jesli dlugosc adresu jest wieksza od zera probujemy po³¹czyc siê z tym adresem
        if (Server.Length() > 0){
                ClientSocket->Host = Server;
                ClientSocket->Active = true;
        }
}
}
//---------------------------------------------------------------------------
//Przycisk 'Connect'
void __fastcall TAutoEnableBox::ConClick(TObject *Sender)
{
//Aktywujemy przcisk 'Listening'
Lis->Enabled = true;

//Dezaktywujemy pole 'Listening'
PortS->Enabled = false;
PortS->Color = clSilver;

OKlis->Enabled = false;

//Aktywujemy pole 'Computer Adress'
IpAdress->Enabled = true;
IpAdress->Color = clWhite;

PortP->Enabled = true;
PortP->Color = clWhite;

OkNet->Enabled = true;

//Blokujemy przcisk 'Connect'
Con->Enabled = false;
}
//---------------------------------------------------------------------------

//Przycisk 'Listening'
void __fastcall TAutoEnableBox::LisClick(TObject *Sender)
{
//Aktywujemy przcisk 'Connect'
Con->Enabled = true;

//Dezaktywujemy pole 'Computer Adress'
IpAdress->Enabled = false;
IpAdress->Color = clSilver;

PortP->Enabled = false;
PortP->Color = clSilver;

OkNet->Enabled = false;

//Aktywujemy pole 'Listening'
PortS->Enabled = true;
PortS->Color = clWhite;

OKlis->Enabled = true;

//Blokujemy przcisk 'Listening'
Lis->Enabled = false;
}
//---------------------------------------------------------------------------

//Przycisk 'OK' dla Listening
void __fastcall TAutoEnableBox::OKlisClick(TObject *Sender)
{
//Automatycznie zapisujemy Port do rejestru (jesli to sie nie uda - milczymy)

//Uchwyt do klucza
HKEY hKey;

//Otwieramy klucz, w którym program przechowuje ustawienia
if((RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\SDCsettings",0,KEY_ALL_ACCESS,&hKey)) == ERROR_SUCCESS){

        //Port
        strcpy(sB,PortS->Text.c_str());
        if((RegSetValueEx(hKey,"NetSocketB",0,REG_SZ,(LPBYTE)&sB,sizeof(sB))) != ERROR_SUCCESS){

        }
}

RegCloseKey(hKey);

//Sprawdzamy czy pola zostaly wypelnione
if(PortS->Text == ""){
        Application->MessageBoxA("ERROR: Please fill in field correctly!","SDC",MB_ICONERROR);
}else{
        //Blokujemy pole port i przycisk 'OK' oraz 'Connect'
        PortS->Enabled = false;
        OKlis->Enabled = false;
        Con->Enabled = false;

        //Odblokowujemy przycisk 'Disconnect'
        DiscNet->Enabled = true;

        //Wyswietlamy odpowiednia informacje
        InfoClient->Font->Color = clTeal;
        InfoClient->Caption = "Listening...";

        //Informujemy na pierwszej stronie, ze ten tryb dzia³a
        iNetMode->Caption = "Working";
        iNetMode->Font->Color = clGreen;

        //Przypisujemy wprowadzony port
        ServerSocket1->Port = StrToInt(PortS->Text);

        //W³¹czamy nasluch
        ServerSocket1->Active = true;
}
}
//---------------------------------------------------------------------------
//£¹cze - server: KOMUNIKAT SUKCESU
void __fastcall TAutoEnableBox::ServerSocket1Accept(TObject *Sender,
      TCustomWinSocket *Socket)
{
//Wyswietlamy odpowiednia informacje
InfoClient->Font->Color = clGreen;
InfoClient->Caption = "Connect to: " + Socket->RemoteAddress;

//Odblokowujemy przycisk 'Discconect' abysmy mogli sie roz³¹czyc
DiscNet->Enabled = true;

//W³¹czamy NetTimer
NetTimer->Enabled = true;

//Informujemy na pierwszej stronie, ze ten tryb dzia³a
iNetMode->Caption = "Working";
iNetMode->Font->Color = clGreen;
}
//---------------------------------------------------------------------------
//£¹cze - server: KOMUNIKAT B£ÊDU
void __fastcall TAutoEnableBox::ServerSocketClient1Error(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
InfoClient->Font->Color = clRed;
InfoClient->Caption = "Error!";

//Wy³¹czamy NetTimer
NetTimer->Enabled = false;

//Informujemy na pierwszej stronie, ze ten tryb nie dzia³a
iNetMode->Caption = "Stop";
iNetMode->Font->Color = clRed;
}
//---------------------------------------------------------------------------
//£¹cze - serwer: KOMUNIKAT ROZ£ACZY£ SIÊ
void __fastcall TAutoEnableBox::ServerSocketClient1Disconnect(
      TObject *Sender, TCustomWinSocket *Socket)
{
InfoClient->Font->Color = clRed;
InfoClient->Caption = Socket->RemoteAddress + " disconnected!";

//Wy³¹czamy NetTimer
NetTimer->Enabled = false;

//Informujemy na pierwszej stronie, ze ten tryb nie dzia³a
iNetMode->Caption = "Stop";
iNetMode->Font->Color = clRed;
}
//---------------------------------------------------------------------------
//£¹cze - server: ODBIERAJ INFORMACJE
void __fastcall TAutoEnableBox::ServerSocketClient1Read(TObject *Sender,
      TCustomWinSocket *Socket)
{
//Sprawdzamy czy nie jest wykonywane jakies inne polecenie. Jesli nie, odbieramy
//polecenie. W przeciwnym razie informujemy, ze nalezy czekac na wykonanie poprzedniego
//polecenia.
if(CanI == false){
        //Przechwytujemy polecenie i zapisujemy w zmiennej string
        ReceivedCommand = Socket->ReceiveText();

        //Informujemy NetTimer, ¿e nadesz³o polecenie
        CanI = true;

        //Wysy³amy potwierdzenie odbioru
        ServerSocket1->Socket->Connections[0]->SendText("Accepted!");
}else{
        //Wysy³amy informacjê
        ServerSocket1->Socket->Connections[0]->SendText("Please wait!");
}
}
//---------------------------------------------------------------------------
//Przycisk 'Disconnect'
void __fastcall TAutoEnableBox::DiscNetClick(TObject *Sender)
{
//Rozlaczamy sie
ClientSocket->Active = false;
ServerSocket1->Active = false;

//Blokujemy Panel Control
NowLogOut->Enabled = false;
NowRestart->Enabled = false;
NowShutDown->Enabled = false;

NowMessage->Enabled = false;
NowMessage->Color = clSilver;
NowSendOne->Enabled = false;

NowCommandText->Enabled = false;
NowCommandText->Color = clSilver;
NowSendTwo->Enabled = false;

LogShow->Color = clSilver;

//Przywracamy zakladke do stanu pierwotnego
Con->Enabled = true;
Lis->Enabled = true;

IpAdress->Enabled = false;
PortP->Enabled = false;

PortS->Enabled = false;
OKlis->Enabled = false;

InfoClient->Font->Color = clRed;
InfoClient->Caption = "Stop";

DiscNet->Enabled = false;

//Wy³¹czamy NetTimer
NetTimer->Enabled = false;

//Informujemy na pierwszej stronie, ze ten tryb nie dzia³a
iNetMode->Caption = "Stop";
iNetMode->Font->Color = clRed;
}
//---------------------------------------------------------------------------
//Przycisk Log Out dla Net Mode
void __fastcall TAutoEnableBox::NowLogOutClick(TObject *Sender)
{
//Wysy³amy polecenie: Wyloguj uzytkownika
ClientSocket->Socket->SendText("@LO842@");

LogShow->Lines->Add("Sent command: Log Out");

}
//---------------------------------------------------------------------------
//Przycisk Restart dla Net Mode
void __fastcall TAutoEnableBox::NowRestartClick(TObject *Sender)
{
ClientSocket->Socket->SendText("@RE124@");

LogShow->Lines->Add("Sent command: Restart");
}
//---------------------------------------------------------------------------
//Przycisk Shut down dla Net Mode
void __fastcall TAutoEnableBox::NowShutDownClick(TObject *Sender)
{
ClientSocket->Socket->SendText("@SH600@");

LogShow->Lines->Add("Sent command: Shut down");
}
//---------------------------------------------------------------------------
//Przycisk Send 'Command Mode' dla Net Mode
void __fastcall TAutoEnableBox::NowSendTwoClick(TObject *Sender)
{
ClientSocket->Socket->SendText(
"@CM134@" + NowCommandText->Lines->Strings[NowCommandText->Lines->Count - 1]);

LogShow->Lines->Add("Sent command: " + NowCommandText->Lines->Strings[NowCommandText->Lines->Count - 1]);
}
//---------------------------------------------------------------------------
//Przycisk Send 'Send Message' dla Net Mode
void __fastcall TAutoEnableBox::NowSendOneClick(TObject *Sender)
{
ClientSocket->Socket->SendText("@MG700@" + NowMessage->Text);

LogShow->Lines->Add("Sent message: " + NowMessage->Text);
}
//---------------------------------------------------------------------------
//Funkcja dla 'Net' wykonuj¹ca nades³ane polecenia
void __fastcall TAutoEnableBox::NetTimerTimer(TObject *Sender)
{
//Sprawdzamy czy jest komenda do wykonania
if(CanI){
        //Zapisujemy, ¿e polecenie przyjêto
        CanI = false;

        //Resetujemy zmienne
        NetLogOutD = false;
        NetRestartD = false;
        NetShutDownD = false;
        NetCommandD = false;
        NetMessageD = false;

        //Sprawdzamy kolejno polecenia:

        //Przypisujemy wskaŸnik do ReceivedCommand
        PtrNetB = ReceivedCommand.c_str();

        //Polecenie: Wyloguj sie
        PtrNetA = NetLogOut.c_str();
        
        if(CheckCommands(PtrNetB,PtrNetA,7)){
                NetLogOutD = true;
        }

        //Polecenie: Restart
        PtrNetA = NetRestart.c_str();

        if(CheckCommands(PtrNetB,PtrNetA,7)){
                NetRestartD = true;
        }

        //Polecenie: Zamknij
        PtrNetA = NetShutDown.c_str();

        if(CheckCommands(PtrNetB,PtrNetA,7)){
                NetShutDownD = true;
        }

        //Polecenie: Pokaz komunikat
        PtrNetA = NetMessage.c_str();

        if(CheckCommands(PtrNetB,PtrNetA,6)){
                NetMessageD = true;
        }

        //Polecenie: Wykonaj komendê
        PtrNetA = NetCommand.c_str();

        if(CheckCommands(PtrNetB,PtrNetA,6)){
                NetCommandD = true;
        }
        //Teraz sprawdzamy wynik powy¿szego dzia³ania i wykonujemy odpowiedni¹ czynnosc

        //Polecenie: Wyloguj
        if(NetLogOutD == true){
                //Wysy³amy informacjê
                ServerSocket1->Socket->Connections[0]->SendText("Done!");

                //Pokazujemy informacjê w 'Log'
                LogShow->Lines->Add("Received: Log Out now");

                //OpóŸniamy wykonanie na 3 sekundy
                Sleep(3000);

                //Wykonujemy polecenie
                ShutDownTheComputer(1);

        }

        //Polecenie: Restart
        if(NetRestartD == true){
                //Wysy³amy informacjê
                ServerSocket1->Socket->Connections[0]->SendText("Done!");

                //Pokazujemy informacjê w 'Log'
                LogShow->Lines->Add("Received: Restart now");

                //OpóŸniamy wykonanie na 3 sekundy
                Sleep(3000);

                //Wykonujemy polecenie
                ShutDownTheComputer(2);

        }

        //Polecenie: Zamknij
        if(NetShutDownD == true){
                //Wysy³amy informacjê
                ServerSocket1->Socket->Connections[0]->SendText("Done!");

                //Pokazujemy informacjê w 'Log'
                LogShow->Lines->Add("Received: Shut down now");

                //OpóŸniamy wykonanie na 3 sekundy
                Sleep(3000);

                //Wykonujemy polecenie
                ShutDownTheComputer(0);

        }

        //Polecenie: Pokaz komunikat
        if(NetMessageD == true){
                //Wysy³amy informacjê
                ServerSocket1->Socket->Connections[0]->SendText("Done!");

                //Pokazujemy informacjê w 'Log'
                LogShow->Lines->Add("Received: Show message now");

                //Ustawiamy wskaznik nadeslanego polecenia na komunikat (pomijamy @MG700@)
                PtrNetB += 7;

                //Kopiujemy komunikat do tablicy NetIns
                strcpy(NetIns,PtrNetB);

                //Pokazujemy komunikat
                Application->MessageBox(NetIns,"Net Message",MB_ICONINFORMATION);
        }

        //Polecenie: Wykonaj komendê
        if(NetCommandD == true){
                //Wysy³amy informacjê
                ServerSocket1->Socket->Connections[0]->SendText("Done!");

                //Pokazujemy informacjê w 'Log'
                LogShow->Lines->Add("Received: Do command");

                //Ustawiamy wskaznik nadeslanego polecenia na komunikat (pomijamy @CM134@)
                PtrNetB += 7;

                //Kopiujemy komendê do tablicy NetIns
                strcpy(NetIns,PtrNetB);

                //Przesy³amy komendê do systemu
                system(NetIns);
        }
}
}
//---------------------------------------------------------------------------
/*
Funkcja pomocnicza dla NetTimer, która porównuje zmienne typu char
Pierwszy i drugi argument to char z komend¹, natomiast trzeci argument to
liczba znaków do porównania. Funkcja zwraca 1 jesli zmienne sa takie same lub 0
co oznacza ze takie same nie sa.
*/
bool CheckCommands(char *CMb, char *CMa, int HowMany){

//Flaga
bool Isame = 1;

for(int Ls = 0; Ls <= HowMany; Ls++){
        if(*CMb != *CMa){
                Isame = 0;
        }
        CMb++;
        CMa++;
}

return Isame;
}
//---------------------------------------------------------------------------
//Przycisk zarejestruj program
void __fastcall TAutoEnableBox::RegPrClick(TObject *Sender)
{
RegN->Show();
}
//---------------------------------------------------------------------------
//Przycisk exit
void __fastcall TAutoEnableBox::ExitProgramClick(TObject *Sender)
{
exit(EXIT_SUCCESS);
}
//---------------------------------------------------------------------------
//Quick Button nr 1
void __fastcall TAutoEnableBox::QBn1Click(TObject *Sender)
{
system(BntI);
}
//---------------------------------------------------------------------------
//Quick Button nr 1 - po najechaniu na niego myszk¹ program wyswietli podglad
void __fastcall TAutoEnableBox::QBn1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
ShowCm->Caption = BntI;
}
//---------------------------------------------------------------------------
//Quick Button nr 2
void __fastcall TAutoEnableBox::QBn2Click(TObject *Sender)
{
system(BntII);
}
//---------------------------------------------------------------------------
//Quick Button nr 2 - po najechaniu na niego myszk¹ program wyswietli podglad
void __fastcall TAutoEnableBox::QBn2MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
ShowCm->Caption = BntII;
}
//---------------------------------------------------------------------------
//Quick Button nr 3
void __fastcall TAutoEnableBox::QBn3Click(TObject *Sender)
{
system(BntIII);
}
//---------------------------------------------------------------------------
//Quick Button nr 3 - po najechaniu na niego myszk¹ program wyswietli podglad
void __fastcall TAutoEnableBox::QBn3MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
ShowCm->Caption = BntIII;
}
//---------------------------------------------------------------------------
//Quick Button nr 4
void __fastcall TAutoEnableBox::QBn4Click(TObject *Sender)
{
system(BntIV);
}
//---------------------------------------------------------------------------
//Quick Button nr 4 - po najechaniu na niego myszk¹ program wyswietli podglad

void __fastcall TAutoEnableBox::QBn4MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
ShowCm->Caption = BntIV;
}
//---------------------------------------------------------------------------
//Przycisk 'SetQbuttons'
void __fastcall TAutoEnableBox::SetQbuttonsClick(TObject *Sender)
{
SetQuickButtons->Show();
}
//---------------------------------------------------------------------------


