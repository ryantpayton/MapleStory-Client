object Form1: TForm1
  Left = 193
  Top = 105
  Caption = 'BASS recording example'
  ClientHeight = 93
  ClientWidth = 328
  Color = clBtnFace
  Constraints.MaxHeight = 120
  Constraints.MinHeight = 90
  Constraints.MinWidth = 336
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  DesignSize = (
    328
    93)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 3
    Top = 30
    Width = 118
    Height = 13
    Alignment = taCenter
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    ExplicitWidth = 154
  end
  object Bevel1: TBevel
    Left = 111
    Top = 35
    Width = 211
    Height = 20
    Anchors = [akTop, akRight]
    ExplicitLeft = 147
  end
  object lPos: TLabel
    Left = 135
    Top = 39
    Width = 196
    Height = 13
    Alignment = taCenter
    Anchors = [akTop, akRight]
    AutoSize = False
    ExplicitLeft = 171
  end
  object VolumeLevel: TLabel
    Left = 8
    Top = 72
    Width = 64
    Height = 13
    Caption = 'VolumeLevel:'
  end
  object Label2: TLabel
    Left = 88
    Top = 72
    Width = 6
    Height = 13
    Caption = '0'
  end
  object ComboBox1: TComboBox
    Left = 8
    Top = 8
    Width = 93
    Height = 21
    Style = csDropDownList
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    TabOrder = 0
    OnChange = ComboBox1Change
  end
  object bRecord: TButton
    Left = 111
    Top = 8
    Width = 90
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Record'
    TabOrder = 1
    OnClick = bRecordClick
  end
  object bPlay: TButton
    Left = 208
    Top = 8
    Width = 53
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Play'
    TabOrder = 2
    OnClick = bPlayClick
  end
  object TrackBar1: TTrackBar
    Left = 6
    Top = 45
    Width = 95
    Height = 15
    Anchors = [akLeft, akTop, akRight]
    Max = 100
    TabOrder = 3
    ThumbLength = 9
    TickMarks = tmBoth
    TickStyle = tsNone
    OnChange = TrackBar1Change
  end
  object bSave: TButton
    Left = 268
    Top = 8
    Width = 53
    Height = 22
    Anchors = [akTop, akRight]
    Caption = 'Save'
    TabOrder = 4
    OnClick = bSaveClick
  end
  object PosTimer: TTimer
    Interval = 200
    OnTimer = PosTimerTimer
    Left = 120
    Top = 32
  end
  object SaveDialog: TSaveDialog
    DefaultExt = 'wav'
    Filter = 'WAV files|*.wav|All files|*.*'
    Title = 'Save WAV...'
    Left = 152
    Top = 32
  end
end
