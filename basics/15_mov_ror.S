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
		mov r1, #0xff, 8
		bl print
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
