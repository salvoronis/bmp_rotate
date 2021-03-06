extern _GLOBAL_OFFSET_TABLE_

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

align 16

first: dd 	c31, c21, c11, c31,	c21, c11, c31, c21,	c11, c31, c21, c11
second: dd 	c32, c22, c12, c32,	c22, c12, c32, c22,	c12, c32, c22, c12
third: dd 	c33, c23, c13, c33,	c23, c13, c33, c23,	c13, c33, c23, c13

section .data

byte_to_float: dd 0.0 , 1.0 , 2.0 , 3.0 , 4.0 , 5.0 , 6.0 , 7.0 , 8.0 , 9.0 , 10.0 , 11.0 , 12.0 , 13.0 , 14.0 , 15.0 ,16.0 , 17.0 , 18.0 , 19.0 , 20.0 , 21.0 , 2.0 , 23.0 ,24.0 , 25.0 , 26.0 , 27.0 , 28.0 , 29.0 , 30.0 , 31.0 ,32.0 , 33.0 , 34.0 , 35.0 , 36.0 , 37.0 , 38.0 , 39.0 ,40.0 , 41.0 , 42.0 , 43.0 , 44.0 , 45.0 , 46.0 , 47.0 ,48.0 , 49.0 , 50.0 , 51.0 , 52.0 , 53.0 , 54.0 , 55.0 ,56.0 , 57.0 , 58.0 , 59.0 , 60.0 , 61.0 , 62.0 , 63.0 ,64.0 , 65.0 , 66.0 , 67.0 , 68.0 , 69.0 , 70.0 , 71.0 ,72.0 , 73.0 , 74.0 , 75.0 , 76.0 , 77.0 , 78.0 , 79.0 ,80.0 , 81.0 , 82.0 , 83.0 , 84.0 , 85.0 , 86.0 , 87.0 ,88.0 , 89.0 , 90.0 , 91.0 , 92.0 , 93.0 , 94.0 , 95.0 ,96.0 , 97.0 , 98.0 , 99.0 , 100.0 , 101.0 , 102.0 , 103.0 ,104.0 , 105.0 , 106.0 , 107.0 , 108.0 , 109.0 , 110.0 , 111.0 ,112.0 , 113.0 , 114.0 , 115.0 , 116.0 , 117.0 , 118.0 , 119.0 ,120.0 , 121.0 , 122.0 , 123.0 , 124.0 , 125.0 , 126.0 , 127.0 ,128.0 , 129.0 , 130.0 , 131.0 , 132.0 , 133.0 , 134.0 , 135.0 ,136.0 , 137.0 , 138.0 , 139.0 , 140.0 , 141.0 , 142.0 , 143.0 ,144.0 , 145.0 , 146.0 , 147.0 , 148.0 , 149.0 , 150.0 , 151.0 ,152.0 , 153.0 , 154.0 , 155.0 , 156.0 , 157.0 , 158.0 , 159.0 ,160.0 , 161.0 , 162.0 , 163.0 , 164.0 , 165.0 , 166.0 , 167.0 ,168.0 , 169.0 , 170.0 , 171.0 , 172.0 , 173.0 , 174.0 , 175.0 ,176.0 , 177.0 , 178.0 , 179.0 , 180.0 , 181.0 , 182.0 , 183.0 ,184.0 , 185.0 , 186.0 , 187.0 , 188.0 , 189.0 , 190.0 , 191.0 ,192.0 , 193.0 , 194.0 , 195.0 , 196.0 , 197.0 , 198.0, 199.0,200.0, 201.0, 202.0, 203.0, 204.0, 205.0, 206.0, 207.0,208.0, 209.0, 210.0, 211.0, 212.0, 213.0, 214.0, 215.0,216.0, 217.0, 218.0, 219.0, 220.0, 221.0, 222.0, 223.0,224.0, 225.0, 226.0, 227.0, 228.0, 229.0, 230.0, 231.0,232.0, 233.0, 234.0, 235.0, 236.0, 237.0, 238.0, 239.0,240.0, 241.0, 242.0, 243.0, 244.0, 245.0, 246.0, 247.0,248.0, 249.0, 250.0, 251.0, 252.0, 253.0, 254.0, 255.0

align 16
rows: resb 48 ; 0b1 4b2 8b3 12b4 16g1 20g2 24g3 28g4 32r1 36r2 40r3 44r4
buffer: resb 16

global sepia_sse

section .text
;rdi = pixels
;rsi = size/4
sepia_sse:
	.start:
		lea r10, [rel rows] ; contains buffer address
		mov r11, 0 ; counter of colors of pixels
		mov rcx, 0 ; buffer offset
		lea r9, [rel byte_to_float] ; translate uint8_t to float table address
		mov r8, 4
	push rdi
	.chunk_fill:
		xor rax, rax

		mov al, [rdi] ; take a pixel's color
		mul r8 ; mulpiplie color to be equal to 4 bytes
		add rax, r9 ; search float in the table

		mov edx, [rax] ; cathc float color and write it into edx
		mov [r10 + rcx], edx ; write color into the buffer

		inc rdi ; move on
		cmp rcx, 32	; we move buffer's address by offset, first of all we want to have a "bbbbggggrrrr" structure,
		jae .erase	; but pixel is bgr, so we move address by 16 and from 0 to 16 move inside the color address space
		add rcx, 16
		jmp .next

		.erase:
		sub rcx, 32
		add rcx, 4

		.next:
		inc r11
		cmp r11, 12 ; 12 colors or 4 pixels fill the buffer
		jne .chunk_fill
	pop rdi

	xor r9, r9 ; mask counter
	.handler:

		call sepia_creator ; sepia filter
		add rdi, 4 ; move to another 4 bytes
		add r9, 16
		cmp r9, 48
		jne .handler

	dec rsi ; rsi is a number of iterations
	cmp rsi, 0
	je .end

	jmp .start

	.end:
		ret

;rdi - pixels point
;r9 - mask pointer
sepia_creator:

	lea r8, [rel first]
	movdqa xmm0, [r8 + r9] ; loat coefs

	lea r8, [rel second]
	movdqa xmm1, [r8 + r9]

	lea r8, [rel third]
	movdqa xmm2, [r8 + r9]

	lea r11, [rel rows]
	movdqa xmm3, [r11 + 32] ; load colors r1r2r3r4
	movdqa xmm4, [r11 + 16] ; load g1g2g3g4
	movdqa xmm5, [r11] ; b1b2b3b4
.loader:

	cmp r9, 0
	je .first_stage
	cmp r9, 16
	je .second_stage
	cmp r9, 32
	je .third_stage

	.first_stage:
		shufps xmm3, xmm3, 01000000b ;r1 r1 r1 r2
		shufps xmm4, xmm4, 01000000b ;g1 g1 g1 g2
		shufps xmm5, xmm5, 01000000b ;b1 b1 b1 b2
		jmp .conclusion
	.second_stage:
		shufps xmm3, xmm3, 10100101b ;r2 r2 r3 r3
		shufps xmm4, xmm4, 10100101b ;g2 g2 g3 g3
		shufps xmm5, xmm5, 10100101b ;b2 b2 b3 b3
		jmp .conclusion
	.third_stage:
		shufps xmm3, xmm3, 11111110b ;r3 r4 r4 r4
		shufps xmm4, xmm4, 11111110b ;g3 g4 g4 g4
		shufps xmm5, xmm5, 11111110b ;b3 b4 b4 b4
	.conclusion:
	mulps xmm3, xmm0 ; mul with first coeficients
	mulps xmm4, xmm1 ; mul second
	mulps xmm5, xmm2 ; mul third

	addps xmm3, xmm4
	addps xmm3, xmm5 ; xmm3 -> float B1 G1 R1 B2
	;packuswb xmm3, xmm3
	cvtps2dq xmm3, xmm3 ; xmm -> uint32_t B1 G1 R1 B2

	packusdw xmm3, xmm3
	packuswb xmm3, xmm3

	lea r12, [rel buffer] ; buffer which contains uint32_t B1 G1 R1 B2
	movdqa [r12], xmm3

	xor rax, rax

	mov eax, [r12] ; eax -> uint32_t B1
	.fst:
	mov [rdi], al ; B1 -> pixels

	mov eax, [r12+1]
	.scnd:
	mov [rdi+1], al

	mov eax, [r12+2]
	.thrd:
	mov [rdi+2], al

	mov eax, [r12+3]
	.fth:
	mov [rdi+3], al
	
	ret
