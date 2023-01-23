//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("Unit1.cpp", AutoEnableBox);
USEFORM("Unit5.cpp", FormShowPR);
USEFORM("Unit2.cpp", RegN);
USEFORM("Unit3.cpp", SetQuickButtons);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Shut Down Computer BETA";
                 Application->CreateForm(__classid(TAutoEnableBox), &AutoEnableBox);
                 Application->CreateForm(__classid(TFormShowPR), &FormShowPR);
                 Application->CreateForm(__classid(TRegN), &RegN);
                 Application->CreateForm(__classid(TSetQuickButtons), &SetQuickButtons);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
