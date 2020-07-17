#!/usr/bin/env python3

def listsplit(a, x):
	b = [[]]
	for y in a:
		if y == x:
			b.append([])
		else:
			b[-1].append(y)
	return b

def tokenize(a):
	if isinstance(a, str):
		a = a.strip().replace('\t', '').split('\n')
	columns = ["".join(s[j] for s in a) for j in range(len(a[0]))]
	grid_tokens = listsplit(columns, '.' * len(a))
	grid_tokens = filter(len, grid_tokens)
	def f(l):
		gt = ["".join(s[j] for s in l) for j in range(len(l[0]))]
		while gt[-1] == '.' * len(gt[-1]):
			gt.pop()
		while gt[0] == '.' * len(gt[0]):
			gt = gt[1:]
		return "\n".join(gt)
	grid_tokens = list(map(f, grid_tokens))
	return grid_tokens

lmbd = "##\n.#"

def build_tree(tokens):
	arities = []
	tkns = []
	arity = 0
	for token in tokens:
		if token == lmbd:
			arity += 1
		else:
			tkns.append(token)
			arities.append(arity)
			arity = 0
	assert arity == 0
	sz = len(tkns)
	args = [[] for i in range(len(sz))]
	st = []
	for i in range(sz - 1, -1, -1):
		for j in range(arities[i]):
			args[i].append(st[-1])
			st.pop()
		st.append(i)
	assert len(st) == 1
	return tkns, args

print(*tokenize("""
##..###....###
.#..#.#....###
....###....###
"""), sep="\n\n")

# def grid2txt(a, )