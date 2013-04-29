MOV SP D0

ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
RD D9			; Read Var Apples
MOV D9 0(D0)
PUSH 0(D0)	; Identifier Apples
PUSH #0		; Integer litaral
CMPGTS
BRFS L1		; branch if false
PUSH 0(D0)	; Identifier Apples
PUSH #5		; Integer litaral
MULS
POP 1(D0)
WRT 1(D0)

L1:		; if false
PUSH 0(D0)	; Identifier Apples
PUSH #7		; Integer litaral
MULS
POP 1(D0)
WRT 1(D0)

MOV D0 SP
HLT
