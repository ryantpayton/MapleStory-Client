object Form1: TForm1
  Left = 435
  Top = 272
  BorderStyle = bsDialog
  Caption = 'Bass Plugin Demo'
  ClientHeight = 181
  ClientWidth = 400
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
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 400
    Height = 181
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 0
    object lblInfo: TLabel
      Left = 8
      Top = 134
      Width = 3
      Height = 13
    end
    object GroupBox1: TGroupBox
      Left = 0
      Top = 0
      Width = 393
      Height = 97
      TabOrder = 0
      object ListView_PlugIns: TListView
        Left = 8
        Top = 16
        Width = 377
        Height = 73
        Columns = <
          item
            Caption = 'Loaded Plugin '
            MaxWidth = 90
            MinWidth = 90
            Width = 90
          end
          item
            Caption = 'Supported Formats'
            MaxWidth = 280
            MinWidth = 280
            Width = 280
          end>
        ColumnClick = False
        GridLines = True
        ReadOnly = True
        TabOrder = 0
        ViewStyle = vsReport
      end
    end
    object Button1: TButton
      Left = 8
      Top = 104
      Width = 385
      Height = 25
      Caption = 'cklick here to open a file'
      TabOrder = 1
      OnClick = Button1Click
    end
    object ScrollBar1: TScrollBar
      Left = 8
      Top = 154
      Width = 385
      Height = 16
      PageSize = 0
      TabOrder = 2
      OnScroll = ScrollBar1Scroll
    end
  end
  object open1: TOpenDialog
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 500
    OnTimer = Timer1Timer
    Left = 32
  end
end
