object FormShowPR: TFormShowPR
  Left = 652
  Top = 134
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Shut Down Computer v1.0 BETA'
  ClientHeight = 507
  ClientWidth = 354
  Color = clWindow
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object ProcessList: TGroupBox
    Left = 8
    Top = 0
    Width = 337
    Height = 457
    TabOrder = 0
    object Label1: TLabel
      Left = 8
      Top = 16
      Width = 130
      Height = 29
      Caption = 'Process list:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -23
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object ProcList: TFileListBox
      Left = 8
      Top = 48
      Width = 321
      Height = 401
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ItemHeight = 24
      ParentFont = False
      TabOrder = 0
    end
  end
  object BitBtn1: TBitBtn
    Left = 208
    Top = 472
    Width = 113
    Height = 25
    Caption = 'Close'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clRed
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 1
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 40
    Top = 472
    Width = 115
    Height = 25
    Caption = 'Show'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clOlive
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 2
    OnClick = BitBtn2Click
  end
end
