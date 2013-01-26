import sys

class Lexer(object):
    def __init__(self, file_name):
        self.syntax = list('{}()[];,:.@')
        self.operators = list('+-*/=<>!&|^~%')
        self.source = open(file_name, 'r')

    def get_next(self):
        self.current = self.source.read(1)
        return self.current

    def match(self):
        # Just an alias. Nothing to see here.
        return self.get_next()

    @property
    def tokens(self):
        tokens = []
        self.get_next()
        while self.current:  # current will be '' when done
            if self.current.isspace():
                while self.current.isspace():  # om nom nom white space
                    self.get_next()
                continue
            elif self.current == '#':
                while self.current != '\n':
                    self.get_next()
                continue
            yield self.get_token()

    def get_token(self):
        if self.current.isdigit():
            return self.parse_number()
        elif self.current in (r'"', r"'"):
            return self.parse_string()
        elif self.current in self.operators:
            return self.parse_operator()
        elif self.current in self.syntax:
            return self.parse_syntax()
        elif self.current.isalpha() or self.current == '_':
            return self.parse_id()
        else:
            raise RuntimeError('Unknown token: %s' % self.current)

    def parse_number(self):
        token = ''
        while self.current.isdigit():
            token += self.current
            self.match()
        return '<number, %s>' % token

    def parse_string(self):
        # Doesn't handle embedded quotes of the same type
        token = ''
        quote_type = self.current
        self.match()
        while self.current and self.current != quote_type:
            token += self.current
            self.match()
        if self.current:
            self.match()
        return '<string, %s>' % token

    def parse_operator(self):
        token = self.current
        self.match()
        return '<op, %s>' % token

    def parse_syntax(self):
        token = self.current
        self.match()
        return '<syntax, %s>' % token

    def parse_id(self):
        token = ''
        while self.current.isalnum() or self.current == '_':
            token += self.current
            self.match()
        return '<id, %s>' % token


def main():
    if len(sys.argv) < 2:
        print >> sys.stderr, 'usage: python lex.py FILE'
        sys.exit(2)

    lex = Lexer(sys.argv[1])
    for token in lex.tokens:
        print token

if __name__ == '__main__':
    main()
