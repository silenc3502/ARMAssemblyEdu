		.data

arr:		.asciz	"HelloARM"

fmt:		.asciz	"r1 = %c\n"

		.text
		.globl main

print:		stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr

main:		stmfd sp!, {lr}
		ldr r2, =arr
		ldrb r1, [r2, #5]
		bl print
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
