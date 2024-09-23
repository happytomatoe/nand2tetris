import { ParseTreeListener } from "antlr4ts/tree/ParseTreeListener";
import { ClassDeclarationContext, ClassVarDecContext, SubroutineDecContext, SubroutineReturnTypeContext, VarDecContext, VarTypeContext } from "./generated/JackParser";
import { JackParserListener } from "./generated/JackParserListener";
import { TerminalNode } from "antlr4ts/tree/TerminalNode";

export class SymbolTableListener implements JackParserListener, ParseTreeListener {

    // fix compiler error
    visitTerminal?: (/*@NotNull*/ node: TerminalNode) => void;

    enterClassDeclaration(ctx: ClassDeclarationContext) {
        console.log(`Entering class ${ctx.className()?.text}`);
    };
    enterClassVarDec(ctx: ClassVarDecContext) {
        const names = ctx.IDENTIFIER();
        console.log(`class field: ${this.getType(ctx.varType())} ${names.join(",")}`);
    };

    enterVarDec(ctx: VarDecContext) {
        const names = ctx.IDENTIFIER();
        console.log(`Function var: ${this.getType(ctx.varType())} ${names.join(",")}`);
    };
    enterSubroutineDec(ctx: SubroutineDecContext) {
        const subroutineType = ctx.CONSTRUCTOR() ? "constructor" :
            ctx.FUNCTION() ? "function" :
                ctx.METHOD() ? "method" : "";
        const subroutineReturnType = this.getSubroutineReturnType(ctx.subroutineReturnType());
        if (subroutineType === "") throw new Error(`Undefined subroutine type`)
        console.log(`Subroutine: ${subroutineType} ${subroutineReturnType} ${ctx.subroutineName()?.text} `);
    }



    getType(ctx: VarTypeContext): string {
        return ctx.BOOLEAN() ? "boolean" :
            ctx.INT() ? "int" :
                ctx.CHAR() ? "char" :
                    ctx.IDENTIFIER() ? ctx.IDENTIFIER()!.text :
                        "unknown";
    }

    getSubroutineReturnType(ctx: SubroutineReturnTypeContext): string {
        return ctx.VOID() ? "void" : this.getType(ctx.varType()!);
    }
}
