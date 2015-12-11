import Image

png = Image.open('HoN.png')
out = png.copy()
dims = png.size

def m(val):
    return ( val & 1) * 255

for y in xrange(dims[1]):
    for x in xrange(dims[0]):
        p = png.getpixel( (x,y) )
        out.putpixel((x,y) , (m(p[0]),m(p[1]),m(p[2])))
out.save('out.png')

#r = []
#g = []
#b = []
#for pixel in data:
#    r.append(pixel[0] & 3)
#    g.append(pixel[1] & 3)
#    b.append(pixel[2] & 3)
#
#ascii = ''
#capture = []
#capturing = False
#for pos in xrange(len(b)):
#    bit = (r[pos] ^ g[pos] ^ b[pos] ) & 1
#    if capturing:
#        capture.append(bit)
#        if len(capture) == 8:
#            capturing = False
#            num = 0
#            for i in range(8):
#                num |= (int(capture[i]) << i)
#            #if num>=32 and num<=126:
#            ascii = ascii + chr(num)
#
#    elif bit == 0:
#        capturing = True
#        capture = []
#
#print r
#
