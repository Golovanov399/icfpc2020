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

lmbd = "##\n#."
eq = "###\n#..\n###"

tokname = {
	'.#\n#.': '0',
	'.#\n##': '1',
	'.##\n#.#\n#..': '2',
	'.##\n###\n#..': '3',
	'.#\n##\n#.': '-1',
	'.##\n#.#\n#..\n#..': '-2',
	'.##\n###\n#..\n#..': '-3',
	'##\n#.': 'lambda',
	'##.####\n#..##..\n...#..#\n...#.##': 'succ',
	'##.####\n#..##..\n...#.#.\n...#.##': 'pred',
	'##.##.####\n#..#..##.#\n......##.#\n......##.#': 'sum',
	'##.##.####\n#..#..#.#.\n......#.#.\n......#.#.': 'prod',
	'##.##.####\n#..#..#...\n......##.#\n......#...': 'div',
	'##.##.####\n#..#..#...\n......#...\n......####': 'eq',
	'##.##.####\n#..#..#...\n......#..#\n......#.##': 'lt',
	'##.####\n#..#.#.\n...##.#\n...#.#.': 'modulate',
	'##.####\n#..##.#\n...#.#.\n...##.#': 'demodulate',
	'###\n#.#\n#..': 'true',
	'###\n#..\n#.#': 'false',
	'##.###\n#..#.#\n...#.#': 'negate',
	'##.##.##.###\n#..#..#..###\n.........##.': 'S',
	'##.##.##.###\n#..#..#..#.#\n.........##.': 'C',
	'##.##.##.###\n#..#..#..##.\n.........##.': 'B',
	'##.##.###\n#..#..#.#\n......#..': 'K',
	'##.##.###\n#..#..#..\n......#.#': 'False',
	'##.#######\n#..#.....#\n...#..##.#\n...#.#.#.#\n...#.#...#\n...#.....#\n...#######': 'pow2',
	'##.##.\n#..##.\n......': 'I',
	'##.##.#####\n#..#..#.#.#\n......#.#.#\n......#.#.#\n......#####': 'Cons',
	'##.#####\n#..#.###\n...#.#.#\n...#.#.#\n...#####': 'Car',
	'##.#####\n#..###.#\n...#.#.#\n...#.#.#\n...#####': 'Cdr',
	'###\n#.#\n###': 'nil',
	'###\n###\n###': 'isemptylist',
	'..#\n.##\n###\n.##\n..#': 'left_bracket',
	'##\n##\n##\n##\n##': 'delim',
	'#..\n##.\n###\n##.\n#..': 'right_bracket',
	'######\n##....\n#.#...\n#..#..\n#...#.\n#....#': 'point',
	'######\n#....#\n#....#\n#....#\n#....#\n######': 'draw',
}

tokname["######\n#.#.#.\n##.#.#\n#.#.#.\n##.#.#\n#.#.#."] = "checkerboard"
tokname["#######\n#..#..#\n#..#..#\n#######\n#..#..#\n#..#..#\n#######\n"] = "multipledraw"
tokname["####\n#.##\n##.#\n#.#."] = "send"
tokname["#####\n#....\n#.###\n###..\n#.###"] = "if0"
tokname["######\n#....#\n#.##.#\n#.##.#\n#....#\n######"] = "interact"
tokname["#######\n#....#.\n#......\n#......\n#......\n#.#....\n#......"] = "statelessdraw"
tokname["#######\n##.....\n##.....\n#......\n#......\n#..#...\n#......"] = "statefuldraw"
# tokname["######\n######\n#####.\n####..\n###..#\n##...."] = "send"

ntok = dict()
for k, v in tokname.items():
	if len(k) < 3:
		ntok[k] = v
		continue
	g = k.split()
	i = 0
	while i + 3 < len(g[0]):
		if g[0][i:i+3] != '##.':
			break
		if g[1][i:i+3] != '#..':
			break
		ok = True
		for j in range(2, len(g)):
			if g[j][i:i+3] != '...':
				ok = False
				break
		if not ok:
			break
		i += 3
	ntok["\n".join(x[i:] for x in g)] = v
tokname = ntok

def decode_number(g):
	if len(g) == len(g[0]) and g[0][0] == '.' and g[0][1:] == '#' * (len(g[0]) - 1) and "".join(x[0] for x in g[1:]) == '#' * (len(g) - 1):
		res = 0
		cur = 1
		for i in range(1, len(g[0])):
			for j in range(1, len(g[0])):
				if g[i][j] == '#':
					res += cur
				cur *= 2
		return str(res)
	elif len(g) == len(g[0]) + 1 and g[0][0] == '.' and g[0][1:] == '#' * (len(g[0]) - 1) and "".join(x[0] for x in g[1:]) == '#' * (len(g) - 1) and g[-1][1:] == '.' * (len(g[0]) - 1):
		res = 0
		cur = 1
		for i in range(1, len(g[0])):
			for j in range(1, len(g[0])):
				if g[i][j] == '#':
					res += cur
				cur *= 2
		return str(-res)
	else:
		return None

def decode_var(g):
	if len(g) != len(g[0]):
		return None
	sz = len(g)
	if g[0] != '#' * sz or g[-1] != '#' * sz:
		return None
	if "".join(s[0] for s in g) != '#' * sz:
		return None
	if "".join(s[-1] for s in g) != '#' * sz:
		return None
	def invert(s):
		return "".join('#' if x == '.' else '.' for x in s)
	tmp = decode_number([invert(s[1:-1]) for s in g[1:-1]])
	if tmp:
		return 'x' + tmp
	else:
		return None

def decode_bitstream(g):
	if len(g) > 2:
		return None
	t = "bits("
	for i in range(len(g[0])):
		if len([j for j in range(len(g)) if g[j][i] == '#']) != 1:
			return None
		t += str(1 if g[0][i] == '#' else 0)
	return t + ")"

def decode_grid(g):
	gs = g.split()
	if g in tokname:
		return tokname[g]
	elif decode_number(gs):
		return decode_number(gs)
	elif decode_var(gs):
		return decode_var(gs)
	elif decode_bitstream(gs):
		return decode_bitstream(gs)
	else:
		return '?'

def build_tree(tokens):
	if not tokens:
		return [], []
	arities = []
	tkns = []
	arity = 0
	for token in tokens:
		if token == "lambda":
			arity += 1
		else:
			tkns.append(token)
			arities.append(arity)
			arity = 0
	assert arity == 0
	sz = len(tkns)
	args = [[] for i in range(sz)]
	st = []
	for i in range(sz - 1, -1, -1):
		for j in range(arities[i]):
			args[i].append(st[-1])
			st.pop()
		st.append(i)
	assert len(st) == 1
	return tkns, args

def repr(tkns, args):
	if not tkns:
		return ""
	sz = len(args)
	reprs = ["" for i in range(sz)]
	for i in range(sz - 1, -1, -1):
		reprs[i] = tkns[i]
		if args[i]:
			reprs[i] += "(" + ", ".join(reprs[j] for j in args[i]) + ")"
	return reprs[0]

def parse_hand_side(tokens):
	tokens = list(map(decode_grid, tokens))
	st = [[]]
	for x in tokens:
		if x == "left_bracket":
			st[-1].append([])
			st.append([])
		elif x == "right_bracket":
			st[-2][-1].append(repr(*build_tree(st[-1])))
			st.pop()
			st[-1][-1] = "[" + ", ".join(st[-1][-1]) + "]"
		elif x == "delim":
			st[-2][-1].append(repr(*build_tree(st[-1])))
			st[-1] = []
		else:
			st[-1].append(x)
	return repr(*build_tree(st[0]))

def parse(grid):
	try:
		tokens = tokenize(grid)
		hand_sides = listsplit(tokens, eq)
		return " = ".join(parse_hand_side(x) for x in hand_sides)
	except:
		return "error"

import sys

grids = open(sys.argv[1]).read().strip().split('\n')
grids = listsplit(grids, '.' * len(grids[0]))
grids = filter(len, grids)

for grid in grids:
	print(parse(grid))