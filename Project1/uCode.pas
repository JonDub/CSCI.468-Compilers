MOV SP D0
ADD SP #1 SP
ADD SP #1 SP
ADD SP #1 SP
ADD SP #1 SP
WRT #"Enter an integer for A: "
WRTLN #""
RD D9			; Read Var A
MOV D9 0(D0)
WRT #"A = "
WRT 0(D0)
WRTLN #""
WRT #"Enter a float for B: "
RDF D9		; Read Var B
MOV D9 1(D0)
WRT #"B = "
WRT 1(D0)
WRTLN #""
WRT #"Enter an integer for C: "
RD D9			; Read Var C
MOV D9 2(D0)
WRT #"C = "
WRT 2(D0)
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
PUSH -2(SP)	; Casting from int to float
CASTSF
POP -2(SP)	; Cast done
ADDSF
PUSH 2(D0)	; C
PUSH -2(SP)	; Casting from int to float
CASTSI
POP -2(SP)	; Cast done
SUBS
POP 0(D0)
WRT #"A = "
WRT 0(D0)
WRT #", B = "
WRT 1(D0)
WRT #", and C = "
WRT 2(D0)
WRTLN #""
PUSH #1			; boolean true
POP 3(D0)		; boolean assign
WRT #" boo = "
WRT 3(D0)
WRTLN #""
MOV D0 SP
HLT
