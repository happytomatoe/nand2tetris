import { BinderListener } from "./listener/binder.listener";
import { CustomErrorListener } from "./listener/error.listener";
import { ValidatorListener } from "./listener/validator.listener";
import { JackCompilerError } from "./error";
import { VMWriter } from "./listener/vm.writer.listener";
import JackParser, { ProgramContext } from "./generated/JackParser";
import { CharStreams, CommonTokenStream, ParseTreeWalker } from "antlr4";
import JackLexer from "./generated/JackLexer";
export class Compiler {
    private binder = new BinderListener();
    private errorListener = new CustomErrorListener();
    //TODO: add compile for many files. This should concat the files in the end
    compile(tree: ProgramContext): string | JackCompilerError[] {
        if (Object.keys(this.binder.globalSymbolTable).length == 0) {
            throw new Error("Please populate global symbol table using parserAndBind method")
        }
        const validator = new ValidatorListener(this.binder.globalSymbolTable);
        ParseTreeWalker.DEFAULT.walk(validator, tree);
        if (validator.errors.length > 0) {
            console.log("Errors in validator " + JSON.stringify(validator.errors));
            return validator.errors;
        }
        const vmWriter = new VMWriter(this.binder.globalSymbolTable);
        ParseTreeWalker.DEFAULT.walk(vmWriter, tree);
        if (vmWriter.errors.length > 0) {
            return vmWriter.errors;
        }
        return vmWriter.result;
    }



    parserAndBind(src: string): ProgramContext | JackCompilerError[] {
        const lexer = new JackLexer(CharStreams.fromString(src));
        lexer.removeErrorListeners();
        lexer.addErrorListener(this.errorListener);
        const tokenStream = new CommonTokenStream(lexer);
        const parser = new JackParser(tokenStream);
        // parser.isTrace = trace;
        parser.removeErrorListeners();
        parser.addErrorListener(this.errorListener);
        const tree = parser.program();
        if (this.errorListener.errors.length > 0) {
            console.log("Errors when parsing or lexing");
            return this.errorListener.errors;
        }
        // console.log(tree.toStringTree(parser.ruleNames));

        ParseTreeWalker.DEFAULT.walk(this.binder, tree);
        if (this.binder.errors.length > 0) {
            console.log("Errors in binder");
            return this.binder.errors;
        }
        return tree;
    }
}