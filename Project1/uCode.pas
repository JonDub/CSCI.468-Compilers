MOV SP D0

ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
PUSH #10		; Integer litaral
PUSH #2		; Integer litaral
PUSH -2(SP)	; Casting from int to float
CASTSF
POP -2(SP)	; casting done
PUSH -1(SP)	; Casting from int to float
CASTSF
POP -1(SP)	; casting done
DIVSF
CASTSI			; result does not match assignemnt variable type cast to integer
POP 0(D0)

MOV D0 SP
HLT
