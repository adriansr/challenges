import sys
import itertools

class Problem:
    def __init__(self,nflavors):
        self.N = nflavors
        self.customers = []
        #self.result = [0] * self.N
    
    def addCustomer(self,line):
        part = map(int,line.split())
        num = part[0]
        constrains = []
        for i in xrange(num):
            constrains.append((part[1+i*2],part[2+i*2]!=0))

        self.customers.append(constrains)
    
    def evaluate(self,result):
        
        for c in self.customers:
            for rule in c:
                if result[rule[0]-1] == int(rule[1]):
                    break
            else:
                return False

        return True

    def solve(self):
       
        mandatory = {}
        choices = {}

        #
        # start with no malted flavours
        #
        result = [0] * self.N
        
        for c in self.customers:

            #
            # please customers with only one choice and detect contradictions
            #
            if len(c) == 1:
                rule = c[0]
                if rule[0] in mandatory:
                    if mandatory[rule[0]] != rule[1]:
                        # already exists with opposite maltedness
                        return None
                else:
                    mandatory[rule[0]] = rule[1]
            else:
                #
                # customers with multiple choices and some malted are a problem
                #
                for rule in c:
                    if rule[1]:
                        if rule[0] not in mandatory:
                            choices[rule[0]] = True

        #
        # enforce mandatory flavours
        #
        for key in mandatory:
            result[key-1] = int( mandatory[key] )

        # check if sufficient by forcing mandatory flavors
        if self.evaluate(result) == True:
            return result

        # bruteforce choices, picking the least possible malted flavours
        NC = len(choices)
        for nmalted in xrange(1,NC-1):
            for c in itertools.combinations(choices,nmalted):
                r = result
                for ci in c:
                    r[ci-1] = 1
                if self.evaluate(result) == True:
                    return result

        # last chance: all choices malted
        for key in choices:
            result[key-1] = 1

        if self.evaluate(result) == True:
            return result

        return None


f = open(sys.argv[1],'r')
C = int(f.readline())

for testn in xrange(C):
    N = int(f.readline()) # flavors
    M = int(f.readline()) # customers
    
    P = Problem(N)
    
    for i in xrange(M):
        P.addCustomer(f.readline())
    
    result = P.solve()
    print 'Case #%d:' % (testn+1),
    if result is None:
        print 'IMPOSSIBLE'
    else:
        print ' '.join(map(str,result))
