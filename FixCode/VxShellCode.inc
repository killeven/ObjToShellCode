{$ifndef VX_SHELLCODE}
{$define VX_SHELLCODE}

//  ============================================================================
//  Memory Function's

//  ============================================================================
//  String Function's

function _StrlenA(lpszStr: PAnsiChar): LongWord;
asm
{$ifdef CPUX64}
  push  rdi

  mov   rdi, rcx

  xor   rcx, rcx  //  6 bytes
  not   rcx
  //or    rcx, -1     //  7 bytes

  xor   rax, rax
  repne scasb
  not   rcx
  dec   rcx

  pop   rdi
{$else}
  push  edi

  mov   edi, eax
  or    ecx, -1
  xor   eax, eax
  repne scasb
  not   ecx
  dec   ecx
  mov	eax, ecx
  pop   edi
{$endif}
end;

function _StrlenW(lpszStr: PWideChar): LongWord;
asm
{$ifdef CPUX64}
  push  rdi

  mov   rdi, rcx

  xor   rcx, rcx  //  6 bytes
  not   rcx
  //or    rcx, -1     //  7 bytes

  xor   rax, rax
  repne scasw
  not   rcx
  dec   rcx

  pop   rdi
{$else}
  push  edi

  mov   edi, eax
  or    ecx, -1
  xor   eax, eax
  repne scasw
  not   ecx
  dec   ecx

  pop   edi
{$endif}
end;

//  ============================================================================
//  Hash Function's

function _HashStringFromRol3Xor(lpszStr: PAnsiChar): LongWord;
asm
{$ifdef CPUX64}
{$else}
  push  esi

  cdq
  test  eax, eax
  jz    @@finished

  mov   esi, eax
  xor   eax, eax


@@calc_hash:
  lodsb
  test  al, al
  jz    @@finished

	rol		edx, 3
	xor		dl, al
  jmp   @@calc_hash

@@finished:
  mov   eax, edx
  pop   esi
{$endif}
end;

//function _HashStringFromRol3Xor

function _HashStringFromRor13Add(lpszStr: PAnsiChar): LongWord;
asm
{$ifdef CPUX64}
{$else}
  push  esi

  cdq
  test  eax, eax
  jz    @@finished

  mov   esi, eax
  xor   eax, eax

@@calc_hash:
  lodsb
  test  al, al
  jz    @@finished

	ror		edx, 13
	add		edx, eax
  jmp   @@calc_hash

@@finished:
  mov   eax, edx
  pop   esi
{$endif}
end;

//  ============================================================================
//  ShellCode Function's

function _GetDelta(): UIntPtr;
asm
{$ifdef CPUX64}
  xor   rax, rax
{$else}
  call  @Delta
@Delta:
  pop   eax
  sub   eax, offset @Delta
{$endif}
end;

function _GetFixAddress(p: Pointer): Pointer;
asm
{$ifdef CPUX64}
  mov   rax, rcx
{$else}
  call  @Delta
@Delta:
  pop   ecx
  sub   ecx, offset @Delta
  add   eax, ecx
{$endif}
end;

function _GetFixString(p: PTChar): PTChar;
asm
{$ifdef CPUX64}
  mov   rax, rcx
{$else}
  call  @Delta
@Delta:
  pop   ecx
  sub   ecx, offset @Delta
  add   eax, ecx
{$endif}
end;


function _GetDelta_Pas(lpParam: Pointer): UIntPtr; stdcall;
{$ifdef WIN32}
const
  CALL_CODE_LENGTH = 5;
var
  lpCallAddr, lpGetDeltaAddr: Pointer;
begin
  if (lpParam = nil) then
  begin
    Result := _GetDelta_Pas(@_GetDelta_Pas);
  end else
  begin
    lpCallAddr := Pointer(PLongWord(LongWord(@lpParam) - SizeOf(LongWord))^ - CALL_CODE_LENGTH);
    lpGetDeltaAddr := Pointer(LongWord(lpCallAddr) + PLongword(LongWord(lpCallAddr) + SizeOf(Byte))^);
    Result := (LongWord(lpGetDeltaAddr) + CALL_CODE_LENGTH) - UIntPtr(lpParam);
  end;
end;
{$else}
begin
  Result := 0;
end;
{$endif}

function _GetKernelBase_PEB(): HMODULE;
asm
{$ifdef WIN64}
  //  A:(9 byte)
  //mov   rax, [gs:60h]

  //  B:(8 bytes)
  //push  rsi
  //push  60h
  //pop   rsi
  //lods  [gs:60h]
  //pop   rsi

  //  C:(8 bytes)
  xor   rax, rax
  mov   rax, [gs:rax + 60h]

  //  A:(18 bytes)
  mov   rax, [rax + 18h]
  mov   rax, [rax + 30h]  //  InInitializationOrderModuleList : _LIST_ENTRY
  mov   rax, [rax]
  mov   rax, [rax]
  mov   rax, [rax + 10h]

  //  B:(21 bytes)win7+   (理论上来说无法保证准确性[比A好一点]-所以只适合kernel32)
  {mov   rdx, [rax + 18h]
  mov   rdx, [rdx + 30h]  //  InInitializationOrderModuleList : _LIST_ENTRY
@@next_module:
  mov   rax, [rdx + 10h]
  cmp   byte [rdx + 38h], 18h //  (kernel32.dll-宽字符长24字节)
  mov   rdx, [rdx]
  jnz   @@next_module}


{$else}
  //  A:(7 bytes)
  //mov   eax, [fs:30h]

  //  B:(7 bytes)
  //push  esi
  //push  30h
  //pop   esi
  //lods  [fs:esi]
  //pop   esi

  //  C:(5 bytes)
  //cdq
  //mov   eax, [fs:edx + 30h]
  xor	eax, eax
  mov   eax, [fs:eax + 30h]
{
  test  eax, eax
  jns   @@os_Nt

@@os_9x:
  mov   eax, [eax + 34h]
  //lea   eax, [eax + 7ch]
  //mov   eax, [eax + 3ch]
  mov   eax, [eax + 0b8h]
  ret
}
@@os_Nt:
  //  A:(13 bytes)win7+   (理论上来说无法保证准确性-所以只适合kernel32)
  mov   eax, [eax + 0ch]
  mov   eax, [eax + 14h]  //  InMemoryOrderModuleList : _LIST_ENTRY
  mov   eax, [eax]
  mov   eax, [eax]
  mov   eax, [eax + 10h]

  //  B:(17 bytes)win7+   (理论上来说无法保证准确性[比A,B好一点]-所以只适合kernel32)
  {mov   edx, [eax + 0ch]
  mov   edx, [edx + 1ch]  //  InInitializationOrderModuleList : _LIST_ENTRY
@@next_module:
  mov   eax, [edx + 8h]
  cmp   byte [edx + 1ch], 18h //  (kernel32.dll-宽字符长24字节)
  mov   edx, [edx]
  jnz   @@next_module}

  //  C:(14 bytes)win7+   (理论上来说无法保证准确性-所以只适合kernel32)
  {push  esi
  mov   eax, [eax + 0ch]
  mov   esi, [eax + 0ch]  //  InLoadOrderModuleList : _LIST_ENTRY
  lodsd
  xchg  esi, eax
  lodsd
  mov   eax, [eax + 18h]  //  +0x018 DllBase
  pop   esi}
{$endif}
end;

//  不区分大小写的宽字符串Hash(Ror13 & add)
function _String_Ror13HashIW(lpszStr: PWideChar): LongWord;
asm
{$ifdef CPUX64}
  push  rsi
  push  rdi

  mov   rsi, rcx
  

{$else}
  push  esi
  push  edi
  mov   esi, eax
  xor   eax, eax

@@next_char:
  lodsb
  or    al, al
  jz    @@finished

  mov   edx, eax
  add   dl, 9fh
  sub   dl, 1ah
  jnb   @@not_lowercase
  sub   al, 20h

@@not_lowercase:
  ror   edi, 13
  add   edi, eax
  jmp   @@next_char

@@finished:
  mov   eax, edi
  pop   edi
  pop   esi
{$endif}
end;

function _GetModuleHandleW_FromHash(nNameHash: LongWord): HMODULE;
asm
{$ifdef WIN64}
  push  rsi
  push  rdi

  mov   ebp, ecx

  xor   rax, rax
  mov   rax, [gs:rax + 60h]

  mov   rdx, [rax + 18h]
  mov   r9, [rdx + 30h]  //  InInitializationOrderModuleList : _LIST_ENTRY
@@next_mod:
  movzx	ecx, word ptr[r9 + 38h]   //  .FullDllName.Length
  jecxz @@error                   //  判断模块名称是否为NULL
  mov   rsi, [r9 + 40h]			      //  .FullDllName.Buffer
  xor   edi, edi                  //  dwhash = 0

@@loop_modname:
  xor   rax, rax
  lodsb
  mov   rdx, rax
  add   dl, 9fh
  sub   dl, 1ah
  jnb   @@not_lowercase
  sub   al, 20h

@@not_lowercase:
  ror   edi, 13
  add   edi, eax
  loop	@@loop_modname

  cmp   edi, ebp          //  "kernel32.dll"(WideChar) = 06A4ABC5Bh
  mov   rax, [r9 + 10h]

  mov   r9, [r9]
  jne   @@next_mod
  jmp   @@finished

@@error:
  xor   rax, rax

@@finished:
  pop   rdi
  pop   rsi
{$else}
  pushad

  mov   ebp, eax

  cdq
  mov   edx, [fs:edx + 30h]

  mov   eax, [edx + 0ch]
  mov   ebx, [eax + 14h]
@@next_mod:
  movzx	ecx, word ptr[ebx + 24h]  //  .FullDllName.Length
  jecxz @@error                   //  判断模块名称是否为NULL
  mov   esi, [ebx + 28h]			    //  .FullDllName.Buffer
  xor   edi, edi					        //  dwhash = 0

@@loop_modname:
  xor   eax, eax
  lodsb
  mov   edx, eax
  add   dl, 9fh
  sub   dl, 1ah
  jnb   @@not_lowercase
  sub   al, 20h

@@not_lowercase:
  ror   edi, 13
  add   edi, eax
  loop	@@loop_modname

  cmp   edi, ebp          //  "kernel32.dll"(WideChar) = 06A4ABC5Bh
  mov   eax, [ebx + 10h]

  mov   ebx, [ebx]
  jne   @@next_mod
  jmp   @@finished

@@error:
  xor   eax, eax

@@finished:
  mov   [esp+(4*7)], eax
  popad
{$endif}
end;

function _GetProcAddress_FromHash(nFuncHash: LongWord; fnGetProcAddres: Pointer; hModuleBase: HMODULE): Pointer;
{$ifdef WIN64}
asm
  push  rcx
  push  rdx
  push  rbx
  push  rdi
  push  rsi
  push  rbp
  push  r10
  push  r11

  mov   rbp,  r8                //  hModuleBase save to rbp
  mov   r10,  rdx               //  fnGetProcAddress save to r11
  mov   r11,  rcx               //  nFuncHash save to rbx

	mov		eax, [rbp+3ch]          //  PE
	mov		eax, [rbp+rax+88h]      //  Export Table RVA
	lea		rsi, [rbp+rax+18h]      //  Export Table VA+18h
	lodsd
	xchg  rax, rcx                //  NumberOfNames
	lodsd                         //  AddressOfFunctions
	push  rax
	lodsd                         //  AddressOfNames
	xchg  rax, rdx
	lodsd                         //  AddressOfNameOrdinals
	add   rax, rbp

	xchg	rax, rbx
	lea		rsi, [rbp+rdx]

@@next_func:
	lodsd
	add		rax, rbp

	xor		edi, edi
	xchg	rax, rdi
	//  Calc function name hash
	xor		edx, edx
@@calc_hash:
	rol		edx, 3
	xor		dl, byte ptr[rdi]
	inc   rdi
	cmp   byte ptr[rdi], 0
	jnz   @@calc_hash

  //  comp hash
  cmp   r11d, edx
  jnz   @@loop_func

    //  Needed function found
    or    r10, r10
    jz    @@get_ord

      mov		eax, [rsi-4]
			add		rax, rbp

      sub   rsp, (8*4)
      mov   rdx, rax
      mov   rcx, rbp
    	call	r10                 //  GetProcAddress
      add   rsp, (8*4)

      jmp   @@jmp_end

@@get_ord:
      movzx eax, word ptr[rbx]  //  Name ordinal
			shl   eax, 2              //  Multiply by 4
			add   eax, [rsp]
			mov   eax, [rax+rbp]
			add		rax, rbp

@@jmp_end:
    jmp   @@finished

  //
@@loop_func:
	inc		rbx
	inc		rbx
	loop  @@next_func

@@finished:
	pop   rcx

  pop   r11
  pop   r10
  pop   rbp
  pop   rsi
  pop   rdi
  pop   rbx
  pop   rdx
  pop   rcx
end;
{$else}
const
  pushad_eax = 7 * 4;
  pushad_ecx = 6 * 4;
  pushad_edx = 5 * 4;
asm
  pushad
	mov		ebp, [esp+pushad_ecx]
	mov		eax, [ebp+3ch]          //  PE
	mov		eax, [ebp+eax+78h]      //  Export Table RVA
	lea		esi, [ebp+eax+18h]      //  Export Table VA+18h
	lodsd
	xchg  eax, ecx                //  NumberOfNames
	lodsd                         //  AddressOfFunctions
	push  eax
	lodsd                         //  AddressOfNames
	xchg  eax, edx
	lodsd                         //  AddressOfNameOrdinals
	add   eax, ebp

	xchg	eax, ebx
	lea		esi, [edx+ebp]

@@next_func:
	lodsd
	add		eax, ebp

	xor		edi, edi
	xchg	eax, edi
	//  Calc function name hash
	xor		edx, edx
@@calc_hash:
	rol		edx, 3
	xor		dl, byte ptr[edi]
	inc   edi
	cmp   byte ptr[edi], 0
	jnz   @@calc_hash

  //
  cmp   [esp+(4+pushad_eax)], edx
  jnz   @@loop_func

    //  Needed function found
    mov		edx, [esp+(4+pushad_edx)]
    or    edx, edx
    jz    @@get_ord

      mov		eax, [esi-4]
			add		eax, ebp
    	push	eax
    	push	ebp
    	call	edx                 //  GetProcAddress
      jmp   @@jmp_end

@@get_ord:
      movzx eax, word ptr[ebx]  //  Name ordinal
			shl   eax, 2              //  Multiply by 4
			add   eax, [esp]
			mov   eax, [eax+ebp]
			add		eax, ebp

@@jmp_end:
    jmp   @@finished

  //
@@loop_func:
	inc		ebx
	inc		ebx
	loop  @@next_func

@@finished:
	pop   ecx
	mov		[esp+pushad_eax], eax
	popad
end;
{$endif}

//  ============================================================================
//  aPlib 1.1.1
//  aP_depack_asm(const void *source, void *destination)
function aP_depack_asm(const source: Pointer; destination: Pointer): LongWord; stdcall;
{$ifdef WIN64}
asm
  push    rbx
  push    rsi
  push    rdi

  push    rdx

  //mov    rsi, rcx
  //mov    rdi, rdx
  mov     rsi, [source]
  mov     rdi, [destination]

  cld
  mov     dl, 80h
  xor     ebx, ebx

@literal:
  movsb
  mov     bl, 2
@nexttag:
  call    @getbit
  jnc     @literal

  xor     ecx, ecx
  call    @getbit
  jnc     @codepair
  xor     eax, eax
  call    @getbit
  jnc     @shortmatch
  mov     bl, 2
  inc     ecx
  mov     al, 10h

@@getmorebits:
  call    @getbit
  adc     al, al
  jnc     @@getmorebits
  jnz     @domatch
  stosb
  jmp     @nexttag

@codepair:
  call    @getgamma_no_ecx
  sub     ecx, ebx
  jnz     @normalcodepair
  call    @getgamma
  jmp     @domatch_lastpos

@shortmatch:
  lodsb
  shr     eax, 1
  jz      @donedepacking
  adc     ecx, ecx
  jmp     @domatch_with_2inc

@normalcodepair:
  xchg    eax, ecx
  dec     eax
  shl     eax, 8
  lodsb
  call    @getgamma

  cmp     eax, 32000
  jae     @domatch_with_2inc
  cmp     ah, 5
  jae     @domatch_with_inc
  cmp     eax, 7fh
  ja      @domatch_new_lastpos

@domatch_with_2inc:
  inc     ecx

@domatch_with_inc:
  inc     ecx

@domatch_new_lastpos:
  xchg    eax, r8d
@domatch_lastpos:
  mov     eax, r8d

  mov     bl, 1

@domatch:
  push    rsi
  mov     rsi, rdi
  sub     rsi, rax
  rep     movsb
  pop     rsi
  jmp     @nexttag

@getbit:
  add     dl, dl
  jnz     @@stillbitsleft
  mov     dl, [rsi]
  inc     rsi
  adc     dl, dl
@@stillbitsleft:
  ret

@getgamma:
  xor     ecx, ecx
@getgamma_no_ecx:
  inc     ecx
@@getgammaloop:
  call    @getbit
  adc     ecx, ecx
  call    @getbit
  jc      @@getgammaloop
  ret

@donedepacking:
  pop     rdx
  sub     rdi, rdx
  xchg    eax, edi

  pop     rdi
  pop     rsi
  pop     rbx
end;
{$else}
const
  _ret  = 7*4;
  _src  = 8*4 + 4;
  _dst  = 8*4 + 8;
asm
  pushad

  mov     esi, [source]
  mov     edi, [destination]
  //mov     esi, [esp + _src] //  C calling convention
  //mov     edi, [esp + _dst]

  cld
  mov     dl, 80h
  xor     ebx,ebx

@literal:
  movsb
  mov     bl, 2
@nexttag:
  call    @getbit
  jnc     @literal

  xor     ecx, ecx
  call    @getbit
  jnc     @codepair
  xor     eax, eax
  call    @getbit
  jnc     @shortmatch
  mov     bl, 2
  inc     ecx
  mov     al, 10h

@@getmorebits:
  call    @getbit
  adc     al, al
  jnc     @@getmorebits
  jnz     @domatch
  stosb
  jmp     @nexttag

@codepair:
  call    @getgamma_no_ecx
  sub     ecx, ebx
  jnz     @normalcodepair
  call    @getgamma
  jmp     @domatch_lastpos

@shortmatch:
  lodsb
  shr     eax, 1
  jz      @donedepacking
  adc     ecx, ecx
  jmp     @domatch_with_2inc

@normalcodepair:
  xchg    eax, ecx
  dec     eax
  shl     eax, 8
  lodsb
  call    @getgamma

  cmp     eax, 32000
  jae     @domatch_with_2inc
  cmp     ah, 5
  jae     @domatch_with_inc
  cmp     eax, 7fh
  ja      @domatch_new_lastpos

@domatch_with_2inc:
  inc     ecx

@domatch_with_inc:
  inc     ecx

@domatch_new_lastpos:
  xchg    eax, ebp
@domatch_lastpos:
  mov     eax, ebp

  mov     bl, 1

@domatch:
  push    esi
  mov     esi, edi
  sub     esi, eax
  rep     movsb
  pop     esi
  jmp     @nexttag

@getbit:
  add     dl, dl
  jnz     @@stillbitsleft
  mov     dl, [esi]
  inc     esi
  adc     dl, dl
@@stillbitsleft:
  ret

@getgamma:
  xor     ecx, ecx
@getgamma_no_ecx:
  inc     ecx
@@getgammaloop:
  call    @getbit
  adc     ecx, ecx
  call    @getbit
  jc      @@getgammaloop
  ret

@donedepacking:
  //sub     edi, [esp + _dst]
  //mov     [esp + _ret], edi  //  return unpacked length in eax
  sub     edi, [destination]
  mov     [Result], edi

  popad
end;
{$endif}





{$endif}