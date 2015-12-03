import sys

def read_fonts(f,s,out):
    for linenum in xrange(5):
        parts = f.readline().split()
        for (i,p) in enumerate(parts):
            k = s[i]
            if k in out:
				out[k] += p[:-1]
            else:
				out[k] = p[:-1]

    f.readline()

f = open(sys.argv[1])

out = {}

read_fonts(f,"ABCDEFGHI",out)
read_fonts(f,"JKLMNOPQR",out)
read_fonts(f,"STUVWXYZ ",out)

for k in sorted(out.keys()):
    print '"'+out[k]+'",','//',k
