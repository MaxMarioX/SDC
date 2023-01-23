//---------------------------------------------------------------------------

#ifndef Unit5H
#define Unit5H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <CheckLst.hpp>
#include <FileCtrl.hpp>
//---------------------------------------------------------------------------
class TFormShowPR : public TForm
{
__published:	// IDE-managed Components
        TGroupBox *ProcessList;
        TLabel *Label1;
        TBitBtn *BitBtn1;
        TFileListBox *ProcList;
        TBitBtn *BitBtn2;
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFormShowPR(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormShowPR *FormShowPR;
//---------------------------------------------------------------------------
#endif
