import sys

keys = {}

# a .. r : 2-7
for i in xrange(ord('s') - ord('a')):
    keys[chr(i + ord('a'))] = chr(ord('0') + 2 + (i/3)) * (1 + (i%3))

keys['s'] = '7777'

for i in xrange(ord('t')-ord('a'),ord('z')-ord('a')):
    j=i-1
    keys[chr(i + ord('a'))] = chr(ord('0') + 2 + (j/3)) * (1 + (j%3))

keys['z'] = '9999'
keys[' '] = '0'
# print keys

def solve(line):
    last = '-'
    result = ''
    for i in line:
        s = keys[i]
        if s[0] == last:
            result += ' '
        result += s
        last = s[0]
    return result

f = open(sys.argv[1],'r')
N = int(f.readline())

for i in xrange(N):
    #C = int(f.readline())
    #I = int(f.readline())
    line = f.readline().rstrip('\n')
    result = solve(line)
    print 'Case #%d: %s' % ((i+1),result)
