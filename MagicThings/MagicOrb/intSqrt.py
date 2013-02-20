from math import sqrt

def intQuadro(v, n, shift = 1):
    return v * v / n

def intSqrtTable(n, shift = 1):
    return (intQuadro(v, n, shift) for v in xrange(n + 1))

def strIntSqrtTable(n, shift = 1):
    return ', '.join(str(v) for v in intSqrtTable(n, shift))

def printIntSqrtTable(n, indent, maxLength, shift = 1):
    s = ' ' * indent
    length = indent
    for w in strIntSqrtTable(n, shift).split(' '):
        w = w.replace(',', ', ')
        if length + len(w) >= maxLength:
            print s
            s = ' ' * indent
            length = indent
        s += w
        length += len(w)
    if s:
        print s

printIntSqrtTable(255, 8, 80)
