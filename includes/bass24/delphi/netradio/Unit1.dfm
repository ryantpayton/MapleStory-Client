object Form1: TForm1
  Left = 192
  Top = 109
  BorderStyle = bsToolWindow
  Caption = 'BASS internet radio tuner'
  ClientHeight = 262
  ClientWidth = 277
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 277
    Height = 262
    Align = alClient
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 8
      Top = 0
      Width = 265
      Height = 73
      Caption = 'Presets'
      TabOrder = 0
      object Label1: TLabel
        Left = 8
        Top = 24
        Width = 52
        Height = 13
        Caption = 'Broadband'
      end
      object Label2: TLabel
        Left = 8
        Top = 48
        Width = 35
        Height = 13
        Caption = 'Modem'
      end
      object Button1: TButton
        Left = 64
        Top = 14
        Width = 33
        Height = 25
        Caption = '1'
        TabOrder = 0
        OnClick = Button1Click
      end
      object Button2: TButton
        Tag = 2
        Left = 104
        Top = 14
        Width = 33
        Height = 25
        Caption = '2'
        TabOrder = 1
        OnClick = Button1Click
      end
      object Button3: TButton
        Tag = 4
        Left = 144
        Top = 14
        Width = 33
        Height = 25
        Caption = '3'
        TabOrder = 2
        OnClick = Button1Click
      end
      object Button4: TButton
        Tag = 6
        Left = 184
        Top = 14
        Width = 33
        Height = 25
        Caption = '4'
        TabOrder = 3
        OnClick = Button1Click
      end
      object Button5: TButton
        Tag = 8
        Left = 224
        Top = 14
        Width = 33
        Height = 25
        Caption = '5'
        TabOrder = 4
        OnClick = Button1Click
      end
      object Button6: TButton
        Tag = 1
        Left = 64
        Top = 41
        Width = 33
        Height = 25
        Caption = '1'
        TabOrder = 5
        OnClick = Button1Click
      end
      object Button7: TButton
        Tag = 3
        Left = 104
        Top = 41
        Width = 33
        Height = 25
        Caption = '2'
        TabOrder = 6
        OnClick = Button1Click
      end
      object Button8: TButton
        Tag = 5
        Left = 144
        Top = 41
        Width = 33
        Height = 25
        Caption = '3'
        TabOrder = 7
        OnClick = Button1Click
      end
      object Button9: TButton
        Tag = 7
        Left = 184
        Top = 41
        Width = 33
        Height = 25
        Caption = '4'
        TabOrder = 8
        OnClick = Button1Click
      end
      object Button10: TButton
        Tag = 9
        Left = 224
        Top = 41
        Width = 33
        Height = 25
        Caption = '5'
        TabOrder = 9
        OnClick = Button1Click
      end
    end
    object GroupBox2: TGroupBox
      Left = 8
      Top = 76
      Width = 265
      Height = 101
      Caption = 'Currently playing'
      TabOrder = 1
      object Label3: TLabel
        Left = 11
        Top = 16
        Width = 245
        Height = 25
        Alignment = taCenter
        AutoSize = False
        ShowAccelChar = False
        WordWrap = True
      end
      object Label4: TLabel
        Left = 8
        Top = 40
        Width = 241
        Height = 41
        Alignment = taCenter
        AutoSize = False
        Caption = 'not playing'
        ShowAccelChar = False
        WordWrap = True
      end
      object Label5: TLabel
        Left = 8
        Top = 80
        Width = 249
        Height = 13
        Alignment = taCenter
        AutoSize = False
        ShowAccelChar = False
        WordWrap = True
      end
    end
    object GroupBox3: TGroupBox
      Left = 8
      Top = 176
      Width = 265
      Height = 81
      Caption = 'Proxy server'
      TabOrder = 2
      object Label6: TLabel
        Left = 10
        Top = 40
        Width = 112
        Height = 13
        Caption = '(user:pass@]server:port'
      end
      object ed_ProxyServer: TEdit
        Left = 8
        Top = 16
        Width = 249
        Height = 21
        MaxLength = 100
        ParentShowHint = False
        ShowHint = True
        TabOrder = 0
      end
      object cbDirectConnection: TCheckBox
        Left = 8
        Top = 60
        Width = 113
        Height = 17
        Caption = 'Direct connection'
        TabOrder = 1
      end
    end
  end
end
