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
WRT #"100"
