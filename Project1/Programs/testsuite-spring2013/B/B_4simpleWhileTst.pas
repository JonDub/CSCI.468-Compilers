MOV SP D0

ADD SP #2 SP	; space for 2 variables of type MP_INTEGER_LIT
RD D9			; Read Var a
MOV D9 0(D0)
RD D9		; Read Var b
MOV D9 1(D0)
WRT 0(D0)
WRTLN #""

L1:
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
CMPGTS

BRFS L2		; while false
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
SUBS
POP 0(D0)
WRT 0(D0)
WRTLN #""

BR L1		; while true

L2:		; while false
PUSH #20		; Integer litaral
POP 0(D0)
WRT 0(D0)
WRTLN #""

L3:
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
CMPGES

BRFS L4		; while false
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
SUBS
POP 0(D0)
WRT 0(D0)
WRTLN #""

BR L3		; while true

L4:		; while false
PUSH #0		; Integer litaral
POP 0(D0)
WRT 0(D0)
WRTLN #""

L5:
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
CMPLTS

BRFS L6		; while false
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)
WRT 0(D0)
WRTLN #""

BR L5		; while true

L6:		; while false
PUSH #0		; Integer litaral
POP 0(D0)
WRT 0(D0)
WRTLN #""

L7:
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
CMPLES

BRFS L8		; while false
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)
WRT 0(D0)
WRTLN #""

BR L7		; while true

L8:		; while false

MOV D0 SP
HLT
