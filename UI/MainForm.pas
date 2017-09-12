unit MainForm;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, RzButton, ShellCodeEngine, Vcl.StdCtrls,
  RzCmboBx, Vcl.Mask, RzEdit;

type
  TForm2 = class(TForm)
    btn1: TRzButton;
    btn2: TRzButton;
    btn3: TRzButton;
    dlgOpen1: TOpenDialog;
    mmo1: TMemo;
    cbbEntry: TRzComboBox;
    btn4: TRzButton;
    dlgSave1: TSaveDialog;
    edt1: TRzEdit;
    btn5: TRzButton;
    btn6: TRzButton;
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure btn1Click(Sender: TObject);
    procedure btn2Click(Sender: TObject);
    procedure btn4Click(Sender: TObject);
    procedure btn3Click(Sender: TObject);
    procedure btn5Click(Sender: TObject);
    procedure btn6Click(Sender: TObject);
  private
    { Private declarations }
    Engine: TEngine;
  public
    { Public declarations }
  end;

var
  Form2: TForm2;

implementation

{$R *.dfm}

procedure TForm2.btn1Click(Sender: TObject);
var
  obj: string;
begin
  if (dlgOpen1.Execute()) then
  begin
    for obj in dlgOpen1.Files do
    begin
      if (Engine.AddObj(PAnsiChar(AnsiString(obj)))) then
      begin
        mmo1.Lines.Add(Format('[%s]增加成功!', [obj]));
      end else
      begin
        mmo1.Lines.Add(Format('[%s]增加失败!', [obj]));
      end;
    end;
  end;
end;

procedure TForm2.btn2Click(Sender: TObject);
begin
  //dlgOpen1.Filter := 'DLL FILE|*.dll;';
  //if (dlgOpen1.Execute()) then
  //begin
    if (Engine.AddModule(PAnsiChar(AnsiString(edt1.Text)))) then
    begin
      mmo1.Lines.Add(edt1.Text + ': 增加成功!');
    end else
    begin
      mmo1.Lines.Add(edt1.Text + ': 增加失败!');
    end;
  //end;
end;

procedure TForm2.btn3Click(Sender: TObject);
begin
  if (dlgSave1.Execute()) then
  begin
    if (Engine.Build(PAnsiChar(AnsiString(dlgSave1.FileName)), PAnsiChar(AnsiString(cbbEntry.Items[cbbEntry.ItemIndex])))) then
    begin
      mmo1.Lines.Add('生成成功!');
    end else
    begin
      mmo1.Lines.Add('生成失败!');
    end;
  end;
end;

procedure CallBack(func: PAnsiChar); stdcall;
begin
  Form2.cbbEntry.Items.Add(func);
end;

procedure TForm2.btn4Click(Sender: TObject);
begin
  cbbEntry.Items.Clear;
  Engine.GetFuncList(@CallBack);
  cbbEntry.ItemIndex := 1;
end;

procedure TForm2.btn5Click(Sender: TObject);
begin
  AllocConsole();
end;

procedure TForm2.btn6Click(Sender: TObject);
begin
  FreeConsole();
end;

procedure TForm2.FormCreate(Sender: TObject);
begin
  Engine := TEngine.Create;
end;

procedure TForm2.FormDestroy(Sender: TObject);
begin
  FreeAndNil(Engine);
end;

end.
