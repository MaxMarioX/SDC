//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <TabNotBk.hpp>
#include <Menus.hpp>
#include <FileCtrl.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
class TAutoEnableBox : public TForm
{
__published:	// IDE-managed Components
        TTimer *Time;
        TTimer *ActivAutoMode;
        TBitBtn *ExitButton;
        TTabbedNotebook *MainWindow;
        TPanel *Panel1;
        TRadioGroup *RadioGroup1;
        TRadioButton *Check1minute;
        TRadioButton *Check3minutes;
        TRadioButton *Check5minutes;
        TBitBtn *AutoModeSave;
        TGroupBox *SM;
        TLabeledEdit *TextLogOut;
        TLabeledEdit *TextRestart;
        TLabeledEdit *TextShutDown;
        TBitBtn *AutoModeRun;
        TBitBtn *AutoModeStop;
        TGroupBox *SetEnabledBox;
        TCheckBox *CheckAutoLog;
        TCheckBox *CheckAutoRestart;
        TCheckBox *CheckAutoShutDown;
        TLabel *Label1;
        TEdit *HtimeAutoLog;
        TEdit *MtimeAutoLog;
        TEdit *HtimeAutoRestart;
        TEdit *MtimeAutoRestart;
        TLabel *Label2;
        TLabel *Label3;
        TEdit *HtimeAutoShutDown;
        TEdit *MtimeAutoShutDown;
        TGroupBox *BoxLogOut;
        TBitBtn *LogOutButton;
        TLabel *UserLoggedView;
        TLabel *Logged;
        TGroupBox *BoxTime;
        TLabel *THour;
        TLabel *Label7;
        TLabel *TMinute;
        TLabel *Label8;
        TLabel *TSecond;
        TGroupBox *BoxRestartShutDown;
        TBitBtn *RestartButton;
        TBitBtn *ShutDownButton;
        TLabel *ComputerName;
        TLabel *ComputerNameView;
        TCheckBox *CheckEnableAutoExit;
        TPanel *Panel2;
        TRadioGroup *BoxActions;
        TRadioButton *AcLogOut;
        TRadioButton *AcRestart;
        TRadioButton *AcShutDown;
        TTimer *AutoExitModeFields;
        TTimer *AutoExitMode;
        TBitBtn *Stop;
        TBitBtn *Run;
        TBitBtn *Refresh;
        TBitBtn *Save;
        TGroupBox *BoxShowMessages;
        TCheckBox *ChechShowMessages;
        TGroupBox *SMDB;
        TLabeledEdit *TextLogOutDB;
        TLabeledEdit *TextRestartDB;
        TLabeledEdit *TextShutDownDB;
        TGroupBox *BoxName;
        TLabeledEdit *ProgramName;
        TGroupBox *BoxShowMessagesDB;
        TCheckBox *MsgActionAEM;
        TGroupBox *BoxComputerAdress;
        TPanel *Panel3;
        TLabeledEdit *IpAdress;
        TButton *OkNet;
        TGroupBox *BoxWorkMode;
        TBitBtn *Con;
        TBitBtn *Lis;
        TBitBtn *DiscNet;
        TClientSocket *ClientSocket;
        TEdit *PortP;
        TGroupBox *BoxListen;
        TGroupBox *BoxNetStatus;
        TLabel *Label4;
        TLabel *InfoClient;
        TEdit *PortS;
        TButton *OKlis;
        TServerSocket *ServerSocket1;
        TGroupBox *BoxPanelControl;
        TGroupBox *BoxQuickCM;
        TButton *NowLogOut;
        TButton *NowRestart;
        TButton *NowShutDown;
        TGroupBox *BoxMessageNet;
        TEdit *NowMessage;
        TButton *NowSendOne;
        TGroupBox *BoxLogShow;
        TMemo *LogShow;
        TGroupBox *BoxCommand;
        TButton *NowSendTwo;
        TMemo *NowCommandText;
        TTimer *NetTimer;
        TGroupBox *BoxStatus;
        TLabel *sAutoMode;
        TLabel *sAutoExitMode;
        TLabel *sNetMode;
        TLabel *iAutoMode;
        TLabel *iAutoExitMode;
        TLabel *iNetMode;
        TGroupBox *BoxMainButton;
        TBitBtn *RegPr;
        TBitBtn *ExitProgram;
        TLabel *RegFor;
        TLabel *RegOwner;
        TGroupBox *QuickBn;
        TBitBtn *QBn1;
        TBitBtn *QBn2;
        TBitBtn *QBn3;
        TBitBtn *QBn4;
        TGroupBox *WhatIsThat;
        TBitBtn *SetQbuttons;
        TLabel *ShowCm;
        void __fastcall LogOutButtonClick(TObject *Sender);
        void __fastcall RestartButtonClick(TObject *Sender);
        void __fastcall ShutDownButtonClick(TObject *Sender);
        void __fastcall TimeTimer(TObject *Sender);
        void __fastcall ExitButtonClick(TObject *Sender);
        void __fastcall ActivAutoModeTimer(TObject *Sender);
        void __fastcall AutoModeRunClick(TObject *Sender);
        void __fastcall AutoModeStopClick(TObject *Sender);
        void __fastcall AutoModeSaveClick(TObject *Sender);
        void __fastcall RefreshClick(TObject *Sender);
        void __fastcall AutoExitModeFieldsTimer(TObject *Sender);
        void __fastcall RunClick(TObject *Sender);
        void __fastcall AutoExitModeTimer(TObject *Sender);
        void __fastcall ExeNameFilterChange(TObject *Sender);
        void __fastcall StopClick(TObject *Sender);
        void __fastcall SaveClick(TObject *Sender);
        void __fastcall ClientSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall OkNetClick(TObject *Sender);
        void __fastcall ClientSocketError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall ConClick(TObject *Sender);
        void __fastcall LisClick(TObject *Sender);
        void __fastcall OKlisClick(TObject *Sender);
        void __fastcall ServerSocket1Accept(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall DiscNetClick(TObject *Sender);
        void __fastcall ServerSocketClient1Disconnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ServerSocketClient1Error(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall ClientSocketDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall NowLogOutClick(TObject *Sender);
        void __fastcall ServerSocketClient1Read(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall NowRestartClick(TObject *Sender);
        void __fastcall NowShutDownClick(TObject *Sender);
        void __fastcall NowSendTwoClick(TObject *Sender);
        void __fastcall NowSendOneClick(TObject *Sender);
        void __fastcall ClientSocketRead(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall NetTimerTimer(TObject *Sender);
        void __fastcall RegPrClick(TObject *Sender);
        void __fastcall ExitProgramClick(TObject *Sender);
        void __fastcall QBn1Click(TObject *Sender);
        void __fastcall QBn1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall QBn2Click(TObject *Sender);
        void __fastcall QBn2MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall QBn3Click(TObject *Sender);
        void __fastcall QBn3MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall QBn4Click(TObject *Sender);
        void __fastcall QBn4MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
        void __fastcall SetQbuttonsClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TAutoEnableBox(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAutoEnableBox *AutoEnableBox;
//---------------------------------------------------------------------------
#endif
