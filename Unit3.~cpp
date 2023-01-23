//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define LoadError Application->MessageBox("Cannot load settings program! Install program once again!","SDC",MB_ICONERROR);
TSetQuickButtons *SetQuickButtons;

char _BntI[100], _BntII[100], _BntIII[100], _BntIV[100];
//---------------------------------------------------------------------------
__fastcall TSetQuickButtons::TSetQuickButtons(TComponent* Owner)
        : TForm(Owner)
{
HKEY hKey;

DWORD _SizeValueBntI = sizeof(_BntI);
DWORD _SizeValueBntII = sizeof(_BntII);
DWORD _SizeValueBntIII = sizeof(_BntIII);
DWORD _SizeValueBntIV = sizeof(_BntIV);
//Wczytujemy polecenia do przycisków

//Otwieramy klucz, w którym program przechowuje ustawienia
if((RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\SDCsettings",0,KEY_READ,&hKey)) == ERROR_SUCCESS){
        //Przyciski 'Quick Button'
        if((RegQueryValueEx(hKey,"QBn1",0,0,(LPBYTE)&_BntI,&_SizeValueBntI)) != ERROR_SUCCESS){
                LoadError;
                exit(EXIT_FAILURE);
        }
        Bn1->Text = _BntI;
        if((RegQueryValueEx(hKey,"QBn2",0,0,(LPBYTE)&_BntII,&_SizeValueBntII)) != ERROR_SUCCESS){
                LoadError;
                exit(EXIT_FAILURE);
        }
        Bn2->Text = _BntII;
        if((RegQueryValueEx(hKey,"QBn3",0,0,(LPBYTE)&_BntIII,&_SizeValueBntIII)) != ERROR_SUCCESS){
                LoadError;
               exit(EXIT_FAILURE);
        }
        Bn3->Text = _BntIII;
        if((RegQueryValueEx(hKey,"QBn4",0,0,(LPBYTE)&_BntIV,&_SizeValueBntIV)) != ERROR_SUCCESS){
                LoadError;
                exit(EXIT_FAILURE);
        }
        Bn4->Text = _BntIV;
}
RegCloseKey(hKey);
}
//---------------------------------------------------------------------------

void __fastcall TSetQuickButtons::CloseNowClick(TObject *Sender)
{
SetQuickButtons->Close();
}
//---------------------------------------------------------------------------
void __fastcall TSetQuickButtons::SaveNowClick(TObject *Sender)
{
HKEY hKey;

short int _SaveWarning = 0;

//Otwieramy klucz, w którym program przechowuje ustawienia
if((RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\SDCsettings",0,KEY_ALL_ACCESS,&hKey)) == ERROR_SUCCESS){
        //Przyciski 'Quick Button'

        strcpy(_BntI,Bn1->Text.c_str());
        if((RegSetValueEx(hKey,"QBn1",0,REG_SZ,(LPBYTE)&_BntI,sizeof(_BntI))) != ERROR_SUCCESS){
                _SaveWarning++;
        }
        strcpy(_BntII,Bn2->Text.c_str());
        if((RegSetValueEx(hKey,"QBn2",0,REG_SZ,(LPBYTE)&_BntII,sizeof(_BntII))) != ERROR_SUCCESS){
                _SaveWarning++;
        }
        strcpy(_BntIII,Bn3->Text.c_str());
        if((RegSetValueEx(hKey,"QBn3",0,REG_SZ,(LPBYTE)&_BntIII,sizeof(_BntIII))) != ERROR_SUCCESS){
                _SaveWarning++;
        }
        strcpy(_BntIV,Bn4->Text.c_str());
        if((RegSetValueEx(hKey,"QBn4",0,REG_SZ,(LPBYTE)&_BntIV,sizeof(_BntIV))) != ERROR_SUCCESS){
                _SaveWarning++;
        }
}else{
        Application->MessageBoxA("Error!","SDC",MB_ICONERROR);
}

if(_SaveWarning == 0){
        Application->MessageBoxA("Changes has been saved! Run application once again!","SDC",MB_ICONINFORMATION);
}
RegCloseKey(hKey);
}
//---------------------------------------------------------------------------
