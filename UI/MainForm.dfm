object Form2: TForm2
  Left = 0
  Top = 0
  Caption = 'Form2'
  ClientHeight = 370
  ClientWidth = 550
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object btn1: TRzButton
    Left = 352
    Top = 16
    Height = 23
    Caption = #22686#21152'OBJ'
    TabOrder = 0
    OnClick = btn1Click
  end
  object btn2: TRzButton
    Left = 352
    Top = 45
    Caption = #22686#21152#27169#22359
    TabOrder = 1
    OnClick = btn2Click
  end
  object btn3: TRzButton
    Left = 457
    Top = 76
    Caption = #29983#25104
    TabOrder = 2
    OnClick = btn3Click
  end
  object mmo1: TMemo
    Left = 0
    Top = 113
    Width = 550
    Height = 257
    Align = alBottom
    ImeName = #20013#25991'('#31616#20307') - '#24517#24212' Bing '#36755#20837#27861
    TabOrder = 3
    ExplicitTop = 80
    ExplicitWidth = 474
  end
  object cbbEntry: TRzComboBox
    Left = 25
    Top = 76
    Width = 304
    Height = 21
    Style = csDropDownList
    ImeName = #20013#25991'('#31616#20307') - '#24517#24212' Bing '#36755#20837#27861
    TabOrder = 4
  end
  object btn4: TRzButton
    Left = 352
    Top = 76
    Caption = #33719#21462#20989#25968
    TabOrder = 5
    OnClick = btn4Click
  end
  object edt1: TRzEdit
    Left = 25
    Top = 49
    Width = 304
    Height = 21
    Text = ''
    ImeName = #20013#25991'('#31616#20307') - '#24517#24212' Bing '#36755#20837#27861
    TabOrder = 6
  end
  object btn5: TRzButton
    Left = 106
    Top = 15
    Caption = 'showlog'
    TabOrder = 7
    OnClick = btn5Click
  end
  object btn6: TRzButton
    Left = 25
    Top = 15
    Caption = 'hidelog'
    TabOrder = 8
    OnClick = btn6Click
  end
  object dlgOpen1: TOpenDialog
    Filter = 'COMMON OBJECT FILE|*.Obj;'
    Options = [ofHideReadOnly, ofAllowMultiSelect, ofEnableSizing]
    Left = 256
  end
  object dlgSave1: TSaveDialog
    Left = 208
  end
end
