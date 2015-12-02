import sys

class SolvePatternMatching:
    def __init__(self,wordlength,words):
        self.L = wordlength
        self.words = words
    
    def compile(self,pattern):
        r = []
        saving = False
        tmp = ''
        for c in pattern:
            if not saving:
                if c != '(':
                    r.append(c)
                else:
                    saving = True
            else:
                if c != ')':
                    tmp = tmp + c
                else:
                    r.append(tmp)
                    tmp = ''
                    saving = False
        
        #print 'Compiled',pattern,'into',r
        return r

    def matches(self,word,pattern):
        for i in xrange(self.L):
            if pattern[i].find(word[i]) < 0:
                #print 'Word',word,'does not match',pattern
                return False
        #print 'Word',word,'match',pattern
        return True

    def solve(self,pattern):
        p = self.compile(pattern)
        nmatch = 0
        for w in self.words:
            nmatch += int(self.matches(w,p))
        return nmatch

f = open(sys.argv[1],'r')
[L,D,N] = map(int,f.readline().split())

words = []
for i in xrange(D):
    w = f.readline().strip()
    assert len(w) == L, "word %d has wrong length" % i
    words.append(w)

P = SolvePatternMatching(L,words)

for i in xrange(N):
    print 'Case #%d: %d' % ((i+1),P.solve(f.readline().strip()))
