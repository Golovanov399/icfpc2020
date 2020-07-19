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

def main():
    server_url = sys.argv[1]
    player_key = sys.argv[2]
    print('ServerUrl: %s; PlayerKey: %s' % (server_url, player_key))

    url = server_url + '/aliens/send'

    resp = requests.post(url, data=modulate([2, int(player_key), []]), params={"apiKey": "e8bdb469f76642ce9b510558e3d024d7"})
    print(resp)
    state = demodulate(resp.text)
    print(state)
    state = demodulate(requests.post(url, data=modulate([3, int(player_key), [4, 4, 4, 4]]), params={"apiKey": "e8bdb469f76642ce9b510558e3d024d7"}).text)

    while 1:
        print(state)
        if state[0] == 0:
            break
        flag, stage, staticInfo, gameState = state
        if stage == 2:
            break
        state = demodulate(requests.post(url, data=modulate([4, int(player_key), []]), params={"apiKey": "e8bdb469f76642ce9b510558e3d024d7"}).text)

if __name__ == '__main__':
    main()
