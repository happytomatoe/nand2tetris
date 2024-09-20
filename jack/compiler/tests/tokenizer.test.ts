import Tokenizer from '../src/Tokenizer';
import { TokenType, Token } from '../src/Token';

import * as fs from 'fs'; 
import * as path from 'path';
describe('testing index file', () => {
  test('basic', () => {
    const l = new Tokenizer("a + b");
    const expected =
      [
        new Token(TokenType.Identifier, 1, 'a'),
        new Token(TokenType.Plus, 1),
        new Token(TokenType.Identifier, 1, 'b'),
        new Token(TokenType.EOF, 1),
      ]
    const actual = l.tokenize();
    expect(actual).toEqual(expected);
  });

  test('long identifier', () => {
    const l = new Tokenizer("this_is_a_long_identifier");
    const expected =
      [
        new Token(TokenType.Identifier, 1, 'this_is_a_long_identifier'),
        new Token(TokenType.EOF, 1),
      ]
    const actual = l.tokenize();
    expect(actual).toEqual(expected);
  });
    import * as path from 'path';

    test("main.jack", () => {
      const file = path.join(__dirname, "./", "bla.txt");
      const f = fs.readFileSync('Main.jack', 'utf8');
      console.log(f.toString());
    });
});
