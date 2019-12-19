		.comm	arr, 28, 4
		.section	.rodata

fmt:		.asciz	"arr[%d] = %d\n"
fmt2:		.asciz	"r4 = 0x%x, r5 = 0x%x, r6 = 0x%x\n"
parent:		.asciz	"r0 = %u, parent\n"
child:		.asciz	"r0 = %u, child\n"

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
		cmp r1, #5
		ble loop
		nop
		sub sp, fp, #4
		pop {fp, pc}

main:		push {r7, lr}
		mov r7, #2
		swi #0
		mov r1, r0
if:		subs r2, r0, #0
		ldrne r0, =parent
		ldreq r0, =child
		bl printf
		mov r0, #0
		pop {r7, pc}
