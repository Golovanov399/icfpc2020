#!/usr/bin/env python3

def convert_or_leave_it(a):
	if a == "nil":
		return []
	elif isinstance(a, str):
		return int(a)
	elif isinstance(a, tuple):
		assert len(a) == 2
		l = convert_or_leave_it(a[0])
		r = convert_or_leave_it(a[1])
		if isinstance(r, list):
			return [l] + r
		else:
			return (l, r)
	else:
		return a

def modulate_int(n):
	if n == 0:
		return "010"
	res = ""
	if n >= 0:
		res += "01"
	else:
		res += "10"
		n = -n
	sz = 0
	while n + 1 > 16 ** sz:
		sz += 1
	res += '1' * sz + '0'
	res += bin(n)[2:].rjust(4 * sz, '0')
	return res

def modulate_tokens(tokens):
	res = ""
	i = 0
	while i < len(tokens):
		if tokens[i:i+3] == ["ap", "ap", "cons"]:
			res += "11"
			i += 3
		elif tokens[i] == "nil":
			res += "00"
			i += 1
		else:
			res += modulate_int(int(tokens[i]))
			i += 1
	return res

def modulate(a):
	res = ""
	if isinstance(a, list):
		for x in a:
			res += "11"
			res += modulate(x)
		res += "00"
	elif isinstance(a, tuple):
		assert len(a) == 2
		res += "11" + modulate(a[0]) + modulate(a[1])
	else:
		res += modulate_int(int(a))
	return res

def apapcons(a):
	if isinstance(a, list):
		if not a:
			return ["nil"]
		return ["ap", "ap", "cons"] + apapcons(a[0]) + apapcons(a[1:])
	elif isinstance(a, tuple):
		assert len(a) == 2
		return ["ap", "ap", "cons"] + apapcons(a[0]) + apapcons(a[1])
	else:
		return [str(a)]

def repr(tokens):
	a = tokens[:]
	for i in range(len(a) - 1, -1, -1):
		while True:
			if a[i] == "ap" and a[i + 1] == "ap" and a[i + 2] == "cons":
				a = a[:i] + [(a[i + 3], a[i + 4])] + a[i + 5:]
			else:
				break
	assert len(a) == 1
	a = a[0]
	return convert_or_leave_it(a)

a = input().strip().split()
print(repr(a))
for v in repr(a)[2]:
	print(*map(lambda x: "%s %s" % x, v))
print(modulate(repr(a)[2][0]))
print(*apapcons(repr(a)[1]))
