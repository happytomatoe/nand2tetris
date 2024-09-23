import TokenizerError from './TokenizerError';
import { Token, TokenType } from './Token';
export default class Tokenizer {
    code: string;
    current: number = 0;
    line: number = 1;
    tokens: Token[] = [];
    errors: TokenizerError[] = [];
    whitespaceChars = [' ', '\r', '\t'];

    constructor(code: string) {
        this.code = code;
    }
    tokenize(): Token[] | TokenizerError[] {
        while (!this.isAtEnd()) {
            const c = this.advance();
            switch (c) {
                case '\n':
                    this.line++;
                    break;
                case '/':
                    if (this.match('/')) {
                        while (this.peek() !== '\n') {
                            this.advance();
                        }
                    } else {
                        this.unexpectedCharError(this.code[this.current]);
                    }
                    break;
                case '(':
                    this.addToken(TokenType.LeftParen);
                    break;
                case ')':
                    this.addToken(TokenType.RightParen);
                    break;
                case '{':
                    this.addToken(TokenType.LeftBrace);
                    break;
                case '}':
                    this.addToken(TokenType.RightBrace);
                    break;
                case '[':
                    this.addToken(TokenType.LeftSquareBracket);
                    break;
                case ']':
                    this.addToken(TokenType.RightSquareBracket);
                    break;
                case ',':
                    this.addToken(TokenType.Comma);
                    break;
                case ';':
                    this.addToken(TokenType.Semicolon);
                    break;
                case '=':
                    this.addToken(TokenType.Equals);
                    break;
                case '.':
                    this.addToken(TokenType.PeriodDot);
                    break;
                case '+':
                    this.addToken(TokenType.Plus);
                    break;
                case '-':
                    this.addToken(TokenType.Minus);
                    break;
                case '*':
                    this.addToken(TokenType.Multiply);
                    break;
                case '\\':
                    this.addToken(TokenType.Divide);
                    break;
                case '&':
                    this.addToken(TokenType.And);
                    break;
                case '|':
                    this.addToken(TokenType.Or);
                    break;
                case '~':
                    this.addToken(TokenType.Tilde);
                    break;
                case '<':
                    this.addToken(TokenType.LessThan);
                    break;
                case '>':
                    this.addToken(TokenType.GreaterThan);
                    break;
                case ':':
                    this.addToken(TokenType.Colon);
                    break;
                //TODO: check if we can have 2 operators together
                //I assume we don't

                //literals 
                case '"':
                    this.string();
                    break;

                default:
                    if (this.whitespaceChars.indexOf(c) != -1) {
                        break
                    } else if (isDigit(c)) {
                        this.number();
                        break;
                    } else if (isAlpha(c)) {
                        this.identifier();
                        break;
                    } else {
                        this.unexpectedCharError('Unexpected character ' + this.code[this.current]);
                        break;
                    }
            }
        }
        this.addToken(TokenType.EOF);
        return this.errors.length > 0 ? this.errors : this.tokens;
    }
    identifier() {
        const start = this.current - 1;
        while (isAlphaNumberic(this.peek())) {
            this.advance();
        }
        const text = this.code.slice(start, this.current);
        console.log('Identifier:', text);
        if (keywords.has(text)) {
            this.addToken(keywords.get(text)!);
        } else {
            this.addToken(TokenType.Identifier, text);
        }
    }
    number() {
        const start = this.current;
        while (isDigit(this.peek())) {
            this.advance();
        }
        this.addToken(TokenType.NumberConstant, this.code.slice(start, this.current));

    }
    string() {
        const start = this.current;
        while (this.peek() != '"' && !this.isAtEnd()) {
            if (this.peek() == '\n') this.line++;
            this.advance();
        }
        if (this.isAtEnd()) {
            this.unexpectedCharError('Unterminated string');
        }
        this.advance();
        const value = this.code.slice(start + 1, this.current - 1);
        this.addToken(TokenType.StringConstant, value);
    }

    peek() {
        if (this.isAtEnd()) return '\0';
        return this.code[this.current];
    }

    advance() {
        return this.code[this.current++];
    }
    addToken(type: TokenType, literal?: string) {
        this.tokens.push(new Token(type, this.line, literal));
    }

    isAtEnd(): boolean {
        return this.current >= this.code.length;
    }
    match(expected: string): boolean {
        if (this.isAtEnd()) return false;
        if (this.code[this.current] !== expected) return false;
        this.current++;
        return true;
    }

    unexpectedCharError(message: string) {
        this.errors.push(new TokenizerError(this.line, 'Unexpected character `' +message +"`"));
    }
}

function isDigit(c: string): boolean {
    return c >= '0' && c <= '9';
}

function isAlpha(c: string) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c === '_';
}

function isAlphaNumberic(c: string) {
    const a = isDigit(c);
    const d = isAlpha(c);
    return a || d;
}

const keywords = new Map<string, TokenType>([
    ["class", TokenType.class],
    ["constructor", TokenType.constructor],
    ["method", TokenType.method],
    ["function", TokenType.function],
    ["int", TokenType.int],
    ["boolean", TokenType.boolean],
    ["char", TokenType.char],
    ["void", TokenType.void],
    ["var", TokenType.var],
    ["static", TokenType.static],
    ["field", TokenType.field],
    ["let", TokenType.let],
    ["do", TokenType.do],
    ["if", TokenType.if],
    ["else", TokenType.else],
    ["while", TokenType.while],
    ["return", TokenType.return],
    ["true", TokenType.true],
    ["false", TokenType.false],
    ["null", TokenType.null],
    ["this", TokenType.this],
]);