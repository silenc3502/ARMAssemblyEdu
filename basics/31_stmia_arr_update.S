		.comm	arr, 24, 4
		.section	.rodata

fmt:		.asciz	"arr[%d] = %d\n"

		.text
		.globl main

print:		stmfd sp!, {lr}
		ldr r0, =fmt
		bl printf
		ldmfd sp!, {lr}
		mov pc, lr

print_for:	push {fp, lr}
		add fp, sp, #4
		sub sp, sp, #8
		mov r3, #0
		str r3, [fp, #-8]
		b loop_cond
loop:		ldr r2, =arr
		ldr r3, [fp, #-8]
		ldr r3, [r2, r3, lsl #2]
		mov r2, r3
		ldr r1, [fp, #-8]
		ldr r0, =fmt
		bl printf
		ldr r3, [fp, #-8]
		add r3, r3, #1
		str r3, [fp, #-8]
loop_cond:	ldr r1, [fp, #-8]
		cmp r1, #4
		ble loop
		nop
		sub sp, fp, #4
		pop {fp, pc}

main:		push {r4, r5, fp, lr}
		add fp, sp, #12
		ldr r5, =arr
		mov r1, #3
		mov r2, r1, lsl #2
		mov r4, #2
		add r3, r1, r2, lsl r4
		stmia r5!, {r1, r2, r3}
		str r4, [r5]
		bl print_for
		mov r0, #0
		pop {r4, r5, fp, pc}
