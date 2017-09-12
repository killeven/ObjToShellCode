unit ShellCodeEngine;

interface

type
  SCEInstance = Pointer;
  TCallBack = procedure(name: PAnsiChar); stdcall;
  TEngine = class
  private
    instance: SCEInstance;
  public
    constructor Create;
    destructor Destroy; override;
    function AddObj(path: PAnsiChar): Boolean;
    function AddModule(path: PAnsiChar): Boolean; stdcall;
    function Build(savepath: PAnsiChar; entryname: PAnsiChar): Boolean; stdcall;
    procedure GetFuncList(callback: TCallBack); stdcall;
  end;

implementation

const ShellCodeEngineDll = 'Builder.dll';
function xCreateInstance(): SCEInstance; stdcall; external ShellCodeEngineDll name 'CreateInstance';
procedure xDeleteInstance(instance: SCEInstance); stdcall; external ShellCodeEngineDll name 'DeleteInstance';
function xAddObj(instance: SCEInstance; path: PAnsiChar): Boolean; stdcall; external ShellCodeEngineDll name 'AddObj';
function xAddModule(instance: SCEInstance; path: PAnsiChar): Boolean; stdcall; external ShellCodeEngineDll name 'AddModule';
function xBuild(instance: SCEInstance; savepath: PAnsiChar; entryname: PAnsiChar): Boolean; stdcall; external ShellCodeEngineDll name 'Build';
procedure xGetFuncList(instance: SCEInstance; callback: TCallBack); stdcall; external ShellCodeEngineDll name 'GetFuncList';

{ TEngine }

function TEngine.AddModule(path: PAnsiChar): Boolean;
begin
  Result := xAddModule(instance, path);
end;

function TEngine.AddObj(path: PAnsiChar): Boolean;
begin
  Result := xAddObj(instance, path);
end;

function TEngine.Build(savepath, entryname: PAnsiChar): Boolean;
begin
  Result := xBuild(instance, savepath, entryname);
end;

constructor TEngine.Create;
begin
  instance := xCreateInstance();
end;

destructor TEngine.Destroy;
begin
  xDeleteInstance(instance);
end;

procedure TEngine.GetFuncList(callback: TCallBack);
begin
  xGetFuncList(instance, callback);
end;

end.
