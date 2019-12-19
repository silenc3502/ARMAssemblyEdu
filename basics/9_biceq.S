		.data
fmt:	.asciz	"r1 = %d\n"

		.text
		.globl main

main:		stmfd sp!, {lr}
		mov r3, #7
		mov r2, #7
		mov r1, #46
		cmp r2, r3
		bne print
		biceq r1, r1, #7
print:		ldr r0, =fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
