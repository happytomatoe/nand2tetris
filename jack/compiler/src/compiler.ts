import path from "path";
import { BinderListener } from "./listener/binder.listener";
import fs from "fs";
import { ParseTreeWalker } from "antlr4ts/tree/ParseTreeWalker";
import { JackParser } from "./generated/JackParser";
import { CharStreams, CommonTokenStream } from "antlr4ts";
import { JackLexer } from "./generated/JackLexer";
import { ErrorListener } from "./listener/error.listener";
import { ValidatorListener } from "./listener/validator.listener";
import { JackCompilerError } from "./error";
import { VMWriter } from "./listener/vm.writer.listener";
export class Compiler {
    static compile(src: string): string | JackCompilerError[] {

        const binder = new BinderListener();
        const errorListener = new ErrorListener();
        const lexer = new JackLexer(CharStreams.fromString(src));
        if (errorListener) {
            lexer.removeErrorListeners();
            lexer.addErrorListener(errorListener);
        }
        const tokenStream = new CommonTokenStream(lexer);
        const parser = new JackParser(tokenStream);
        // parser.isTrace = trace;
        if (errorListener != undefined) {
            parser.removeErrorListeners();
            parser.addErrorListener(errorListener);
        }
        const tree = parser.program();
        if (errorListener.errors.length > 0) {
            console.log("Errors when parsing or lexing");
            return errorListener.errors;
        }
        // console.log(tree.toStringTree(parser.ruleNames));

        ParseTreeWalker.DEFAULT.walk(binder, tree);
        if (binder.errors.length > 0) {
            console.log("Errors in binder");
            return binder.errors;
        }
        const validator = new ValidatorListener(binder.globalSymbolTable);
        ParseTreeWalker.DEFAULT.walk(validator, tree);
        if (validator.errors.length > 0) {
            console.log("Errors in validator " + JSON.stringify(validator.errors));
            return validator.errors;
        }
        const vmWriter = new VMWriter(binder.globalSymbolTable);
        ParseTreeWalker.DEFAULT.walk(vmWriter, tree);
        if (vmWriter.errors.length > 0) {
            return vmWriter.errors;
        }
        return vmWriter.result;
    }
}