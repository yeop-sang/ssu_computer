class ParseException(Exception):
    def __init__(self, s: str) -> None:
        super().__init__(s)

class RDParser:
    input = ""
    idx = 0

    def parser(self, str):
        self.input = str + "$"
        self.idx = 0
        if self.input[self.idx - 1] != '$':
            raise ParseException()
        print(self.expr())

    def ffSpace(self):
        while(self.input[self.idx] == ' '):
            self.idx += 1

    def digit(self):
        try:
            return int(self.input[self.idx])
        except Valueerror:
            return -1

    def number(self):
        result = 0
        temp_digit = 0

        self.ffSpace()

        while (True):
            temp_digit = self.digit()
            if temp_digit < 0:
                break

            result = result * 10 + temp_digit
            self.idx += 1

        return result

    def factor(self):
        result = 0

        self.ffSpace()
        # TODO: expr 적용
        if self.input[self.idx] == '(':
            self.idx += 1
            self.ffSpace()
            result = self.expr()
            self.ffSpace()
            if self.input[self.idx] != ')':
                raise ParseException()
            self.idx += 1
        else:
            if self.input[self.idx] == '-':
                result = -1
                self.idx += 1
            else:
                result = 1

            result = result * self.number()
        return result

    def term(self):
        result = self.factor()

        self.ffSpace()

        while(True):
            operator = self.input[self.idx]

            if operator == '*' or operator == '/':
                self.idx += 1
                self.ffSpace()
                if operator == '*':
                    result *= self.factor()
                elif operator == '/':
                    result /= self.factor()
            else:
                break

        return result

    def expr(self):
        result = self.term()

        self.ffSpace()

        while(True):
            operator = self.input[self.idx]

            if operator == '+' or operator == '-':
                self.idx += 1
                self.ffSpace()

                if operator == '+':
                    result += self.term()
                elif operator == '-':
                    result -= self.term()
            
            else:
                break
        
        return result

p = RDParser()
while(True):
    try:
        p.parser(input(">>"))
    except ParseException:
        print('syntax error!!')
