object RegN: TRegN
  Left = 337
  Top = 192
  Width = 433
  Height = 183
  AutoSize = True
  Caption = 'Shut Down Computer v1.0 BETA -  Registration'
  Color = clWindow
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 417
    Height = 145
    Color = clWindow
    TabOrder = 0
    object RegProg: TButton
      Left = 8
      Top = 112
      Width = 75
      Height = 25
      Caption = 'Register'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
    end
    object CloseWindow: TButton
      Left = 336
      Top = 112
      Width = 75
      Height = 25
      Caption = 'Close'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = CloseWindowClick
    end
    object Keytext: TLabeledEdit
      Left = 8
      Top = 72
      Width = 401
      Height = 21
      EditLabel.Width = 26
      EditLabel.Height = 13
      EditLabel.Caption = 'Key:'
      EditLabel.Font.Charset = DEFAULT_CHARSET
      EditLabel.Font.Color = clWindowText
      EditLabel.Font.Height = -11
      EditLabel.Font.Name = 'MS Sans Serif'
      EditLabel.Font.Style = [fsBold]
      EditLabel.ParentFont = False
      LabelPosition = lpAbove
      LabelSpacing = 3
      TabOrder = 2
    end
    object IDtext: TLabeledEdit
      Left = 8
      Top = 24
      Width = 401
      Height = 21
      EditLabel.Width = 18
      EditLabel.Height = 13
      EditLabel.Caption = 'ID:'
      EditLabel.Font.Charset = DEFAULT_CHARSET
      EditLabel.Font.Color = clWindowText
      EditLabel.Font.Height = -11
      EditLabel.Font.Name = 'MS Sans Serif'
      EditLabel.Font.Style = [fsBold]
      EditLabel.ParentFont = False
      LabelPosition = lpAbove
      LabelSpacing = 3
      TabOrder = 3
    end
  end
end
