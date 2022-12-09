def diff(a):
    if a < 0:
        return -1
    elif a == 0:
        return 0
    else:
        return 1

def adj(a, b, c, d):
    return abs(a-c) < 2 and abs(b-d) < 2

hx, hy, tx, ty = 0, 0, 0, 0

tail_visited = set([(0,0)])
with open("day_nine.txt") as fp:
    for line in fp:
        match line.strip().split():
            case ("U", x):
                dx, dy = 0, 1
                amt = int(x)
            case ("D", x):
                dx, dy = 0, -1
                amt = int(x)
            case ("R", x):
                dx, dy = 1, 0
                amt = int(x)
            case ("L", x):
                dx, dy = -1, 0
                amt = int(x)
    
        for _ in range(amt):
            hx += dx
            hy += dy

            # Fix tail position
            if not adj(hx, hy, tx, ty):
                tx += diff(hx-tx)
                ty += diff(hy-ty)
            tail_visited.add((tx, ty))

print(len(tail_visited))
            
tail_visited = set([(0,0)])

xs = [0]*10
ys = [0]*10

with open("day_nine.txt") as fp:
    for line in fp:
        match line.strip().split():
            case ("U", x):
                dx, dy = 0, 1
                amt = int(x)
            case ("D", x):
                dx, dy = 0, -1
                amt = int(x)
            case ("R", x):
                dx, dy = 1, 0
                amt = int(x)
            case ("L", x):
                dx, dy = -1, 0
                amt = int(x)
    
        for _ in range(amt):
            xs[0] += dx
            ys[0] += dy

            for k in range(1,10):
                p = k-1
            # Fix tail position
                if not adj(xs[p], ys[p], xs[k], ys[k]):
                    xs[k] += diff(xs[p]-xs[k])
                    ys[k] += diff(ys[p]-ys[k])
                if k == 9:
                    tail_visited.add((xs[k], ys[k]))

print(len(tail_visited))
