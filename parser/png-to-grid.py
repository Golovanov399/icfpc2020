from PIL import Image
import sys

img = Image.open(sys.argv[1])
lx = 10000
rx = 10000
W, H = img.size
sz = 0
px = img.load()
while px[sz, 0] == (255, 255, 255, 255):
    sz += 1
sz += 1
w = W // sz
h = H // sz

s = [['.' for i in range(w - 2)] for j in range(h - 2)]
for i in range(h - 2):
    for j in range(w - 2):
        r, g, b, l = px[j * sz + sz + sz // 2, i * sz + sz + sz // 2]
        if r + g + b > 302:
            s[i][j] = '#'

print('\n'.join([''.join(l) for l in s]))

