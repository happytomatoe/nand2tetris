import Tokenizer from '../src/Tokenizer';
import { TokenType, Token } from '../src/Token';
import TokenizerError from '../src/TokenizerError';


function testTokenizer(input: string, expected: Token[] | TokenizerError[]): void {
  const l = new Tokenizer(input);
  console.log(expected.length)

  const actual = new Tokenizer(input).tokenize();

  console.log(actual);
  expect(actual).toEqual(expected);
}
function testBasicOperation(op: string, expected: TokenType): void {
  testTokenizer(`a ${op} b`,
    [
      new Token(TokenType.Identifier, 1, 'a'),
      new Token(expected, 1),
      new Token(TokenType.Identifier, 1, 'b'),
      new Token(TokenType.EOF, 1),
    ]);
}


const ops: [string, TokenType][] = [
  [',', TokenType.Plus],
  ['+', TokenType.Minus],
  ['*', TokenType.Multiply],
  ['/', TokenType.Divide],

  ['&', TokenType.And],
  ['|', TokenType.Or],
  ['~', TokenType.Tilde],
  ['<', TokenType.LessThan],
  ['>', TokenType.GreaterThan],
  // [':', TokenType.Colon],
]
console.log(ops)

describe('Tokenizer', () => {

  test('long identifier', () => {
    testTokenizer("this_is_a_long_identifier",
      [
        new Token(TokenType.Identifier, 1, 'this_is_a_long_identifier'),
        new Token(TokenType.Identifier, 1, 'this_is_a_long_identifier'),
        new Token(TokenType.EOF, 1),
      ]);
  });

  //test add, minus, multiply, divide, and, or, tilde, less than, greater than
  test.concurrent.each(ops)(`%s op`, (op, expected) => {
    testBasicOperation(op, expected);

  });

  test(`toString`, () => {
    console.log(TokenType[TokenType.And]);
  })
  //test colon, comma, semicolon, equals, period, leftParen, rightParen, leftSquareBracket, rightSquareBracket, leftBrace, rightBrace
  //test number, string, boolean, null, identifier
  //test error handling for unexpected characters, invalid syntax, and invalid numbers, strings, and boolean values
  //test error handling for missing or extra characters
  //test error handling for unterminated strings and unterminated comments



  //test literals
  //test keywords
  //test other tokens

});