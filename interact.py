#!/usr/bin/env python3

import subprocess as sp
import os
import requests

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

def demodulate(s):
	return eval(sp.check_output(["./destroy", "demodulate"], input=s.encode()).decode().strip())

def talk(data):
	print("[*] %s" % data)
	resp = requests.post(
			   "https://icfpc2020-api.testkontur.ru/aliens/send",
			   params={"apiKey": "e8bdb469f76642ce9b510558e3d024d7"},
			   data=modulate(data)).text
	res = demodulate(resp)
	print("[<] %s" % res)
	return res

def create():
	return talk([1, 0])

def join(key, last=[]):
	return talk([2, key, last])

def start(key, stats):
	return talk([3, key, list(stats)])

def commands(key, cmds):
	return talk([4, key, list(cmds)])

def accelerate(id, vec):
	return [0, id, tuple(vec)]

def detonate(id):
	return [1, id]

def shoot(id, target, x):
	return [2, id, target, x]

def clone(id, stats):
	return [3, id, list(stats)]

def repr_ship_with_commands(swc):
	sh, cm = swc
	return ' ' * 12 + "ship: {\n" +\
		' ' * 16 + "role: " + str(sh[0]) + "\n" +\
		' ' * 16 + "id: " + str(sh[1]) + "\n" +\
		' ' * 16 + "position: " + str(sh[2]) + "\n" +\
		' ' * 16 + "velocity: " + str(sh[3]) + "\n" +\
		' ' * 16 + "stats: " + str(sh[4]) + "\n" +\
		' ' * 16 + "damage?: " + str(sh[5]) + "\n" +\
		' ' * 16 + "max hp?: " + str(sh[6]) + "\n" +\
		' ' * 16 + "isalive?: " + str(sh[7]) + "\n" +\
		' ' * 12 + "}, commands: %s" % cm

def main():
	key = None
	keys = None
	while True:
		gamestage, staticgameinfo, gamestate = None, None, None
		cmd = input("[>] ").strip().split()
		if cmd[0] == "create":
			keys = [x[1] for x in create()[1]]
		elif cmd[0] == "join":
			if cmd[1] in "01":
				key = keys[int(cmd[1])]
			elif cmd[1].startswith("att"):
				key = keys[0]
			elif cmd[1].startswith("def"):
				key = keys[1]
			elif cmd[1].isdigit():
				key = int(cmd[1])
			else:
				print("[ ] cannot understand you :/")
			if key:
				print("[.] %s" % key)
				_, gamestage, staticgameinfo, gamestate = join(key)
		elif cmd[0] == "start":
			_, gamestage, staticgameinfo, gamestate = start(key, map(int, cmd[1:]))
		elif cmd[0] == "go":
			i = 1
			cmds = []
			while i < len(cmd):
				if cmd[i].startswith("acc"):
					i += 1
					cmds.append(accelerate(int(cmd[i]), (int(cmd[i + 1]), int(cmd[i + 2]))))
					i += 3
				elif cmd[i].startswith("det"):
					i += 1
					cmds.append(detonate(int(cmd[i])))
					i += 1
				elif cmd[i].startswith("sh"):
					i += 1
					cmds.append(shoot(int(cmd[i]), (int(cmd[i + 1]), int(cmd[i + 2])), int(cmd[i + 3])))
					i += 4
				elif cmd[i].startswith("cl") or cmd[i].startswith("div"):
					i += 1
					cmds.append(clone(int(cmd[i]), list(map(int, cmd[i+1:i+5]))))
					i += 5
				else:
					print("[ ] cannot understand you :/")
			_, gamestage, staticgameinfo, gamestate = commands(key, cmds)
		if gamestage is not None:
			print("[:] gamestage = %s" % gamestage)
			print("[:] staticgameinfo = %s" % staticgameinfo)
			if gamestate:
				print("[:] gamestate = (\n        tick = %s,\n        x1 = %s,\n        ships_and_commands = [\n%s\n        ]\n    )" % (
					gamestate[0],
					gamestate[1],
					",\n".join(map(repr_ship_with_commands, gamestate[2])))
				)

main()
