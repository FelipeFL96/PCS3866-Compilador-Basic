00 DEF FN dobro(n) = 2 * n
01 DIM a(4)
02 READ a(0), a(1), a(2), a(3)
03 DATA 7, 1, 5, 4
04 LET red = 0
05 LET i = 0
06 LET red = red + FN dobro(a(i))
07 LET i = i + 1
08 IF i < 4 THEN 06
09 END
