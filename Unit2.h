//---------------------------------------------------------------------------

#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TRegN : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TButton *RegProg;
        TButton *CloseWindow;
        TLabeledEdit *Keytext;
        TLabeledEdit *IDtext;
        void __fastcall CloseWindowClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TRegN(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TRegN *RegN;
//---------------------------------------------------------------------------
#endif
