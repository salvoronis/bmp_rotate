global sepia_sse

section .text

sepia_sse:
	movdqa xmm0, [rdi]
	mulps xmm0, [rsi]
	addps xmm0, [rsi]
	movdqa [rdi], xmm0
	ret
