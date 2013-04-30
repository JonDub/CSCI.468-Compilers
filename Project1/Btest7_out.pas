MOV SP D0

ADD SP #1 SP	; space for 1 variables of type MP_FLOAT_LIT
PUSH #1.0	; Fixed literal
CASTSF			; result does not match assignemnt variable type cast to float
POP 0(D0)

MOV D0 SP
HLT
