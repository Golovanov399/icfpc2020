#!/usr/bin/python3

with open("../../Golovanov399.github.io/icfpc2020/gen.py") as f:
	cur = []
	need_to_write = False
	for s in f:
		if not s.strip():
			cur = []
			need_to_write = False
		elif "draw_grid" in s:
			need_to_write = True
		elif "replace" in s:
			t = s[s.find("imgs"):]
			print("'%s': %s," % ("\\n".join(cur), t[:t.find(".png")]))
			cur = []
			need_to_write = False
		else:
			cur.append(s.strip())