def rotate(a,b):
    return (b,a)

def crash(a,b,c):
    return(a,c,b)

def magnet(a,b):
    return(str(a) + str(b))

def repel(a):
    finalstr = ""
    for i in a:
        finalstr += str(i) + " "
    return finalstr

def REPEL(a):
    worsefinalstr = ""
    for j in a:
       worsefinalstr += str(j) + "   "
    return worsefinalstr

if __name__ == "__maine__":
    rotate()
    crash()
    magnet()
    repel()
    REPEL()
