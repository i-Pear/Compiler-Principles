import re

bound1 = ['\'', '\"', '\\?', ';', '::', '\\[', '\\]', '\\{', '\\}', ',', '\\.', '\\|\\|', '&&', '\\(',
          '\\)', '~', '\\^=', '&=', '\\|=', '\\+=', '-=', '\\*=', '/=', '<=', '>=', '!=', '%=', '==']
bound2 = [':', '\\^', '&', '\\|', '\\+', '-', '\\*', '/', '<', '>', '!', '%', '=', ' ']
bounds = ['\'', '\"', '?', ';', ':', '::', '[', ']', '{', '}', ',', '.', '||', '&&', '(', ')', '~', '^=', '&=', '|=',
          '+=', '-=', '*=', '/=', '<=', '>=', '!=', '%=', '==', '^', '&', '|', '+', '-', '*', '/', '<', '>', '!', '%',
          '=']
regex1 = '(' + '|'.join(bound1) + ')'
regex2 = '(' + '|'.join(bound2) + ')'
keywords = 'auto break case char const continue default do double else enum extern float for goto if int long register return short signed static sizeof struct switch typedef union unsigned void volatile while'.split()

boundSym = {}


def initBoundSymbols():
    count = 1
    for sym in bounds:
        boundSym[sym] = count
        count += 1


keywordSym = {}


def initKeywordSymbols():
    count = 1
    for sym in keywords:
        keywordSym[sym] = count
        count += 1


words = []
labels = {}
numbers = {}


def splitWords():
    source = open("source.txt", encoding='UTF-8')
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
    source.close()


tokens = []


class Token:
    cls: int
    id: str
    content: object

    def __init__(self, type: str, id, content):
        self.content = content
        if type == 'label':
            self.cls = 1
            self.id = 'I' + str(id)
        elif type == 'num':
            self.cls = 2
            self.id = 'C' + str(id)
        elif type == "keyword":
            self.cls = 2 + id
            self.id = '-'
        else:
            self.cls = len(keywordSym.keys()) + 2 + id
            self.id = '-'

    def __repr__(self):
        return "{0:15s} => {1}".format('<{0}, {1}>'.format(self.cls, str(self.id)), self.content)


def getTokens():
    for item in words:
        if item in boundSym.keys():
            tokens.append(Token('bound', boundSym[item], item))
        elif item in keywordSym.keys():
            tokens.append(Token('keyword', keywordSym[item], item))
        elif '0' <= item[0] <= '9':
            if item not in numbers:
                numbers[item] = len(numbers.keys()) + 1
            tokens.append(Token('num', numbers[item], item))
        else:
            if item not in labels:
                labels[item] = len(labels.keys()) + 1
            tokens.append(Token('label', labels[item], item))


def writeToFile():
    out = open("result.txt", encoding='UTF-8', mode='w')
    for token in tokens:
        out.write(str(token))
        out.write('\n')
    out.close()


if __name__ == '__main__':
    initBoundSymbols()
    initKeywordSymbols()
    splitWords()
    getTokens()
    writeToFile()
