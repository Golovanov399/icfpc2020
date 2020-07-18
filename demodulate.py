#!/usr/bin/env python3

s = input().strip()

i = 0

def getNum():
    global i, s
    sgn = s[i: i + 2]
    i += 2
    len = 0
    while s[i] == '1':
        i += 1
        len += 1
    i += 1
    num = 0
    for j in range(4 * len):
        num = 2 * num + int(s[i])
        i += 1
    if sgn == '10':
        num = -num
    return num

def parse():
    global i, s
    t = s[i: i + 2]
    i += 2
    if t == '11':
        res = [parse()]
        while 1:
            t = s[i: i + 2]
            i += 2
            if t == '00':
                break
            res += [parse()]
        return res
    else:
        i -= 2
        return getNum()

print(parse())
