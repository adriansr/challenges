class PrimeEnum:
    def __init__(self,limit):
        self.isP = [False,False] + [True] * limit
        self.pos = 3
        self.limit = limit

    def next(self):
        while self.isP[self.pos] == False:
            self.pos += 2
            #if self.pos > self.limit:
            #    return None

        r = self.pos
        self.update(r)
        self.pos += 2
        return r
    
    def update(self,prime):
        pos = prime * 2
        while pos <= self.limit:
            self.isP[pos] = False
            pos += prime


def solve(n,plist):
    
    v = range(1,n+1)
    pos = -1
    pi = 0
    for rem in xrange(len(v),1,-1):
        pos = ( pos + plist[pi]) % rem
        pi += 1
        del v[pos]
        pos -= 1
        if pos < 0:
            pos += rem-1


    return v[0]

#print solve(6)
pe = PrimeEnum(50000)
p = [2]
while len(p)<3502:
    pp = pe.next()
    p.append(pp)

for i in xrange(1,3502):
    v = str(solve(i,p))
    print '"'+v+'\\n",'

