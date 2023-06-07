import os
n = 5

allWords = open('allWords.txt')
outWords = 'Words.txt'
if not os.path.exists(outWords):
    wfile = open(outWords, 'w')
wfile = open(outWords, 'w')
rfile = allWords.read()

print(len(rfile))
length = 0
valid = True
for i in range(len(rfile)):
    if rfile[i].islower():
        length += 1
        #print(length, valid)
    elif (rfile[i] == '\n') or (i == len(rfile)):
        if length != 5:
            valid = False
        if valid == True:
            line = ''
            for j in range(n):
                line += rfile[i + j - n]
            wfile.write(line)
            wfile.write('\n')
        valid = True
        length = 0
    else:
        valid = False

wfile.close()