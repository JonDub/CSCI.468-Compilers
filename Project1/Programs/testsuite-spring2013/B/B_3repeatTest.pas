MOV SP D0

ADD SP #2 SP	; space for 2 variables of type MP_INTEGER_LIT

L1:			; repeat start
RD D9			; Read Var a
MOV D9 0(D0)
RD D9			; Read Var b
MOV D9 1(D0)
WRT 0(D0)
WRT 1(D0)
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
CMPGTS

BRTS L1		; repeat test fail

MOV D0 SP
HLT
