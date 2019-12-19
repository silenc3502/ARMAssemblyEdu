			.data
fmt:		.asciz	"float = %f\n"

			.text
			.globl main

main:		push {fp, lr}
			add fp, sp, #4
			sub sp, sp, #32
consta1:	.float 0.1
			.float 0.2
			vldr.32 s0, consta1
			vstr.32 s0, [fp, #-12]
			vldr.32 s1, consta1 + 4
			vstr.32 s0, [fp, #-16]
			vadd.f32 s2, s0, s1
			vstr.32 s2, [fp, #-20]
			vcvt.f64.f32 d16, s2
			vmov r2, r3, d16
			ldr r0, =fmt
			bl printf
fin:		sub sp, fp, #4
			pop {fp, pc}
