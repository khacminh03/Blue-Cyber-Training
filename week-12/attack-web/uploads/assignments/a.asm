.386
.model flat, stdcall
option casemap:none


.data
	

.code

start:
	call main_code
strcmp:
	push ebp
	mov ebp, esp
	push edx
	push ebx
	push esi
	push edi
	
	xor eax, eax
	mov esi, [esp + 18h]
	mov edi, [esp + 1ch]
	strcmp_loop:
		mov al, BYTE PTR [esi]
		mov ah, BYTE PTR [edi]
		test al, al
		je strcmp_exit
		test ah, ah
		je strcmp_exit
		sub al, ah
		xor ah, ah
		inc esi
		inc edi
		test al, al
		je strcmp_loop
		
	strcmp_exit:
	pop edi
	pop esi
	pop ebx
	pop edx
	pop ebp
	ret
main_code:
	push ebp
	mov ebp, esp
	sub esp, 208h
	mov [esp + 204h], eax
	assume fs:nothing
	mov eax, fs:[30h]
	mov eax, [eax + 0ch]		
	mov eax, [eax + 14h]		
	mov eax, [eax] 
	mov eax, [eax]
	mov eax, [eax + 10h] ; kernel32 base address
	mov [esp + 20h], eax ; save kernel32 base
	
	mov ebx, [esp + 20h]
	xor edx, edx
	mov dl, [ebx + 3ch] ; IMAGE_DOS_HEADER->e_lfanew
	
	add ebx, 4 ; signature
	add ebx, edx ; start of coff header
	add ebx, 20 ; start of optional header
	xor edx, edx
	mov dx, [ebx]
	mov [esp + 1ch], edx ; save magic byte
	add ebx, 24
	mov edx, [esp + 1ch]
	cmp edx, 10bh
	jne _x64
	add ebx, 72
	jmp export_data_directory
	_x64:
	add ebx, 88
	export_data_directory:
	
	mov eax, [ebx] ; RVA of export directory
	mov edx, [esp + 20h]
	add edx, eax 
	
	mov [esp + 18h], edx ; export data directory address
	mov eax, [esp + 20h]
	mov ebx, [edx + 1ch] ; export address table RVA
	add ebx, eax
	mov [esp + 24h], ebx
	
	mov ebx, [edx + 24h] ; ordinal table
	add ebx, [esp + 20h]
	
	mov edx, [edx + 20h] ; export name pointer
	add edx, eax
	
	xor ecx, ecx
	get_function_address_loop:
		mov eax, [esp + 20h]
		add eax, [edx]
		push eax
		mov eax, 0
		mov [esp + 200h], eax
		mov eax, 41797261h
		mov [esp + 1fch], eax
		mov eax, 7262694ch
		mov [esp + 1f8h], eax
		mov eax, 64616f4ch
		mov [esp + 1f4h], eax
		lea eax, [esp + 1f4h]
		push eax
		call strcmp
		add esp, 8
		test eax, eax
		jne find_getprocaddress
		inc ecx
		xor eax, eax
		mov ax, WORD PTR [ebx]
		shl eax, 2
		add eax, [esp + 24h]
		mov eax, [eax]
		mov [esp + 14h], eax ; LoadLibraryA RVA
		jmp backtoloop
		
		find_getprocaddress:
		mov eax, [esp + 20h]
		add eax, [edx]
		push eax
		mov eax, 7373h
		mov [esp + 200h], eax
		mov eax, 65726464h
		mov [esp + 1fch], eax
		mov eax, 41636f72h
		mov [esp + 1f8h], eax
		mov eax, 50746547h
		mov [esp + 1f4h], eax
		lea eax, [esp + 1f4h]
		push eax
		call strcmp
		add esp, 8
		test eax, eax
		jne backtoloop
		inc ecx
		xor eax, eax
		mov ax, WORD PTR [ebx]
		shl eax, 2
		; add eax, [esp + 20h]
		add eax, [esp + 24h]
		mov eax, [eax]
		mov [esp + 10h], eax ; GetProcAddress RVA
	
		backtoloop:
		add edx, 4
		add ebx, 2
		cmp ecx, 2
		jne get_function_address_loop
	
	mov ebx, [esp + 14h]
	add ebx, [esp + 20h]
	mov eax, 6c6ch
	mov [esp + 200h], eax
	mov eax, 642e3233h
	mov [esp + 1fch], eax
	mov eax, 72657355h
	mov [esp + 1f8h], eax
	lea eax, [esp + 1f8h]
	push eax
	call ebx
	mov [esp + 8], eax ; HANDLE of User32.dll

	mov ebx, [esp + 10h]
	add ebx, [esp + 20h]
	mov eax, 41786fh
	mov [esp + 200h], eax
	mov eax, 42656761h
	mov [esp + 1fch], eax
	mov eax, 7373654dh
	mov [esp + 1f8h], eax
	lea eax, [esp + 1f8h]
	push eax
	mov eax, [esp + 8 + 4]
	push eax
	call ebx
	mov [esp + 4], eax ; messageboxA VA, not RVA
	
	mov ebx, [esp + 4]
	xor eax, eax
	push eax
	mov eax, 6e6f69h
	mov [esp + 200h], eax
	mov eax, 74706143h
	mov [esp + 1fch], eax
	lea eax, [esp + 1fch]
	push eax
	mov eax, 6464h
	mov [esp + 1f4h], eax
	mov eax, 64657463h
	mov [esp + 1f0h], eax
	mov eax, 656a6e49h
	mov [esp + 1ech], eax
	lea eax, [esp + 1ech]
	push eax
	xor eax, eax
	push eax
	call ebx
	
	

	
END start	