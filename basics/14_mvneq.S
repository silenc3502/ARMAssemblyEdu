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
		mov r3, #0
		mov r2, #0
		cmp r3, r2
		mvneq r1, #0
		bl print
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
