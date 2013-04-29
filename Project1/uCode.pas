MOV SP D0

ADD SP #3 SP	; space for 3 variables of type MP_INTEGER_LIT
ADD SP #3 SP	; space for 3 variables of type MP_FLOAT_LIT
ADD SP #3 SP	; space for 3 variables of type MP_BOOLEAN
ADD SP #1 SP	; space for 1 variables of type MP_STRING
PUSH #5		; Integer litaral
NEG -1(SP) -1(SP)		; optional sign negative
CASTSI			; result does not match assignemnt variable type cast to integer
POP 0(D0)
PUSH #87		; Integer litaral
NEG -1(SP) -1(SP)		; optional sign negative
CASTSI			; result does not match assignemnt variable type cast to integer
POP 2(D0)
PUSH 0(D0)	; Identifier a
PUSH 2(D0)	; Identifier c
ADDS
CASTSI			; result does not match assignemnt variable type cast to integer
POP 1(D0)
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
CASTSF			; righthandside result is an int left hand side is a float so cast
CMPLTSF
BRFS L1		; branch if false
WRT #"A is less than B\n"
PUSH #1			; boolean true
POP 8(D0)			; boolean assign

L1:		; if false
WRT #"A is greater than or equal to B\n"
PUSH #0			; boolean false
POP 8(D0)			; boolean assign
PUSH 8(D0)	; Identifier lessThan
BRFS L2		; branch if false
WRT #"a: "
WRT 0(D0)
WRT #"; < b: "
WRT 1(D0)
WRT #"\n"

L2:		; if false
PUSH 0(D0)	; Identifier a
PUSH #10		; Integer litaral

L3:
BGT -2(SP) -1(SP) L4	; test condition
WRT 0(D0)
WRTLN #""
PUSH #1			; increment control variable
PUSH -2(SP)
ADDS
POP -2(SP)		; increment done
BR L3		; branch to loop start

L4:		; loop exit

MOV D0 SP
HLT
