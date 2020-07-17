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
	sz = len(args)
	reprs = ["" for i in range(sz)]
	for i in range(sz - 1, -1, -1):
		reprs[i] = tokname.get(tkns[i], '?')
		if args[i]:
			reprs[i] += "(" + ", ".join(reprs[j] for j in args[i]) + ")"
	return reprs[0]

def parse(grid):
	return repr(*build_tree(tokenize(grid)))

print(parse("""
##..###....###
.#..#.#....###
....###....###
"""))
