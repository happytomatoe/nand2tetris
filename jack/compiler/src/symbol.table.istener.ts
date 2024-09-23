import { ParseTreeListener } from "antlr4ts/tree/ParseTreeListener";
import { ClassDeclarationContext, ClassVarDecContext, ConstructorContext, FunctionContext, JackParser, MethodContext, StaticFieldDeclarationContext, SubroutineDecContext, SubroutineDecWithoutTypeContext, SubroutineReturnTypeContext, VarDecContext, VarTypeContext } from "./generated/JackParser";
import { JackParserListener } from "./generated/JackParserListener";
import { TerminalNode } from "antlr4ts/tree/TerminalNode";

export class SymbolTableListener implements JackParserListener, ParseTreeListener {

    // fix compiler error
    visitTerminal?: (/*@NotNull*/ node: TerminalNode) => void;

    enterClassDeclaration(ctx: ClassDeclarationContext) {
        console.log(`Entering class ${ctx.className()?.text}`);
    };
    enterStaticFieldDeclaration(ctx: StaticFieldDeclarationContext) {
        const fieldList = ctx.fieldList();
        console.log(`static field: ${fieldList.varType().text} ${fieldList.fieldName().map(t => t.text).join(",")}`);
    };

    enterVarDec(ctx: VarDecContext) {
        const names = ctx.IDENTIFIER();
        console.log(`Function var: ${this.getType(ctx.varType())} ${names.join(",")}`);
    };
    enterConstructor(ctx: ConstructorContext) {
        this.printSubroutine("Ctor", ctx.subroutineDecWithoutType())
    };
    enterMethod(ctx: MethodContext) {
        this.printSubroutine("Method", ctx.subroutineDecWithoutType())
    };
    enterFunction(ctx: FunctionContext) {
        this.printSubroutine("Function", ctx.subroutineDecWithoutType())
    };

    printSubroutine(type: string, c: SubroutineDecWithoutTypeContext) {
        const returnType = this.getSubroutineReturnType(c.subroutineReturnType())
        const paramList = c.parameterList().parameter().map(t => t.varType().text + " " + t.parameterName().text).join(",");
        console.log(`${type}: ${returnType} ${c.subroutineName().text} (${paramList})`);
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
