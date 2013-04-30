MOV SP D0

ADD SP #13 SP	; space for 13 variables of type MP_INTEGER_LIT
PUSH #1111111		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L1:
PUSH #5		; Integer litaral
PUSH #2		; Integer litaral
MULS
PUSH 0(D0)	; Identifier a
CMPGTS
PUSH #4		; Integer litaral
PUSH 0(D0)	; Identifier a
CMPGTS
ORS

BRFS L2		; while false
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)

BR L1		; while true

L2:		; while false
PUSH #2222222		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L3:
PUSH #5		; Integer litaral
PUSH #2		; Integer litaral
MULS
PUSH 0(D0)	; Identifier a
CMPLTS
PUSH #4		; Integer litaral
PUSH 0(D0)	; Identifier a
CMPGTS
ORS

BRFS L4		; while false
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)

BR L3		; while true

L4:		; while false
PUSH #3333333		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L5:
PUSH #5		; Integer litaral
PUSH #2		; Integer litaral
MULS
PUSH 0(D0)	; Identifier a
CMPGTS
PUSH #400		; Integer litaral
PUSH 0(D0)	; Identifier a
CMPLTS
ORS

BRFS L6		; while false
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)

BR L5		; while true

L6:		; while false
PUSH #4444444		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L7:
PUSH #5		; Integer litaral
PUSH #2		; Integer litaral
MULS
PUSH 0(D0)	; Identifier a
CMPLTS
PUSH #400		; Integer litaral
PUSH 0(D0)	; Identifier a
CMPLTS
ORS

BRFS L8		; while false
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)

BR L7		; while true

L8:		; while false
PUSH #1111111		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L9:
PUSH 0(D0)	; Identifier a
PUSH #10		; Integer litaral
CMPLES
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
CMPLES
ANDS

BRFS L10		; while false
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)

BR L9		; while true

L10:		; while false
PUSH #2222222		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L11:
PUSH 0(D0)	; Identifier a
PUSH #10		; Integer litaral
CMPLES
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
CMPEQS
ANDS

BRFS L12		; while false
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)

BR L11		; while true

L12:		; while false
PUSH #3333333		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L13:
PUSH 0(D0)	; Identifier a
PUSH #0		; Integer litaral
PUSH #0		; Integer litaral
MULS
CMPNES
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
CMPLTS
ANDS

BRFS L14		; while false
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)

BR L13		; while true

L14:		; while false
PUSH #4444444		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L15:
PUSH 0(D0)	; Identifier a
PUSH #0		; Integer litaral
PUSH #0		; Integer litaral
MULS
CMPNES
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
CMPGTS
ANDS

BRFS L16		; while false
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)

BR L15		; while true

L16:		; while false
RD D9			; Read Var a
MOV D9 0(D0)
RD D9		; Read Var b
MOV D9 1(D0)
RD D9		; Read Var c
MOV D9 2(D0)
RD D9		; Read Var d
MOV D9 3(D0)
PUSH #2222222		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L17:
PUSH 0(D0)	; Identifier a
PUSH #3		; Integer litaral
CMPLTS
PUSH #2		; Integer litaral
PUSH 3(D0)	; Identifier d
CMPLTS
ANDS
PUSH 1(D0)	; Identifier b
PUSH #10		; Integer litaral
CMPLTS
ANDS

BRFS L18		; while false
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""

BR L17		; while true

L18:		; while false
PUSH #3333333		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L19:
PUSH 0(D0)	; Identifier a
PUSH #3		; Integer litaral
CMPLTS
PUSH #2		; Integer litaral
PUSH 3(D0)	; Identifier d
CMPGTS
PUSH 1(D0)	; Identifier b
PUSH #10		; Integer litaral
CMPLTS
ANDS
ORS

BRFS L20		; while false
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""

BR L19		; while true

L20:		; while false
PUSH #4444444		; Integer litaral
WRTS
WRTLN #""
PUSH #0		; Integer litaral
POP 0(D0)

L21:
PUSH 0(D0)	; Identifier a
PUSH #3		; Integer litaral
CMPLTS
PUSH #2		; Integer litaral
PUSH 3(D0)	; Identifier d
CMPLTS
ANDS
PUSH 1(D0)	; Identifier b
PUSH #10		; Integer litaral
CMPGTS
ORS

BRFS L22		; while false
PUSH 0(D0)	; Identifier a
PUSH #1		; Integer litaral
ADDS
POP 0(D0)
PUSH 0(D0)	; Identifier a
WRTS
WRTLN #""

BR L21		; while true

L22:		; while false

MOV D0 SP
HLT
