		.data
fmt:	.asciz	"r1 = %u, r2 = 0x%x\n"

		.text
		.globl main

main:		push {fp, lr}
		mov r2, #0x8000000
		clz r1, r2
		ldr r0, =fmt
		bl printf
		mov r0, #0
		pop {fp, pc}
