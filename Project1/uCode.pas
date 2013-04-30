MOV SP D0

ADD SP #3 SP	; space for 3 variables of type MP_INTEGER_LIT
PUSH #55		; Integer litaral
POP 0(D0)
PUSH #65		; Integer litaral
POP 1(D0)
PUSH #10		; Integer litaral
POP 2(D0)
PUSH 1(D0)	; Identifier b
PUSH 2(D0)	; Identifier c
CMPGTS
BRFS L1		; branch if false
