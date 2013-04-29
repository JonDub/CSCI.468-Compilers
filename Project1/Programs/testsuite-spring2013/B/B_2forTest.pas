MOV SP D0

ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
ADD SP #1 SP	; space for 1 variables of type MP_INTEGER_LIT
RD D9			; Read Var N
MOV D9 1(D0)
PUSH #1		; Integer litaral
PUSH 1(D0)	; Identifier N

L1:
BGT -2(SP) -1(SP) L2	; test condition
WRT 0(D0)
WRTLN #""
PUSH #1			; increment control variable
PUSH -2(SP)
ADDS
POP -2(SP)		; increment done
BR L1		; branch to loop start

L2:		; loop exit
PUSH #20		; Integer litaral
PUSH 1(D0)	; Identifier N

L3:
BLT -2(SP) -1(SP) L4	; test condition
WRT 0(D0)
WRTLN #""
PUSH #1			; decrement control variable
NEGS
PUSH -2(SP)
ADDS
POP -2(SP)		; decrement done
BR L3		; branch to loop start

L4:		; loop exit

MOV D0 SP
HLT
