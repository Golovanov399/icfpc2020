#!/usr/bin/python3

# echo -n > deapped.txt; while IFS= read -r l; do ./deap.py <<< $l >> deapped.txt; done < galaxy.txt

class Tree:
	def __init__(self, x, y):
		self.l = x
		self.r = y

	def __str__(self):
		return "(" + str(self.l) + " " + str(self.r) + ")"

def is_list(a):
	i = 0
	while i < len(a) - 1:
		if a[i] == 'ap' and a[i + 1] == 'ap' and a[i + 2] == 'cons':
			i += 4
		else:
			return False
	return i == len(a) - 1 and a[-1] == 'nil'

def to_list(a):
	return "[" + ",".join(a[3:len(a):4]) + "]"

def get_tree(a):
	if is_list(a):
		return to_list(a)
	while len(a) > 1:
		i = len(a) - 1
		while a[i] != 'ap':
			i -= 1
		a = a[:i] + [Tree(a[i + 1], a[i + 2])] + a[i + 3:]
	return a[0]

def f(eq):
	return eq[:eq.index('=')+2] + str(get_tree(list(eq[eq.index('=')+2:].split())))

print(f(input().strip().replace(':', 'x')))
