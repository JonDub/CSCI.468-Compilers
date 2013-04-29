MOV SP D0
ADD SP #1 SP
ADD SP #1 SP
PUSH #1			; boolean true
POP 0(D0)			; boolean assign
L1:
PUSH 1(D0)	; test2
BRFS L2		; while false
PUSH #0			; boolean false
POP 0(D0)			; boolean assign
BR L1		; while true
L2
MOV D0 SP
HLT
