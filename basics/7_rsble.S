		.data
fmt:	.asciz	"r1 = %d\n"

		.text
		.globl main

main:		stmfd sp!, {lr}
		mov r4, #-2
		mov r3, #77
		mov r2, #37
		cmp r2, r3
		bgt print
		rsble r1, r4, #5
print:		ldr r0, =fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
