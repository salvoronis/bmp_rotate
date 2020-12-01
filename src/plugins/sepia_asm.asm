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

c_r: dd c11, c21, c31, c11;, c21, c31, c11, c21, c31, c11, c21, c31
c_g: dd c12, c22, c32, c12;, c22, c32, c12, c22, c32, c12, c22, c32
c_b: dd c13, c23, c33, c13;, c23, c33, c13, c23, c33, c13, c23, c33

global sepia_sse

section .text
;rdi = r
;rsi = g
;rdx = b
;rsx = res
sepia_sse:
	lea r9, [rel c_r]
	lea r10, [rel c_g]
	lea r11, [rel c_b]

	movdqa xmm0, [rdi] ;r1 r2 r3 r4
	movdqa xmm1, [rsi] ;g1 g2 g3 g4
	movdqa xmm2, [rdx] ;b1 b2 b3 b4

	mulps xmm0, [r9] ; mul red
	mulps xmm1, [r10] ; mul green
	mulps xmm2, [r11] ; mul blue

	addps xmm0, xmm1
	addps xmm0, xmm2
	movdqa [rcx], xmm0

	ret
