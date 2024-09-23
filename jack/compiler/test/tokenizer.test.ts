import { Token, TokenType } from '../src/custom_impl/token';
import TokenizerError, { ErrorMessages } from '../src/custom_impl/tokenizer.error';
import { keywords, symbols, testTokenizer, } from './tokenizer.test.utils';

describe('Tokenizer', () => {

  //comments
  test('single line comment', () => {
    testTokenizer("// this is a comment", [new Token(TokenType.EOF, 1)]);
  });

  test('multi-line comment', () => {
    testTokenizer("/* this is a comment */", [new Token(TokenType.EOF, 1)]);
  });

  test('multi-line comment 2', () => {
    testTokenizer("/* this is a comment", [new Token(TokenType.EOF, 1)]);
  });

  test('multi-line comment 3', () => {
    testTokenizer("/* this is a comment *", [new Token(TokenType.EOF, 1)]);
  });

  test('multi-line comment 4', () => {
    testTokenizer("/* this is a comment \n*", [new Token(TokenType.EOF, 1),]);
  });

  test('api documentation comment', () => {
    testTokenizer("/** this is a comment **/",
      [
        new Token(TokenType.EOF, 1),
      ]);
  });

  test('api documentation comment 2', () => {
    testTokenizer("/** this is a comment",
      [
        new Token(TokenType.EOF, 1),
      ]);
  });
  test('api documentation comment 3', () => {
    testTokenizer("/** this is a comment \n*",
      [
        new Token(TokenType.EOF, 1),
      ]);
  });

  test('api documentation comment 4', () => {
    testTokenizer("/** this is a comment \n x=2",
      [
        new Token(TokenType.EOF, 1),
      ]);
  });


  test('api documentation comment multi-line', () => {
    testTokenizer("/** this \n is \n a comment  **/",
      [
        new Token(TokenType.EOF, 1),
      ]);
  });

  //keyword
  test.each(keywords)('%s keyword', (keyword, expected) => {
    testTokenizer(keyword,
      [
        new Token(expected, 1),
        new Token(TokenType.EOF, 1),
      ]);
  });
  //test colon, comma, semicolon, equals, period, leftParen, rightParen, leftSquareBracket, rightSquareBracket, leftBrace, rightBrace
  test.each(symbols)("symbol %s", (symbol, expected) => {
    testTokenizer(
      " " + symbol + " ",
      [
        new Token(expected, 1),
        new Token(TokenType.EOF, 1),
      ]);
  })

  const literals: [string, Token][] = [
    ["null", new Token(TokenType.NullConstant, 1)],
    ["013543342", new Token(TokenType.NumberConstant, 1, 13543342)],
    ["true", new Token(TokenType.BooleanConstant, 1, true)],
    ["false", new Token(TokenType.BooleanConstant, 1, false)]
  ]

  test.each(literals)('literal %s', (token, expected) => {
    testTokenizer(token, [expected, new Token(TokenType.EOF, 1)]);
  });
  test("unterminated string", () => {
    testTokenizer('"dsada', [new TokenizerError(1, ErrorMessages.unterminatedString)]);
  })

  test("multiline string", () => {
    testTokenizer('\n\n"dsada\n"', [new TokenizerError(3, ErrorMessages.unterminatedString)]);
  })

  test("multiline string 2", () => {
    testTokenizer(` "dsadadsa
      dsadsads" ;
      $;
      let x =2;#
      `, [
      new TokenizerError(1, ErrorMessages.unterminatedString),
      TokenizerError.unexpectedToken(3, "$"),
      TokenizerError.unexpectedToken(4, "#"),
    ]);
  })

  //test negative numbers
  test('negative numbers', () => {
    testTokenizer("-123",
      [
        new Token(TokenType.Minus, 1),
        new Token(TokenType.NumberConstant, 1, 123),
        new Token(TokenType.EOF, 1),
      ]);
  });

  //identifiers
  test('valid identifier ', () => {
    const alphabet = 'abcdefghijklmnopqrstuvwxyz';
    const digits = '0123456789';
    const s = alphabet + "_" + digits + "_" + alphabet.toUpperCase();
    testTokenizer(s, [new Token(TokenType.Identifier, 1, s), new Token(TokenType.EOF, 1)]);
  });

  //TODO: Parser should fail for this case
  test('test identifiers with numbers', () => {
    const s = '123abc';
    testTokenizer(s, [new Token(TokenType.NumberConstant, 1, 123), new Token(TokenType.Identifier, 1, "abc"), new Token(TokenType.EOF, 1)]);
  });

  test('test identifiers with special characters', () => {
    const s = 'a_b$c';
    testTokenizer(s, [TokenizerError.unexpectedToken(1, '$')]);
  });

  test('test assignment to a constant', () => {
    testTokenizer("let x=0;",
      [
        new Token(TokenType.let, 1),
        new Token(TokenType.Identifier, 1, 'x'),
        new Token(TokenType.Equals, 1),
        new Token(TokenType.NumberConstant, 1, 0),
        new Token(TokenType.Semicolon, 1),
        new Token(TokenType.EOF, 1),
      ]);
  });


  //test error handling for unexpected characters, invalid syntax, and invalid numbers, strings, and boolean values
  test('error handling', () => {

    testTokenizer("$ ; \n 123abc ; \n true1; \n false_; \n %",
      [
        TokenizerError.unexpectedToken(1, '$'),
        TokenizerError.unexpectedToken(5, '%'),
      ]);
  });


  //test error handling for invalid identifier names
  test('invalid identifiers', () => {
    testTokenizer("$ ; \n 123abc ; \n true1; \n false_; \n %",
      [
        TokenizerError.unexpectedToken(1, '$'),
        TokenizerError.unexpectedToken(5, '%'),
      ]);
  });

  //expressions
  test('arithmetic expressions', () => {
    testTokenizer("1 + 2 * 3",
      [
        new Token(TokenType.NumberConstant, 1, 1),
        new Token(TokenType.Plus, 1),
        new Token(TokenType.NumberConstant, 1, 2),
        new Token(TokenType.Multiply, 1),
        new Token(TokenType.NumberConstant, 1, 3),
        new Token(TokenType.EOF, 1),
      ]);
  });

  test('simple comment', () => {
    testTokenizer("//1 + 2 * 3", [new Token(TokenType.EOF, 1)]);
  });
  test('boolean expressions', () => {
    testTokenizer("true & false | true",
      [
        new Token(TokenType.BooleanConstant, 1, true),
        new Token(TokenType.And, 1),
        new Token(TokenType.BooleanConstant, 1, false),
        new Token(TokenType.Or, 1),
        new Token(TokenType.BooleanConstant, 1, true),
        new Token(TokenType.EOF, 1)
      ]);
  });

  test('test function call', () => {
    testTokenizer("do Screen.setColor(false);",
      [
        new Token(TokenType.do, 1),
        new Token(TokenType.Identifier, 1, 'Screen'),
        new Token(TokenType.PeriodDot, 1),
        new Token(TokenType.Identifier, 1, 'setColor'),
        new Token(TokenType.LeftParen, 1),
        new Token(TokenType.BooleanConstant, 1, false),
        new Token(TokenType.RightParen, 1),
        new Token(TokenType.Semicolon, 1),
        new Token(TokenType.EOF, 1),
      ]);
  });

  test('reserved keyword in identifier', () => {
    testTokenizer("doScreen;",
      [
        new Token(TokenType.Identifier, 1, 'doScreen'),
        new Token(TokenType.Semicolon, 1),
        new Token(TokenType.EOF, 1),
      ]);
  });
  
  test('test function declaration', () => {
    testTokenizer("functionvoid;",
      [
        new Token(TokenType.Identifier, 1, 'functionvoid'),
        new Token(TokenType.Semicolon, 1),
        new Token(TokenType.EOF, 1),
      ]);
  });
  test('field declaration', () => {
    testTokenizer("field int x,y;",
      [
        new Token(TokenType.field, 1),
        new Token(TokenType.int, 1),
        new Token(TokenType.Identifier, 1, 'x'),
        new Token(TokenType.Comma, 1),
        new Token(TokenType.Identifier, 1, 'y'),
        new Token(TokenType.Semicolon, 1),
        new Token(TokenType.EOF, 1),
      ]);
  });

  // Arithmetic expressions with parentheses test
  test('arithmetic expressions with parentheses', () => {
    testTokenizer("1 + (2 * 3)",
      [
        new Token(TokenType.NumberConstant, 1, 1),
        new Token(TokenType.Plus, 1),
        new Token(TokenType.LeftParen, 1),
        new Token(TokenType.NumberConstant, 1, 2),
        new Token(TokenType.Multiply, 1),
        new Token(TokenType.NumberConstant, 1, 3),
        new Token(TokenType.RightParen, 1),
        new Token(TokenType.EOF, 1),
      ]);
  });

  test('hello world', () => {
    testTokenizer(`
      class Main {
        function void main() {
            /* Prints some text using the standard library. */
            do Output.printString("Hello world!");
            do Output.println();      // New line
            return;
        }
      }`,
      [
        new Token(TokenType.class, 2),
        new Token(TokenType.Identifier, 2, "Main"),
        new Token(TokenType.LeftBrace, 2),

        new Token(TokenType.function, 3),
        new Token(TokenType.void, 3),
        new Token(TokenType.Identifier, 3, "main"),
        new Token(TokenType.LeftParen, 3),
        new Token(TokenType.RightParen, 3),
        new Token(TokenType.LeftBrace, 3),

        new Token(TokenType.do, 5),
        new Token(TokenType.Identifier, 5, "Output"),
        new Token(TokenType.PeriodDot, 5),
        new Token(TokenType.Identifier, 5, "printString"),
        new Token(TokenType.LeftParen, 5),
        new Token(TokenType.StringConstant, 5, "Hello world!"),
        new Token(TokenType.RightParen, 5),
        new Token(TokenType.Semicolon, 5),

        new Token(TokenType.do, 6),
        new Token(TokenType.Identifier, 6, "Output"),
        new Token(TokenType.PeriodDot, 6),
        new Token(TokenType.Identifier, 6, "println"),
        new Token(TokenType.LeftParen, 6),
        new Token(TokenType.RightParen, 6),
        new Token(TokenType.Semicolon, 6),

        new Token(TokenType.return, 7),
        new Token(TokenType.Semicolon, 7),
        new Token(TokenType.RightBrace, 8),
        new Token(TokenType.RightBrace, 9),
        new Token(TokenType.EOF, 9),
      ]);
  });


});
