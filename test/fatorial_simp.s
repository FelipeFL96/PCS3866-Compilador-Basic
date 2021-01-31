/* BASIC COMPILER */
/* source: test/fatorial_simp.bas */
.global main

main: 
	LDR      r12, =variables
	LDR      r11, =exe_stack
	LDR      sp,  =exp_stack
	B        L0

L0:
	MOV      r1, #5
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	STR      r0, [r12, #4]
	B        L1

L1:
	MOV      r1, #1
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	STR      r0, [r12, #8]
	B        L3

L4:
	LDR      r1, [r12, #8]
	STMFD    sp!, {r1}
	LDR      r1, [r12, #12]
	STMFD    sp!, {r1}
	LDMFD    sp!, {r2}
	LDMFD    sp!, {r1}
	MUL      r0, r1, r2
	STMFD    sp!, {r0}
	LDMFD    sp!, {r0}
	STR      r0, [r12, #8]
	B        L5

L3:
	MOV      r1, #1
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	STR      r0, [r12, #12]
	B        L3.COMP

L3.STEP:
	MOV      r1, #1
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	LDR      r1, [r12, #12]
	ADD      r0, r1, r0
	STR      r0, [r12, #12]

L3.COMP:
	LDR      r1, [r12, #4]
	STMFD    sp!, {r1}
	MOV      r1, #1
	STMFD    sp!, {r1}
	LDMFD    sp!, {r2}
	LDMFD    sp!, {r1}
	ADD      r0, r1, r2
	STMFD    sp!, {r0}
	LDMFD    sp!, {r0}
	LDR      r1, [r12, #12]
	CMP      r1, r0
	BGE      L6
	B        L4

L5:
	B        L3.STEP

L6:
	LDR      r1, [r12, #8]
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	STR      r0, [r12, #16]
	B        L7

L7:
	B        L7

variables: 
	.space 20

	.space 256
exe_stack: 
	.space 256
exp_stack: 

