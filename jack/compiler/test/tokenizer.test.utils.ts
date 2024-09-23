import { Token, TokenType } from "../src/custom_impl/token";
import Tokenizer from "../src/custom_impl/tokenizer";
import TokenizerError from "../src/custom_impl/tokenizer.error";

export function testTokenizer(input: string, expected: Token[] | TokenizerError[]): void {
    expect(new Tokenizer(input).tokenize()).toEqual(expected);
}
export function testBasicOperation(op: string, expected: TokenType): void {
    testTokenizer(`a ${op} b`,
        [
            new Token(TokenType.Identifier, 1, 'a'),
            new Token(expected, 1),
            new Token(TokenType.Identifier, 1, 'b'),
            new Token(TokenType.EOF, 1),
        ]);
}



export const keywords: [string, TokenType][] = [
    ['class', TokenType.class],
    ['method', TokenType.method],
    ['function', TokenType.function],
    ['constructor', TokenType.constructor],

    ['int', TokenType.int],
    ['boolean', TokenType.boolean],
    ['char', TokenType.char],
    ['void', TokenType.void],

    ['var', TokenType.var],
    ['static', TokenType.static],
    ['field', TokenType.field],

    ['let', TokenType.let],
    ['do', TokenType.do],
    ['if', TokenType.if],
    ['else', TokenType.else],
    ['while', TokenType.while],
    ['return', TokenType.return],
    ['this', TokenType.this],
];
export const symbols: [string, TokenType][] = [
    ['(', TokenType.LeftParen],
    [')', TokenType.RightParen],
    ['[', TokenType.LeftSquareBracket],
    [']', TokenType.RightSquareBracket],
    ['{', TokenType.LeftBrace],
    ['}', TokenType.RightBrace],

    [',', TokenType.Comma],
    [';', TokenType.Semicolon],
    ['=', TokenType.Equals],
    ['.', TokenType.PeriodDot],

    ['+', TokenType.Plus],
    ['-', TokenType.Minus],
    ['*', TokenType.Multiply],
    ['\\', TokenType.Divide],

    ['&', TokenType.And],
    ['|', TokenType.Or],
    ['~', TokenType.Tilde],
    ['<', TokenType.LessThan],
    ['>', TokenType.GreaterThan]
];
