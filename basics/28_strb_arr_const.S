		.data

arr:		.asciz	"HelloARM"

fmt:		.asciz	"arr = %s\n"

		.text
		.globl main

print:		stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr

main:		stmfd sp!, {lr}
		ldr r1, =arr
		mov r2, #0x61
		strb r2, [r1, #5]
		bl print
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
