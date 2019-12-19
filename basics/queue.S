			.equ	link, 0
			.equ	data, 4
			.equ	NULL, 0

			.data
fmt:		.asciz	"r1(queue data) = %d\n"

			.text
			.globl main

get_node:	stmfd sp!, {lr}
			mov r0, #8
			bl malloc
			#str r0, [r4]
			mov r5, #0
			str r5, [r0]
			ldmfd sp!, {lr}
			mov pc, lr



enqueue:	push {fp, lr}
			#stmfd sp!, {lr}
			add fp, sp, #4
			sub sp, sp, #12
			#Need Stack to keep origin value
			#mov r4, r0
			#mov r8, r1
			str r0, [fp, #-8]
			str r1, [fp, #-12]
			ldr r4, [fp, #-8]
			ldr r8, [fp, #-12]
			ldr r6, [r4]
			cmp r6, #0
			bne recurs
			bl get_node
			str r0, [r4]
			str r8, [r0, #4]
			b fin
recurs:		mov r0, r6
			ldr r1, [fp, #-12]
			bl enqueue
fin:		sub sp, fp, #4
			pop {fp, pc}
			#ldmfd sp!, {lr}
			#mov pc, lr



pqueue:		push {fp, lr}
			add fp, sp, #4
			sub sp, sp, #8
			mov r4, r0
			str r0, [fp, #-8]
ploop:		ldr r5, [r4]
			cmp r5, #0
			beq pfin
			ldr r0, =fmt
			ldr r1, [r5, #4]
			bl printf
			mov r4, r5
			b ploop
pfin:		sub sp, fp, #4
			pop {fp, pc}



main:		push {fp, lr}
			#stmfd sp!, {lr}
			add fp, sp, #4
			sub sp, sp, #8
			mov r3, #0
			str r3, [fp, #-8]
			mov r1, #10
			sub r0, fp, #8
			bl enqueue
			mov r1, #20
			sub r0, fp, #8
			bl enqueue
			sub r0, fp, #8
			bl pqueue
			mov r0, #0
			sub sp, fp, #4
			pop {fp, pc}
			#ldmfd sp!, {lr}
			#mov pc, lr
