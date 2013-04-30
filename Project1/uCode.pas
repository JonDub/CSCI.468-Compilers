MOV SP D0

ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
RD D9			; Read Var g
MOV D9 6(D0)
RD D9		; Read Var h
MOV D9 7(D0)
PUSH #3		; Integer litaral
NEG -1(SP) -1(SP)		; optional sign negative
PUSH #5		; Integer litaral
PUSH #3		; Integer litaral
PUSH #6		; Integer litaral
SUBS
PUSH #0		; Integer litaral
SUBS
MULS
PUSH #21		; Integer litaral
MULS
ADDS
PUSH #15		; Integer litaral
SUBS
POP 0(D0)
PUSH #6		; Integer litaral
PUSH #3		; Integer litaral
DIVS
PUSH #2		; Integer litaral
DIVS
PUSH #1		; Integer litaral
DIVS
PUSH #1		; Integer litaral
ADDS
POP 2(D0)
PUSH #10		; Integer litaral
PUSH #11		; Integer litaral
MODS
PUSH #12		; Integer litaral
MODS
PUSH #13		; Integer litaral
MODS
PUSH #14		; Integer litaral
MODS
PUSH #15		; Integer litaral
MODS
PUSH #16		; Integer litaral
MODS
POP 4(D0)
PUSH #6		; Integer litaral
NEG -1(SP) -1(SP)		; optional sign negative
POP 5(D0)
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH 2(D0)	; Identifier c
WRTS
WRTLN #""
PUSH 4(D0)	; Identifier e
WRTS
WRTLN #""
PUSH 5(D0)	; Identifier f
WRTS
WRTLN #""
PUSH 6(D0)	; Identifier g
PUSH 6(D0)	; Identifier g
MULS
PUSH 6(D0)	; Identifier g
MULS
PUSH 6(D0)	; Identifier g
MULS
PUSH 6(D0)	; Identifier g
MULS
PUSH 6(D0)	; Identifier g
MULS
PUSH 7(D0)	; Identifier h
MULS
PUSH 7(D0)	; Identifier h
MULS
PUSH 7(D0)	; Identifier h
MULS
PUSH 7(D0)	; Identifier h
MULS
PUSH 7(D0)	; Identifier h
MULS
PUSH #2		; Integer litaral
MULS
WRTS
PUSH 6(D0)	; Identifier g
WRTS
WRTLN #""

MOV D0 SP
HLT
