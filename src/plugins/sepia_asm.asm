%define c11 0.393
%define c12 0.769
%define c13 0.189
%define c21 0.349
%define c22 0.686
%define c23 0.168
%define c31 0.272
%define c32 0.543
%define c33 0.131

section .rodata

first: dd 	c11, c21, c31, c11,	c22, c32, c12, c22,	c33, c13, c23, c33
second: dd 	c12, c22, c32, c12,	c23, c33, c13, c23,	c31, c11, c21, c31
third: dd 	c13, c23, c33, c13,	c21, c31, c11, c21,	c32, c12, c22, c32

masks: dd 0x1, 0x5, 0x15

global sepia_sse

section .text
;rdi = pixels
;rsi = count
sepia_sse:
	mov rsi, 15

	.base :
	sub rsi, 1
	test rsi, rsi
	je .end
	lea r9, [rel masks] ; mask pointer
	xor r10, r10 ;matrix pointer
	xor r11, r11 ;times
	;mov rax, 10

	.loop:
		lea r8, [rel first]
		add r8, r10
		movdqa xmm0, [r8]
	
		lea r8, [rel second]
		add r8, r10
		movdqa xmm1, [r8]

		lea r8, [rel third]
		add r8, r10
		movdqa xmm2, [r8]

		cvtdq2ps xmm3, [rdi] ;r1 g1 b1 r2
		add rdi, 2
		cvtdq2ps xmm4, [rdi] ;g1 b1 r2 g2
		add rdi, 2
		cvtdq2ps xmm5, [rdi] ;b1 r2 g2 b2

		;add r9, [rel masks]
		shufps xmm3, xmm3, 0x1;[r9] ;r1 r1 r1 r2
		shufps xmm4, xmm4, 0x1;[r9] ;g1 g1 g1 g2
		shufps xmm5, xmm5, 0x1;[r9] ;b1 b1 b1 b2

		mulps xmm3, xmm0 ; mul red
		mulps xmm4, xmm1 ; mul green
		mulps xmm5, xmm2 ; mul blue

		addps xmm3, xmm4
		addps xmm3, xmm5
		cvtps2dq xmm3, xmm3
		movdqa [rdi], xmm3

		cmp r11, 2
		je .base
	
	add rdi, 4
	inc r11
	add r10, 16
	add r9, 4
	jmp .loop

	.end:
		ret
