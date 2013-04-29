MOV SP D0

ADD SP #4 SP	; space for 4 variables of type MP_INTEGER_LIT
RD D9			; Read Var b
MOV D9 1(D0)
RD D9		; Read Var c
MOV D9 2(D0)
RD D9		; Read Var d
MOV D9 3(D0)
PUSH #1		; Integer litaral
PUSH 1(D0)	; Identifier b

L1:
BLT -2(SP) -1(SP) L2	; test condition
WRT 0(D0)
WRTLN #""

L3:
PUSH 2(D0)	; Identifier c
PUSH 3(D0)	; Identifier d
CMPLTS

BRFS L4		; while false
WRT 2(D0)
WRTLN #""
PUSH 2(D0)	; Identifier c
PUSH #1		; Integer litaral
ADDS
POP 2(D0)

L5:			; repeat start
WRT 3(D0)
WRTLN #""
PUSH 3(D0)	; Identifier d
PUSH #1		; Integer litaral
SUBS
POP 3(D0)
PUSH 3(D0)	; Identifier d
PUSH #10		; Integer litaral
CMPLTS

BRTS L5		; repeat test fail

BR L3		; while true

L4:		; while false
PUSH #1			; decrement control variable
NEGS
PUSH -2(SP)
ADDS
POP -2(SP)		; decrement done
BR L1		; branch to loop start

L2:		; loop exit

MOV D0 SP
HLT
