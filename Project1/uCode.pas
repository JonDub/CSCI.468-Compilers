MOV SP D0
ADD SP #2 SP
PUSH #10
POP 0(D0)
PUSH #1
POP 1(D0)
MOV D0 SP
HLT
