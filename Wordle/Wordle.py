import numpy as np
import string

# If n is increased or decreased, the word clas will need to be changed too.
## Working on fixing that.
n = 5

# Create word class for a linked list of all the words.
class Word:
    def __init__(self, letters, position):
        self.letters = letters
        self.position = position
        self.next = None
        self.prev = None

    def returnL(self):
        ret = ''
        for i in range(n):
            ret = ret + self.letters[i]
        return ret


class WordsList:
    def __init__(self):
        self.words = []
        self.head = None
        self.tail = None

    def append(self, letters):
        new_node = Word(letters, 0)
        if self.head is None:
            self.head = new_node
            self.tail = new_node
            self.position = 0
            self.words.append(new_node)
        else:
            new_node.prev = self.tail
            self.tail.next = new_node
            self.tail = new_node
            self.tail.position = self.tail.prev.position + 1
            self.words.append(new_node)

    def displayAll(self):
        current = self.head
        while current:
            print(np.array2string(current.letters))
            current = current.next

    def findPosition(self, pos):
        current = self.head
        while current.position < pos:
            current = current.next
        return current
    
    def highScore(self, fre, typ):
        if typ == 1:
            high = 0
            loc = 0
            current = self.head
            while current:
                score = 0
                list = []
                for i in current.letters:
                    rep = 0
                    for let in list:
                        if let == i:
                            rep += 1
                    list.append(i)
                    score += fre[i][3 * n + rep]
                if score > high:
                    high = score
                    loc = current.position
                current = current.next
        return loc
    
    def nWords(self, current, num):
        current = self.head
        while current:
            num += 1
            current = current.next
        return num

    def returnN(self, loc):
        current = self.head
        while current.position < loc:
            #print(current.letters)
            current = current.next
        retstr = ''
        for a in current.letters:
            retstr.append(a)
        return retstr

    def remove(self, current, rem):
        if rem == False:
            #print("false")
            return current.next
        else:
            if current.prev == None:
                self.head = current.next
                if current.next is not None:
                    current.next.prev = None
            else:
                current.prev.next = current.next
                if current.next is not None:
                    current.next.prev = current.prev
        return current.next

    def removeL(self, pos, ent, res, freq):
        rem = False
        current = self.head
        while current:
            rem = False
            if res == '-':
                for i in range(len(current.letters)):
                    #print('-', current.letters[i])
                    if current.letters[i] == ent:
                        rem = True
            if res == '*':
                rem = True
                rem2 = False
                for i in range(n):
                    #print('*', current.letters[i])
                    if current.letters[i] == ent:
                        rem = False
                        if i == pos:
                            rem2 = True
                if rem2 == True:
                    rem = True
            if res.islower() and current.letters[pos] != ent:
                rem = True
                #print('islower', ent, current.letters[pos])
            #if current.letters.all == 'alter':
                #print('Not deleted')
            if rem == True:
                loc = 0
                for letter in current.letters:
                    rep = 0
                    for i in range(0, loc):
                        if letter == current.letters[i]:
                            rep += 1
                    freq[current.letters[loc]][loc] -= 1
                    freq[current.letters[loc]][3 * n] -= 1
                    if rep > 0:
                        freq[current.letters[loc]][loc + n] -= 1
                        freq[current.letters[loc]][3 * n + 1] -= 1
                    if rep > 1:
                        freq[current.letters[loc]][loc + 2 * n] -= 1
                        freq[current.letters[loc]][3 * n + 2] -= 1
                    print(rep)
                    loc += 1
            current = self.remove(current, rem)
        return freq

# Creat an array of all the words.
file = open("Words.txt")
rfile = file.read()
#print(rfile[0:n])
#print(rfile[6:10])
#print(rfile[len(rfile) - n:len(rfile) - 1])

# Create a hash table of "scores" for each letter at each spot for each letter.
words = WordsList()
freq = {}
abc = string.ascii_lowercase
for a in abc:
    freq[a] = np.zeros(3 * n + 3)

# Print starting message
print("This code will estimate the best word to enter.")
print("An \"-\" indicates a letter not in the word, a \"*\" is in the wrong place,")
print("and outputting the letter itself that it is correct.")

# Add all letters to the hash table
for i in range(len(rfile)):
    #print(i, rfile[i])
    if i % (n + 1) == 0:
        read = np.empty(shape = 5, dtype = str)
        for j in range(n):
            read[j] = rfile[i + j]
        words.append(read)
    if (i + 1) % (n + 1) != 0:
        rep = 0
        for j in range(i - (i % (n + 1)), i):
            #print(j, ' ', rfile[j])
            if rfile[j] == rfile[i]:
                rep += 1
                #print("REP", i, ' ', j, rep)
        freq[rfile[i]][i % n] += 1
        freq[rfile[i]][3 * n] += 1
        if rep > 0:
            freq[rfile[i]][n + i % n] += 1
            freq[rfile[i]][3 * n + 1] += 1
        if rep > 1:
            freq[rfile[i]][2 * n + i % n] += 1
            freq[rfile[i]][3 * n + 2] += 1
        # This code assumes letters are not repeated more than 3 times.
        # It could have to be adjusted for other versions of Wordle.


#print('f', freq['f'][:])
#words.displayAll()
#print(words.nWords(words.head, 0))
#print(words.returnL())
#print(words.findPosition(100).returnL())

# Find out which word has the highest score
loc = words.highScore(freq, 1)
#print(loc)
test = words.findPosition(loc).returnL()
print(test)
valid = False

# Obtain results and determine which letters to remove.
while valid == False:
    valid = True
    result = input("Please enter the results of this wordle guess:\n")
    for i in range(n * 2 - 1):
        if ((i % 2) == 0) & (valid == True) & (result[i] != test[int(i / 2)]) & (result[i] != '*') & (result[i] != '-'):
            valid = False
            print("n")
        if ((i & 2) == 1) & (valid == True) & (result[i] != ' '):
            valid = False
            print("space ", i)
    if valid == False:
        print("Please provide a correct input of letters")

# Return results to program
for i in range(n):
    freq = words.removeL(i, test[i], result[i * 2], freq)
#words.displayAll()
#print(words.nWords(words.head, 0))
#for a in abc:
    #print(freq[a][15:18])
