import re

bound1 = ['\'', '\"', '\\?', ';', ':', '\\[', '\\]', '\\{', '\\}', ',', '\\.', '\\|\\|', '&&', '\\(',
          '\\)', '~', '\\^=', '&=', '\\|=', '\\+=', '-=', '\\*=', '/=', '<=', '>=', '!=', '%=', '==']
bound2 = ['\\^', '&', '\\|', '\\+', '-', '\\*', '/', '<', '>', '!', '%', '=', ' ']
boundSym = bound1 + bound2
regex1 = '(' + '|'.join(bound1) + ')'
regex2 = '(' + '|'.join(bound2) + ')'
keywords = 'auto break case char const continue default do double else enum extern float for goto if int long register return short signed static sizeof struct switch typedef union unsigned void volatile while'.split()

source = open("source.txt", encoding='UTF-8')
out = open("result.txt", encoding='UTF-8', mode='w')

words = []
def splitWords():
    for line in source:
        # erase \n
        line = line[:-1]
        # erase comments
        comment = line.find('//')
        if comment != -1:
            line = line[:comment]
        # split
        sp1 = re.split(regex1, line)

        for item in sp1:
            temp = re.split(regex2, item)
            for word in temp:
                if word.strip() != "":
                    words.append(word)

tokens=[]
def getTokens():
    global tokens,words
    for item in words:
        if item in boundSym:


source.close()
out.close()
