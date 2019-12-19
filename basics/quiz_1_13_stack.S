		.data
fmt:	.asciz	"r1 = %d\n"

		.text
		.globl main

print:	stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

#int find(int *arr, int num)	<< 23
find:	stmfd sp!, {lr}
		mov r2, r0
		mov r3, #2
loop:	cmp r2, r3
		ble fin
		sub r6, r3, #1
		lsl r6, #2
		sub r6, r4, r6
		ldr r5, [r1, r6]
		#ldr r5, [r5]
		sub r6, r3, #2
		lsl r6, #2
		sub r6, r4, r6
		ldr r7, [r1, r6]
		#ldr r7, [r7]
		add r0, r5, r7
		mov r6, r3
		lsl r6, #2
		sub r6, r4, r6
		str r0, [r1, r6]
		add r3, r3, #1
		b loop
fin:	ldmfd sp!, {lr}
		mov pc, lr

main:	stmfd sp!, {lr}
		mov r2, #24
		lsl r2, #2
		sub sp, sp, r2
		sub r2, r2, #4
		mov r4, r2
		mov r3, #1
		str r3, [sp, r2]
		mov r3, #3
		sub r2, r2, #4
		str r3, [sp, r2]
		mov r1, sp
		mov r0, #23
		bl find
		mov r1, r0
		bl print
		add sp, sp, #96
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
