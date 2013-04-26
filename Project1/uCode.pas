MOV SP D0
ADD SP #1 SP
ADD SP #1 SP
ADD SP #1 SP
ADD SP #1 SP
WRT #"Enter an integer for A: "
RD D9
MOV D9 0(D0)
WRT #"A = "
PUSH 0(D0)	; A
WRTS
WRTLN #""
WRT #"Enter a float for B: "
RDF D9
MOV D9 1(D0)
WRT #"B = "
PUSH 1(D0)	; B
WRTS
WRTLN #""
WRT #"Enter an integer for C: "
RD D9
MOV D9 2(D0)
WRT #"C = "
PUSH 2(D0)	; C
WRTS
WRTLN #""
PUSH #3
PUSH 0(D0)	; A
MULS
PUSH 0(D0)	; A
MULS
PUSH 0(D0)	; A
MULS
PUSH #5
PUSH 1(D0)	; B
PUSH -2(SP)	; Casting from int to float
CASTSF
POP -2(SP)
MULSF
PUSH 1(D0)	; B
MULSF
PUSH 2(D0)	; C
POP 0(D0)
WRT #"A = "
PUSH 0(D0)	; A
WRTS
WRT #", A = "
PUSH 1(D0)	; B
WRTS
WRT #", and A = "
PUSH 2(D0)	; C
WRTS
MOV D0 SP
HLT
