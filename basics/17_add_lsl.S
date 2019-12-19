		.data
fmt:	.asciz	"r1 = 0x%x\n"

		.text
		.globl main

print:		stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr

main:		stmfd sp!, {lr}
		mov r3, #7
		mov r2, #3
		add r1, r2, r3, lsl #7
		bl print
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
