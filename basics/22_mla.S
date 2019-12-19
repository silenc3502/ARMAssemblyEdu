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
		mov r4, #33
		mov r3, #7
		mov r2, #3
		mla r1, r2, r3, r4
		bl print
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
