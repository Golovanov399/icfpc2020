import requests
import sys

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

def dem(s, i = 0):
    t = s[i: i + 2]
    i += 2
    if t == '11':
        a = []
        i -= 2
        while 1:
            t = s[i: i + 2]
            i += 2
            if t == '00':
                return a, i
            elif t == '11':
                x, i = dem(s, i)
                a += [x]
            else:
                i -= 2
                x, i = dem(s, i)
                return (a[0], x), i
    elif t == '00':
        return [], i
    else:
        sgn = 1
        if t == '10':
            sgn = -1
        len = 0
        while s[i] == '1':
            len += 1
            i += 1
        i += 1
        t = s[i: i + 4 * len]
        i += 4 * len
        x = 0
        if t != '':
            x = int(t, 2)
        return sgn * x, i

def demodulate(s):
    return dem(s)[0]

def accelerate(id, vec):
    return [0, id, tuple(vec)]

def detonate(id):
    return [1, id]

def shoot(id, target, x):
    return [2, id, target, x]

def clone(id, stats):
    return [3, id, list(stats)]

goodStates = []

def rotate(p):
    x, y = p
    return (y, -x)

def sgn(x):
    if x == 0:
        return 0
    if x > 0:
        return 1
    else:
        return -1

def gravity(p):
    x, y = p
    if abs(x) == abs(y):
        return (-sgn(x), -sgn(y))
    if abs(x) > abs(y):
        return (-sgn(x), 0)
    else:
        return (0, -sgn(y))

def vsum(p, q):
    return (p[0] + q[0], p[1] + q[1])

goodStates = []

def genGoodStates(R):
    global goodStates
    gs = []
    p = (-R * R + R // 2, R * R + R // 2)
    v = (R, R)
    for i in range(2 * R):
        for j in range(4):
            gs += [(p, v)]
            p = rotate(p)
            v = rotate(v)
        v = vsum(v, gravity(p))
        p = vsum(p, v)
    goodStates += [gs]

def dist(a, b):
    return (a[0] - b[0]) ** 2 + (a[1] - b[1]) ** 2

def dist2(a, b):
    return dist(a[0], b[0]) + dist(a[1], b[1])

def steer(orbi, p, v, dodge = 0):
    P = 10

    bsc = 1000000
    st = ()
    for dx in range(-2, 3):
        for dy in range(-2, 3):
            if dodge and (dx, dy) == (0, 0):
                continue
            sc = 1000000
            nv = vsum(v, vsum(gravity(p), (dx, dy)))
            np = vsum(p, nv)
            for w in goodStates[orbi]:
                res = dist2((np, nv), w) + P * max(abs(dx), abs(dy))
                sc = min(sc, res)
            if sc < bsc:
                bsc = sc
                st = (-dx, -dy)
    return st

stable = set()
lstable = []

def steer_stable(p, v, dodge = 0):
    P = 1
    global lstable
    print(len(lstable), p, v)

    bsc = 1000000
    st = ()
    for dx in range(-2, 3):
        for dy in range(-2, 3):
            if dodge and (dx, dy) == (0, 0):
                continue
            sc = 1000000
            nv = vsum(v, vsum(gravity(p), (dx, dy)))
            np = vsum(p, nv)
            for w in lstable:
                res = dist2((np, nv), w) + P * max(abs(dx), abs(dy))
                sc = min(sc, res)
            if sc < bsc:
                bsc = sc
                st = (-dx, -dy)
    print(bsc, st)
    return st

def posPenalty(p):
    R = max(abs(p[0]), abs(p[1]))
    return R <= 16 or R >= 128

def stable(p, v):
    p0 = p
    v0 = v
    lft = 100
    while 1:
        if posPenalty(p):
            return False
        v = vsum(v, gravity(p))
        p = vsum(p, v)
        if (p, v) == (p0, v0):
            break
        lft -= 1
        if lft == 0:
            break
    if posPenalty(p):
        return False
    return (p, v) == (p0, v0)
    
sts = []
stayStill = 0
E0 = 0
maxl = 5

def sidetrack(p, v, E):
#    print(p, v, E)
    global sts, E0, maxl
    if E != E0 and stable(p, v):
        return 1
    if posPenalty(p) or len(sts) > maxl:
        return 0
    for dx in range(-1, 2):
        for dy in range(-1, 2):
            np = p
            nv = v
            nv = vsum(nv, vsum(gravity(np), (dx, dy)))
            np = vsum(np, nv)
            if posPenalty(np):
                continue
            nE = E
            if (dx, dy) != (0, 0):
                nE -= 1
            if nE < 0:
                continue
            sts += [(-dx, -dy)]
            if sidetrack(np, nv, nE):
                return 1
            sts = sts[:-1]
    return 0

def optSideTrack(p, v):
    global E0, sts
    E0 = 1
    while 1:
        sts = []
        if sidetrack(p, v, E0):
            break
        else:
            print("Couldn't find with E0 = %d" % E0)
        E0 += 1
    print("Found new course with %d" % E0)

def next_sh(p, v):
    global sts, stayStill
    sh = (0, 0)
    if len(sts) == 0:
        if stayStill:
            --stayStill;
        else:
            optSideTrack(p, v)
    if len(sts):
        sh = sts[0]
        sts = sts[1:]
        if len(sts) == 0:
            stayStill = 20
    return sh

def main():
    server_url = sys.argv[1]
    player_key = sys.argv[2]
    print('ServerUrl: %s; PlayerKey: %s' % (server_url, player_key))

    url = server_url + '/aliens/send'

    resp = requests.post(url, data=modulate([2, int(player_key), [192496425430, 103652820]]), params={"apiKey": "e8bdb469f76642ce9b510558e3d024d7"})
    print(resp)
    state = demodulate(resp.text)
    print(state)
    our_role = state[2][1]
    their_role = our_role ^ 1
    state = demodulate(requests.post(url, data=modulate([3, int(player_key), [[88,72,8,20], [200,0,8,76]][our_role]]), params={"apiKey": "e8bdb469f76642ce9b510558e3d024d7"}).text)

    noClone = 0
    global sts, stayStill
    sts = []
    while 1:
        print(state)
        if state[0] == 0:
            break
        flag, stage, staticInfo, gameState = state
        if stage == 2:
            break
        our_ships = [x for x in gameState[2] if x[0][0] == our_role] if gameState else None
        their_ships = [x for x in gameState[2] if x[0][0] == their_role] if gameState else None
        cmds = []
        '''
        for ship, _ in our_ships:
            x, y = ship[2]
            def sign(x):
                return -1 if x < 0 else 1 if x > 0 else 0
            if abs(x) < 60 and abs(y) < 60:
                cmds.append(accelerate(ship[1], (-sign(x) if abs(x) >= abs(y) else 0, -sign(y) if abs(y) > abs(x) else 0))


        '''
#        for ship, _ in our_ships:
        divide = len(our_ships) < 4
        for i in range(len(our_ships)):
            ship, _ = our_ships[i]
            stats = ship[4]
            if stats == [0, 0, 0, 0]:
                continue
            if stats[0] == 0:
                for eship, _ in their_ships:
                    D = dist(eship[2], ship[2])
                    if D <= 2:
                        cnds.append(detonate(ship[1]))
                        break
                continue
            buf = ship[6] - ship[5]
            burn = buf < 60
            powah = stats[1]
            dodge = our_role == 1 and buf >= 60

            sh = next_sh(ship[2], ship[3])
            if sh != (0, 0):
                cmds.append(accelerate(ship[1], sh))

            if stats[3] == 1:
                noClone = 1
            if len(sts) == 0 and noClone == 0:
                cmds.append(clone(ship[1], [0, 0, 0, 1]))
                noClone = 50
            else:
                if noClone > 0:
                    noClone -= 1
            if our_role == 0:
                for eship, _ in their_ships:
                    t = vsum(vsum(eship[2], eship[3]), gravity(eship[2]))
                    D = dist(ship[2], t)
                    nD = dist(vsum(ship[2], ship[3]), t)
                    if D > 7000 or D > nD:  
                        continue
                    kamehameha = min(powah, buf)
                    if kamehameha > powah // 2:
                        cmds.append(shoot(ship[1], t, kamehameha))
                        break

        state = demodulate(requests.post(url, data=modulate([4, int(player_key), cmds]), params={"apiKey": "e8bdb469f76642ce9b510558e3d024d7"}).text)

if __name__ == '__main__':
    main()
