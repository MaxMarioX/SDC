//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRegN *RegN;
//---------------------------------------------------------------------------
__fastcall TRegN::TRegN(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
//Przycisk close
void __fastcall TRegN::CloseWindowClick(TObject *Sender)
{
RegN->Close();
}
//---------------------------------------------------------------------------
 