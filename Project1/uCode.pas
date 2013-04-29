MOV SP D0
<<<<<<< HEAD
=======
ADD SP #7 SP
ADD SP #1 SP
ADD SP #1 SP
PUSH #9
POP 0(D0)
PUSH #0
CASTF ;result does not match assignemnt variable type cast to float
POP 7(D0)
L0
PUSH 1(D0)	; b
PUSH #1
ADDS
POP 1(D0)
WRT #"b"
>>>>>>> origin/SarahParserFollowChange
