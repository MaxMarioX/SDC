//---------------------------------------------------------------------------

#include <vcl.h>
#include <windows.h>
#include <tlhelp32.h>
#include <Lmcons.h>
#include <iostream.h>
#pragma hdrstop

#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormShowPR *FormShowPR;
//---------------------------------------------------------------------------
__fastcall TFormShowPR::TFormShowPR(TComponent* Owner)
        : TForm(Owner)
{
//Czyscimy liste
ProcList->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFormShowPR::BitBtn1Click(TObject *Sender)
{
FormShowPR->Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormShowPR::BitBtn2Click(TObject *Sender)
{
//W pierwszej kolejnosci czyscimy liste
ProcList->Clear();

PROCESSENTRY32 proc32;  //deklaracja struktury
HANDLE hSnapshot;       //uchwyt na CreateToolhelp32Snapshot

hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

//ustawiamy rozmiar struktury
proc32.dwSize = sizeof(PROCESSENTRY32);

//pierwsze wywolanie Process32First
if(Process32First(hSnapshot, &proc32)){
        //glowna petla wyœwietlaj¹ca procesy przez Process32Next
        while(Process32Next(hSnapshot, &proc32)){
                ProcList->Items->Add((AnsiString)proc32.szExeFile);
        }
}

CloseHandle(hSnapshot);        
}
//---------------------------------------------------------------------------

