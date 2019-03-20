object Form1: TForm1
  Left = 193
  Top = 107
  BorderStyle = bsDialog
  Caption = 'BASS multi-speaker example'
  ClientHeight = 196
  ClientWidth = 345
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 7
    Top = 0
    Width = 330
    Height = 45
    Caption = '1 - front'
    TabOrder = 0
    object Button1: TButton
      Left = 7
      Top = 14
      Width = 270
      Height = 23
      Caption = 'click here to open a file...'
      TabOrder = 0
      OnClick = Button1Click
    end
  end
  object GroupBox2: TGroupBox
    Left = 7
    Top = 48
    Width = 330
    Height = 45
    Caption = '2 - rear'
    TabOrder = 1
    object Button2: TButton
      Tag = 1
      Left = 7
      Top = 14
      Width = 270
      Height = 23
      Caption = 'click here to open a file...'
      TabOrder = 0
      OnClick = Button1Click
    end
  end
  object GroupBox3: TGroupBox
    Left = 7
    Top = 96
    Width = 330
    Height = 45
    Caption = '3 - center/LFE'
    TabOrder = 2
    object Button3: TButton
      Tag = 2
      Left = 7
      Top = 14
      Width = 270
      Height = 23
      Caption = 'click here to open a file...'
      TabOrder = 0
      OnClick = Button1Click
    end
  end
  object GroupBox4: TGroupBox
    Left = 7
    Top = 144
    Width = 330
    Height = 45
    Caption = '4 - rear center'
    TabOrder = 3
    object Button4: TButton
      Tag = 3
      Left = 7
      Top = 14
      Width = 270
      Height = 23
      Caption = 'click here to open a file...'
      TabOrder = 0
      OnClick = Button1Click
    end
  end
  object Button5: TButton
    Left = 288
    Top = 38
    Width = 41
    Height = 23
    Caption = 'Swap'
    TabOrder = 4
    OnClick = Button5Click
  end
  object Button6: TButton
    Tag = 1
    Left = 288
    Top = 86
    Width = 41
    Height = 23
    Caption = 'Swap'
    TabOrder = 5
    OnClick = Button5Click
  end
  object Button7: TButton
    Tag = 2
    Left = 288
    Top = 134
    Width = 41
    Height = 23
    Caption = 'Swap'
    TabOrder = 6
    OnClick = Button5Click
  end
  object OpenDialog1: TOpenDialog
    Filter = 
      'Streamable files|*.mp3;*.mp2;*.mp1;*.ogg;*.wav;*.aif|All files|*' +
      '.*'
  end
end
