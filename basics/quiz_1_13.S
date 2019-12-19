		.data
arr:	.word	1
		.word	3
		.skip	21 * 4

fmt:	.asciz	"r1 = %d\n"

		.text
		.globl main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

#int find(int num)	<< 23
find:	stmfd sp!, {lr}
		mov r1, r0
		mov r2, #2
loop:	cmp r1, r2
		ble fin
		ldr r3, =arr
		sub r8, r2, #1
		lsl r8, #2
		ldr r4, [r3, r8]
		sub r5, r2, #2
		lsl r5, #2
		ldr r6, [r3, r5]
		add r0, r4, r6
		mov r7, r2
		lsl r7, #2
		str r0, [r3, r7]
		add r2, r2, #1
		b loop
fin:	ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		mov r0, #23
		bl find
		mov r1, r0
		bl print
		ldmfd sp!, {lr}
		mov pc, lr
