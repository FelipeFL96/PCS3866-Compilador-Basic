/* BASIC COMPILER */
/* source: test/acumula_dobro.bas */
.global main

main: 
	LDR      r12, =variables
	LDR      r11, =exe_stack
	LDR      sp,  =exp_stack
	B        L3

dobro:
	LDMFD    sp!, {r1}
	STR      r1, [r12, #4]
	MOV      r1, #2
	STMFD    sp!, {r1}
	LDR      r1, [r12, #4]
	STMFD    sp!, {r1}
	LDMFD    sp!, {r2}
	LDMFD    sp!, {r1}
	MUL      r0, r1, r2
	STMFD    sp!, {r0}
	LDMFD    sp!, {r0}
	MOV      pc, lr

L3:
	MOV      r1, #0
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	MOV      r1, r0, LSL #2
	ADD      r1, r1, #8
	MOV      r0, #7
	STR      r0, [r12, r1]
	MOV      r1, #1
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	MOV      r1, r0, LSL #2
	ADD      r1, r1, #8
	MOV      r0, #1
	STR      r0, [r12, r1]
	MOV      r1, #2
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	MOV      r1, r0, LSL #2
	ADD      r1, r1, #8
	MOV      r0, #5
	STR      r0, [r12, r1]
	MOV      r1, #3
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	MOV      r1, r0, LSL #2
	ADD      r1, r1, #8
	MOV      r0, #4
	STR      r0, [r12, r1]
	B        L4

L4:
	MOV      r1, #0
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	STR      r0, [r12, #24]
	B        L5

L5:
	MOV      r1, #0
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	STR      r0, [r12, #28]
	B        L6

L6:
	LDR      r1, [r12, #24]
	STMFD    sp!, {r1}
	LDR      r1, [r12, #28]
	STMFD    sp!, {r1}
	LDMFD    sp!, {r1}
	MOV      r1, r1, LSL #2
	ADD      r1, r1, #8
	LDR      r1, [r12, r1]
	STMFD    sp!, {r1}
	STMFD    r11!, {lr}
	BL       dobro
	LDMFD    r11!, {lr}
	STMFD    sp!, {r0}
	LDMFD    sp!, {r2}
	LDMFD    sp!, {r1}
	ADD      r0, r1, r2
	STMFD    sp!, {r0}
	LDMFD    sp!, {r0}
	STR      r0, [r12, #24]
	B        L7

L7:
	LDR      r1, [r12, #28]
	STMFD    sp!, {r1}
	MOV      r1, #1
	STMFD    sp!, {r1}
	LDMFD    sp!, {r2}
	LDMFD    sp!, {r1}
	ADD      r0, r1, r2
	STMFD    sp!, {r0}
	LDMFD    sp!, {r0}
	STR      r0, [r12, #28]
	B        L8

L8:
	LDR      r1, [r12, #28]
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	STMFD    sp!, {r0}
	MOV      r1, #4
	STMFD    sp!, {r1}
	LDMFD    sp!, {r0}
	LDMFD    sp!, {r1}
	CMP      r1, r0
	BLT      L6
	B        L9

L9:
	B        L9

variables: 
	.space 32

	.space 256
exe_stack: 
	.space 256
exp_stack: 

