//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormMsgLogOut : public TForm
{
__published:	// IDE-managed Components
        TPanel *Panel1;
        TLabel *Label1;
private:	// User declarations
public:		// User declarations
        __fastcall TFormMsgLogOut(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMsgLogOut *FormMsgLogOut;
//---------------------------------------------------------------------------
#endif
