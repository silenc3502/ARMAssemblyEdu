		.data
fmt1:	.asciz	"r2r1 = 0x%x"
fmt2:	.asciz	"r2r1 = 0x%x\n"
fmt3:	.asciz	"%x"

		.text
		.globl main

print64:	push {fp, lr}
		add fp, sp, #4
		sub sp, sp, #16
		str r1, [fp, #-16]
		str r2, [fp, #-20]
		cmp r1, #0
		bne L1
		ldr r0, =fmt2
		bl printf
		b fin
L1:		cmn r1, #-268435455
		bhi fin	
		mov r1, r2
		ldr r0, =fmt1
		bl printf
		mov r3, #15
		mov r4, #28
loop:		ldr r1, [fp, #-16]
		lsr r1, r1, r4
		ldr r0, =fmt3
		bl printf
		sub r4, r4, #4
		cmn r4, #4
		bne loop
		mov r0, #10
		bl putchar
fin:		nop
		sub sp, fp, #4
		pop {fp, pc}

main:		stmfd sp!, {lr}
		mov r1, #0xf
		mov r2, #0x1
		mov r3, #0x44, 8
		mov r4, #0x200
		umlal r1, r2, r3, r4
		bl print64
		mov r0, #0
		ldmfd sp!, {lr}
		mov pc, lr
