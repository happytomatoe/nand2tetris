import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { JackParserListener } from "../generated/JackParserListener";
import { ClassDeclarationContext, ConstantContext, ExpressionContext, IfElseStatementContext, IfExpressionContext, IfStatementContext, JackParser, LetStatementContext, ReturnStatementContext, SubroutineCallContext, SubroutineDeclarationContext, SubroutineDecWithoutTypeContext, VarDeclarationContext, WhileExpressionContext, WhileStatementContext } from "../generated/JackParser";
import { GenericSymbol } from "../symbol";
import { _notImplemented } from "../error";
import { JackLexer } from "../generated/JackLexer";

const binaryOperationToVmCmd: Record<string, string> = {
    "+": "add",
    "-": "sub",
    "*": "call Math.multiply 2",
    "/": "call Math.divide 2",
    "&": "and",
    "|": "or",
    "<": "lt",
    ">": "gt",
    "=": "eq",
};

const unaryOperationToVmCmd: Record<string, string> = {
    "-": "neg",
    "~": "not",
};

export class VMWriter implements JackParserListener {
    public result: string = ""
    private className: string = "";
    private localVars: string[] = [];
    private currentLabelInd: number = 0;
    constructor(private globalSymbolTable: Record<string, GenericSymbol>) {
    }

    enterClassDeclaration(ctx: ClassDeclarationContext) {
        if (this.className != "") {
            throw new Error("Cannot change class name")
        }
        this.className = ctx.className()!.text;
    };

    enterSubroutineDeclaration(ctx: SubroutineDeclarationContext) {
        const name = ctx.subroutineDecWithoutType().subroutineName().IDENTIFIER().text
        const symbol = this.globalSymbolTable[this.className + "." + name]
        if (symbol == undefined) {
            throw new Error(`Can't find subroutine ${name} in class ${this.className} in symbol table`)
        }
        this.result += `function ${this.className}.${name} ${symbol.subroutineInfo!.localVarsCount}\n`;

        if (ctx.subroutineType().CONSTRUCTOR() != undefined) {
            this.result += "    push constant 0\n"
            this.result += "    call Memory.alloc 1\n"
            this.result += "    pop pointer 0\n"
        } else if (ctx.subroutineType().METHOD() != undefined) {
            this.result += "    push argument 0\n";
            this.result += "    pop pointer 0\n";
        }
    };

    exitSubroutineDeclaration(ctx: SubroutineDeclarationContext) {
        this.localVars = []
    };

    enterVarDeclaration(ctx: VarDeclarationContext) {
        ctx.varNameInDeclaration().forEach((nameContext) => {
            this.localVars.push(nameContext.IDENTIFIER().text)
        })
    };
    enterConstant(ctx: ConstantContext) {
        if (ctx.INTEGER_LITERAL() != undefined) {
            this.result += `    push constant ${ctx.INTEGER_LITERAL()!.text}\n`;
        } else if (ctx.booleanLiteral() != undefined) {
            if (ctx.booleanLiteral()!.FALSE() != undefined) {
                this.result += `    push constant 0\n`;
            } else if (ctx.booleanLiteral()!.TRUE()) {
                this.result += `    push constant 1\n`;
                this.result += `    neg\n`;
            } else {
                throw new Error("Unknown boolean literal")
            }

        } else {
            _notImplemented();
        }
    };
    exitExpression(ctx: ExpressionContext) {
        if (ctx.varName() != undefined) {
            this.result += `    push local ${this.localVars.indexOf(ctx.varName()!.IDENTIFIER().text)}\n`;
        } else if (ctx.binaryOperator() != undefined) {
            const binaryOp = ctx.binaryOperator()!.text
            if (binaryOperationToVmCmd[binaryOp] == undefined) {
                throw new Error(`Unknown binary operator ${binaryOp}`)
            }
            this.result += "\t" + binaryOperationToVmCmd[binaryOp] + "\n";
        } else if (ctx.unaryOperation() != undefined) {
            const unaryOp = ctx.unaryOperation()!.unaryOperator().text;
            if (unaryOperationToVmCmd[unaryOp] == undefined) {
                throw new Error(`Unknown unary operator ${unaryOp}`)
            }
            this.result += "\t" + unaryOperationToVmCmd[unaryOp] + "\n";
        }
    };

    exitLetStatement(ctx: LetStatementContext) {
        if (ctx.varName() != undefined) {
            this.result += `    pop local ${this.localVars.indexOf(ctx.varName()!.IDENTIFIER().text)}\n`;
        }
    };
    //if else
    exitIfExpression(ctx: IfExpressionContext) {
        const parent = ctx._parent as IfStatementContext;
        this.ifNotGoto(parent.endLabel)
    };
    exitIfStatement(ctx: IfStatementContext) {
        const parent = ctx.parent as IfElseStatementContext;
        if (parent.elseStatement() != undefined) {
            parent.endLabel = this.createLabel();
            this.result += `    goto ${parent.endLabel}\n`;
            this.result += `    label ${ctx.endLabel}\n`;
        }
    };
    exitIfElseStatement(ctx: IfElseStatementContext) {
        if (ctx.endLabel) {
            this.result += `    label ${ctx.endLabel} \n`;
        }
    };
    enterWhileStatement(ctx: WhileStatementContext) {
        ctx.startLabel = this.createLabel();
        ctx.endLabel = this.createLabel();
        this.result += `    label ${ctx.startLabel} \n`;
    };
    exitWhileExpression(ctx: WhileExpressionContext) {
        const parent = ctx._parent as WhileStatementContext;
        this.ifNotGoto(parent.endLabel);
    };

    exitWhileStatement(ctx: WhileStatementContext) {
        this.result += `    goto ${ctx.startLabel}\n`;
        this.result += `    label ${ctx.endLabel}\n`;
    };
    enterIfStatement(ctx: IfStatementContext) {
        ctx.endLabel = this.createLabel();
    };
    enterSubroutineCall(ctx: SubroutineCallContext) {
        this.result += `    call ${ctx.subroutineId().text} 0\n`;
        this.result += "    pop temp 0\n";
    };
    exitReturnStatement(ctx: ReturnStatementContext) {
        if (ctx.expression()?.constant()?.THIS_LITERAL() != undefined) {
            this.result += "    push pointer 0\n";
        } else if (ctx.expression() == undefined) {
            this.result += "    push constant 0\n";
        }
        this.result += "    return\n";
    };
    //Utils
    ifNotGoto(endLabel: string) {
        this.result += "    not\n";
        this.result += `    if-goto ${endLabel} \n`;
    }
    getLabel(ind: number) {
        return `${this.className}_${ind}`;
    }
    createLabel() {
        return this.getLabel(this.currentLabelInd++);
    }

    //to fix compiler error
    visitTerminal?: (/*@NotNull*/ node: TerminalNode) => void;

}
class LabelManager {

}