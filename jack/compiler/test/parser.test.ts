import { JackLexer } from "../src/generated/JackLexer";
import { JackParser } from "../src/generated/JackParser";
import { SymbolTableListener } from "../src/symbol.table.istener";
import { Visitor } from "../src/VIsitor";
import { keywords } from "./tokenizer.test.utils";
import { readFileSync } from 'fs';
import { ParseTreeWalker } from 'antlr4ts/tree/ParseTreeWalker'

import { CommonTokenStream, BufferedTokenStream, CharStreams } from 'antlr4ts';
import path from "path";
import { ErrorListener } from "../src/error.listener";
describe('Parser', () => {
    test('Basic', () => {
        const p = `resources${path.sep}Main.jack`
        var filePath = path.join(__dirname, p);
        const f = readFileSync(filePath, 'utf8');
        // console.log("File", f)


        // console.log(f);
        // Create the lexer and parser
        const inputStream = CharStreams.fromString(f);
        const lexer = new JackLexer(inputStream);
        const errorListener = ErrorListener.getInstance();
        errorListener.filepath = filePath;
        lexer.removeErrorListeners();
        lexer.addErrorListener(errorListener);

        const tokenStream = new CommonTokenStream(lexer);
        // const tokens = tokenStream.getTokens()
        // console.log("Tokens ", )
        expect(tokenStream.getTokens.length).toBeGreaterThan(0)
        const parser = new JackParser(tokenStream);

        parser.removeErrorListeners();
        parser.addErrorListener(errorListener);
        
        // Parse the input, where `compilationUnit` is whatever entry point you defined
        const tree = parser.program();
        expect(errorListener.error).toBe(false)
        console.log(tree.toStringTree(parser.ruleNames));
       
        // Create the visitor
        // const v = new Visitor();
        // // Use the visitor entry point
        // const res = v.visit(tree);
        // console.log("Res", res);
        const l = new SymbolTableListener();
        ParseTreeWalker.DEFAULT.walk(l, tree);
    });


});