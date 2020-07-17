
1:
- `0`
- `error`
- `error`
- `error`
- `error`
- `error`
- `error`
- `error`
- `error`
- `error`

2:
- `error`

3:
- `error`

4:
- ` = `
- `0 = 0`
- `1 = 1`
- `2 = 2`
- `3 = 3`
- `error`
- `10 = 10`
- `11 = 11`
- `error`
- `-1 = -1`
- `-2 = -2`
- `error`

5:
- `succ`
- `succ(0) = 1`
- `succ(1) = 2`
- `succ(2) = 3`
- `succ(3) = 4`
- `error`
- `succ(300) = 301`
- `succ(301) = 302`
- `error`
- `succ(-1) = 0`
- `succ(-2) = -1`
- `succ(-3) = -2`
- `error`

6:
- `pred`
- `pred(1) = 0`
- `pred(2) = 1`
- `pred(3) = 2`
- `pred(4) = 3`
- `error`
- `pred(1024) = 1023`
- `error`
- `pred(0) = -1`
- `pred(-1) = -2`
- `pred(-2) = -3`
- `error`

7:
- `sum`
- `sum(1, 2) = 3`
- `sum(2, 1) = 3`
- `sum(0, 1) = 1`
- `sum(2, 3) = 5`
- `sum(3, 5) = 8`
- `error`

8:
- `error`
- `sum(0, x0) = x0`
- `sum(0, x1) = x1`
- `sum(0, x2) = x2`
- `error`
- `sum(x0, 0) = x0`
- `sum(x1, 0) = x1`
- `sum(x2, 0) = x2`
- `error`
- `sum(x0, x1) = sum(x1, x0)`
- `error`

9:
- `prod`
- `prod(4, 2) = 8`
- `prod(3, 4) = 12`
- `prod(3, -2) = -6`
- `prod(x0, x1) = prod(x1, x0)`
- `prod(x0, 0) = 0`
- `prod(x0, 1) = x0`
- `error`

10:
- `div`
- `div(4, 2) = 2`
- `div(4, 3) = 1`
- `div(4, 4) = 1`
- `div(4, 5) = 0`
- `div(5, 2) = 2`
- `div(6, -2) = -3`
- `div(5, -3) = -1`
- `div(-5, 3) = -1`
- `div(-5, -3) = 1`
- `div(x0, 1) = x0`
- `error`

11:
- `eq`
- `eq(x0, x0) = K`
- `eq(0, -2) = False`
- `eq(0, -1) = False`
- `eq(0, 0) = K`
- `eq(0, 1) = False`
- `eq(0, 2) = False`
- `error`
- `eq(1, -1) = False`
- `eq(1, 0) = False`
- `eq(1, 1) = K`
- `eq(1, 2) = False`
- `eq(1, 3) = False`
- `error`
- `eq(2, 0) = False`
- `eq(2, 1) = False`
- `eq(2, 2) = K`
- `eq(2, 3) = False`
- `eq(2, 4) = False`
- `error`
- `eq(19, 20) = False`
- `eq(20, 20) = K`
- `eq(21, 20) = False`
- `error`
- `eq(-19, -20) = False`
- `eq(-20, -20) = K`
- `eq(-21, -20) = False`
- `error`

12:
- `lt`
- `lt(0, -1) = False`
- `lt(0, 0) = False`
- `lt(0, 1) = K`
- `lt(0, 2) = K`
- `error`
- `lt(1, 0) = False`
- `lt(1, 1) = False`
- `lt(1, 2) = K`
- `lt(1, 3) = K`
- `error`
- `lt(2, 1) = False`
- `lt(2, 2) = False`
- `lt(2, 3) = K`
- `lt(2, 4) = K`
- `error`
- `lt(19, 20) = K`
- `lt(20, 20) = False`
- `lt(21, 20) = False`
- `error`
- `lt(-19, -20) = False`
- `lt(-20, -20) = False`
- `lt(-21, -20) = K`
- `error`

13:
- `modulate`
- `modulate(0) = bits(010)`
- `modulate(1) = bits(01100001)`
- `modulate(-1) = bits(10100001)`
- `modulate(2) = bits(01100010)`
- `modulate(-2) = bits(10100010)`
- `error`
- `modulate(16) = bits(0111000010000)`
- `modulate(-16) = bits(1011000010000)`
- `error`
- `modulate(255) = bits(0111011111111)`
- `modulate(-255) = bits(1011011111111)`
- `modulate(256) = bits(011110000100000000)`
- `modulate(-256) = bits(101110000100000000)`
- `error`

14:
- `demodulate`
- `demodulate(modulate(x0)) = x0`
- `modulate(demodulate(x0)) = x0`

15:
- `send`
- `error`
- `?`
- `error`
- `error`
- `error`
- `error`
- `error`
- `error`

16:
- `negate`
- `negate(0) = 0`
- `negate(1) = -1`
- `negate(-1) = 1`
- `negate(2) = -2`
- `negate(-2) = 2`
- `error`

17:
- `error`
- `succ(succ(0)) = 2`
- `succ(succ(succ(0))) = 3`
- `succ(pred(x0)) = x0`
- `pred(succ(x0)) = x0`
- `pred(sum(x0, 1)) = x0`
- `sum(sum(2, 3), 4) = 9`
- `sum(2, sum(3, 4)) = 9`
- `sum(prod(2, 3), 4) = 10`
- `prod(2, sum(3, 4)) = 14`
- `succ = sum(1)`
- `pred = sum(negate(1))`
- `error`

18:
- `S`
- `S(x0, x1, x2) = x0(x2, x1(x2))`
- `S(sum, succ, 1) = 3`
- `S(prod, sum(1), 6) = 42`
- `error`

19:
- `C`
- `C(x0, x1, x2) = x0(x2, x1)`
- `C(sum, 1, 2) = 3`
- `error`

20:
- `B`
- `B(x0, x1, x2) = x0(x1(x2))`
- `B(succ, pred, x0) = x0`
- `error`

21:
- `K`
- `K(x0, x1) = x0`
- `K(1, 5) = 1`
- `error`
- `K(K, succ(5)) = K`
- `K(succ(5), K) = 6`
- `error`

22:
- `False`
- `False(x0, x1) = x1`
- `False = S(K)`

23:
- `pow2`
- `pow2 = S(C(eq(0), 1), B(prod(2), B(pow2, sum(-1))))`
- `pow2(0) = S(C(eq(0), 1), B(prod(2), B(pow2, sum(-1))), 0)`
- `pow2(0) = C(eq(0), 1, 0, B(prod(2), B(pow2, sum(-1)), 0))`
- `pow2(0) = eq(0, 0, 1, B(prod(2), B(pow2, sum(-1)), 0))`
- `pow2(0) = K(1, B(prod(2), B(pow2, sum(-1)), 0))`
- `pow2(0) = 1`
- `pow2(1) = S(C(eq(0), 1), B(prod(2), B(pow2, sum(-1))), 1)`
- `pow2(1) = C(eq(0), 1, 1, B(prod(2), B(pow2, sum(-1)), 1))`
- `pow2(1) = eq(0, 1, 1, B(prod(2), B(pow2, sum(-1)), 1))`
- `pow2(1) = False(1, B(prod(2), B(pow2, sum(-1)), 1))`
- `pow2(1) = B(prod(2), B(pow2, sum(-1)), 1)`
- `pow2(1) = prod(2, B(pow2, sum(-1), 1))`
- `pow2(1) = prod(2, pow2(sum(-1, 1)))`
- `pow2(1) = prod(2, S(C(eq(0), 1), B(prod(2), B(pow2, sum(-1))), sum(-1, 1)))`
- `pow2(1) = prod(2, C(eq(0), 1, sum(-1, 1), B(prod(2), B(pow2, sum(-1)), sum(-1, 1))))`
- `pow2(1) = prod(2, eq(0, sum(-1, 1), 1, B(prod(2), B(pow2, sum(-1)), sum(-1, 1))))`
- `pow2(1) = prod(2, eq(0, 0, 1, B(prod(2), B(pow2, sum(-1)), sum(-1, 1))))`
- `pow2(1) = prod(2, K(1, B(prod(2), B(pow2, sum(-1)), sum(-1, 1))))`
- `pow2(1) = prod(2, 1)`
- `pow2(1) = 2`
- `pow2(2) = S(C(eq(0), 1), B(prod(2), B(pow2, sum(-1))), 2)`
- `error`
- `pow2(2) = 4`
- `pow2(3) = 8`
- `pow2(4) = 16`
- `pow2(5) = 32`
- `pow2(6) = 64`
- `pow2(7) = 128`
- `pow2(8) = 256`
- `error`

24:
- `error`
- `error`
- `error`
- `error`
- `error`
- `error`
- `error`

25:
- `Cons`
- `Cons(x0, x1, x2) = x2(x0, x1)`

26:
- `Car`
- `Car(Cons(x0, x1)) = x0`
- `Car(x2) = x2(K)`

27:
- `Cdr`
- `Cdr(Cons(x0, x1)) = x1`
- `Cdr(x2) = x2(False)`

28:
- `nil`
- `nil(x0) = K`

29:
- `isemptylist`
- `isemptylist(nil) = K`
- `isemptylist(Cons(x0, x1)) = False`

30:
- `[, ]`
- `[] = nil`
- `[x0] = Cons(x0, nil)`
- `[x0, x1] = Cons(x0, Cons(x1, nil))`
- `[x0, x1, x2] = Cons(x0, Cons(x1, Cons(x2, nil)))`
- `[x0, x1, x2, x5] = Cons(x0, Cons(x1, Cons(x2, Cons(x5, nil))))`
- `error`

31:
- `point`
- `point = Cons`

32:
- `draw`
- `draw([]) = ?`
- `draw([point(1, 1)]) = ?`
- `draw([point(1, 2)]) = ?`
- `draw([point(2, 5)]) = ?`
- `draw([point(1, 2), point(3, 1)]) = ?`
- `draw([point(5, 3), point(6, 3), point(4, 4), point(6, 4), point(4, 5)]) = ?`
- `error`

33:
- `checkerboard`
- `error`
- `checkerboard(7, 0) = ?`
- `checkerboard(13, 0) = ?`

34:
- `multipledraw`
- `multipledraw(nil) = nil`
- `multipledraw(Cons(x0, x1)) = Cons(draw(x0), multipledraw(x1))`

35:
- `error`
- `modulate(nil) = bits(11)`
- `modulate(Cons(nil, nil)) = bits(110000)`
- `modulate(Cons(0, nil)) = bits(1101000)`
- `modulate(Cons(1, 2)) = bits(110110000101100010)`
- `modulate(Cons(1, Cons(2, nil))) = bits(1101100001110110001000)`
- `modulate([1, 2]) = bits(1101100001110110001000)`
- `modulate([1, [2, 3], 4]) = bits(1101100001111101100010110110001100110110010000)`
- `error`

36:
- `?`
- `send([0]) = [1, ?]`

37:
- `if0`
- `if0(0, x0, x1) = x0`
- `if0(1, x0, x1) = x1`

38:
- `interact`
- `?(x0) = demodulate(modulate(x0))`
- `?(x2, x0) = if0(Car(x0), [?(Car(Cdr(x0))), multipledraw(Car(Cdr(Cdr(x0))))], interact(x2, ?(Car(Cdr(x0))), send(Car(Cdr(Cdr(x0))))))`
- `interact(x2, x4, x5) = ?(x2, x2(x4, x5))`

39:
- `interact`
- `interact(x0, nil, point(0, 0)) = [x16, multipledraw(x64)]`
- `interact(x0, x16, point(x1, x2)) = [x17, multipledraw(x65)]`
- `interact(x0, x17, point(x3, x4)) = [x18, multipledraw(x66)]`
- `interact(x0, x18, point(x5, x6)) = [x19, multipledraw(x67)]`
- `error`

40:
- `interact(statelessdraw)`
- `statelessdraw(x0, x1) = [0, nil, [[x1]]]`
- `statelessdraw = C(B(B, B(B(Cons(0)), C(B(B, Cons), C(Cons, nil)))), C(B(Cons, C(Cons, nil)), nil))`
- `interact(statelessdraw, nil, point(1, 0)) = [nil, [?]]`
- `interact(statelessdraw, nil, point(2, 3)) = [nil, [?]]`
- `interact(statelessdraw, nil, point(4, 1)) = [nil, [?]]`
- `error`

41:
- `interact(statefuldraw)`
- `statefuldraw(x0, x1) = [0, Cons(x1, x0), [Cons(x1, x0)]]`
- `statefuldraw = B(B(S(B(B(Cons(0)), C(B(B, Cons), C(Cons, nil))), C(Cons, nil))), C(Cons))`
- `interact(statefuldraw, nil, point(0, 0)) = [[point(0, 0)], [?]]`
- `interact(statefuldraw, [point(0, 0)], point(2, 3)) = [x2, [?]]`
- `interact(statefuldraw, x2, point(1, 2)) = [x3, [?]]`
- `interact(statefuldraw, x3, point(3, 2)) = [x4, [?]]`
- `interact(statefuldraw, x4, point(4, 0)) = [x5, [?]]`
- `error`

42:
- `error`

