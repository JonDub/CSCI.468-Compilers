MOV SP D0

ADD SP #13 SP	; space for 13 variables of type MP_INTEGER_LIT
RD D9			; Read Var a
MOV D9 0(D0)
RD D9		; Read Var b
MOV D9 1(D0)
RD D9		; Read Var c
MOV D9 2(D0)
RD D9		; Read Var d
MOV D9 3(D0)
PUSH #4444444		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L1:
PUSH 0(D0)	; Identifier a
PUSH #3		; Integer litaral
CMPLTS
PUSH #2		; Integer litaral
PUSH 3(D0)	; Identifier d
CMPLTS
ANDS
PUSH 1(D0)	; Identifier b
PUSH #10	; Integer litaral
CMPGTS
ORS

BRFS L2		; while false
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""

BR L1		; while true

L2:		; while false

MOV D0 SP
HLT
