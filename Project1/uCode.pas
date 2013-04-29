MOV SP D0

ADD SP #3 SP
ADD SP #3 SP
ADD SP #3 SP
ADD SP #1 SP
PUSH #5
NEG -1(S)P -1(SP) ; optional sign negative
CASTI			; result does not match assignemnt variable type cast to integer
POP 0(D0)
PUSH #87
NEG -1(S)P -1(SP) ; optional sign negative
CASTI			; result does not match assignemnt variable type cast to integer
POP 2(D0)
PUSH 0(D0)	; a
PUSH 2(D0)	; c
ADDS
CASTI			; result does not match assignemnt variable type cast to integer
POP 1(D0)
PUSH 0(D0)	; a
PUSH 1(D0)	; b
CASTF			;righthandside result is an int left hand side is an float so cast
CMPLTSF
BRFS L1		; branch if false
WRT #"A is less than B\n"
PUSH #1			; boolean true
POP 8(D0)			; boolean assign

L1:		; if false
WRT #"A is greater than or equal to B\n"
PUSH #0			; boolean false
POP 8(D0)			; boolean assign
PUSH 8(D0)	; lessThan
BRFS L2		; branch if false
WRT #"a: "
WRT 0(D0)
WRT #"; < b: "
WRT 1(D0)
WRT #"\n"

L2:		; if false
PUSH 0(D0)	; a
PUSH #10

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
