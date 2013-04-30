MOV SP D0

ADD SP #5 SP	; space for 5 variables of type MP_INTEGER_LIT
RD D9			; Read Var a
MOV D9 0(D0)
RD D9			; Read Var b
MOV D9 1(D0)
RD D9		; Read Var c
MOV D9 2(D0)
RD D9		; Read Var d
MOV D9 3(D0)
RD D9		; Read Var e
MOV D9 4(D0)
PUSH #100		; Integer litaral
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH #100		; Integer litaral
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
MULS
WRTS
WRTLN #""
PUSH 2(D0)	; Identifier c
PUSH 3(D0)	; Identifier d
DIVS
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH 4(D0)	; Identifier e
MODS
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
ADDS
WRTS
WRTLN #""
PUSH 2(D0)	; Identifier c
PUSH 1(D0)	; Identifier b
SUBS
WRTS
WRTLN #""
PUSH #88		; Integer litaral
WRTS
WRTLN #""
PUSH #66		; Integer litaral
NEG -1(SP) -1(SP)		; optional sign negative
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
ADDS
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH 1(D0)	; Identifier b
ADDS
NEG -1(SP) -1(SP)		; optional sign negative
WRTS
WRTLN #""

MOV D0 SP
HLT
