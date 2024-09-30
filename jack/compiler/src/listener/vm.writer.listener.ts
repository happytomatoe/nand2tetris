import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { _notImplemented as notImplemented } from "../error";
import { ArrayAccessContext, ClassDeclarationContext, ConstantContext, ExpressionContext, IfElseStatementContext, IfExpressionContext, IfStatementContext, LetStatementContext, ReturnStatementContext, SubroutineCallContext, SubroutineDeclarationContext, VarDeclarationContext, WhileExpressionContext, WhileStatementContext } from "../generated/JackParser";
import { JackParserListener } from "../generated/JackParserListener";
import { GenericSymbol, LocalSymbolTable, ScopeType, SubroutineScope } from "../symbol";
import { CallType, getCallType } from "./common";

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
    private currentLabelInd: number = 0;
    private localVars: string[] | undefined;
    private localSymbolTable: LocalSymbolTable | undefined;
    private preventNextArrayAccessInvocation: boolean = false;
    constructor(private globalSymbolTable: Record<string, GenericSymbol>) { }

    enterClassDeclaration(ctx: ClassDeclarationContext) {
        if (this.className != "") {
            throw new Error("Cannot change class name")
        }
        this.className = ctx.className()!.text;
    }

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
        if (ctx.symbols == undefined) {
            throw new Error("Subroutine symbols not found in parse tree")
        }
        this.localSymbolTable = new LocalSymbolTable(ctx.symbols);
        this.localVars = ctx.symbols.locals.map(e => e.name);
    };

    exitArrayAccess(ctx: ArrayAccessContext) {
        if (this.preventNextArrayAccessInvocation) {
            this.preventNextArrayAccessInvocation = false;
            return;
        }
        if (this.localVars == undefined) {
            throw new Error("Local symbol table not found in parse tree")
        }
        const varName = ctx.varName().IDENTIFIER().text
        const symbolIndex = this.localVars.indexOf(varName);
        if (symbolIndex == -1) {
            throw new Error(`Can't find variable ${varName} in local symbol table`)
        }
        this.result += `    push local ${symbolIndex}\n`;
        this.result += `    add\n`;
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
        } else if (ctx.THIS_LITERAL() != undefined) {
            this.result += `    push pointer 0\n`;
        } else {
            notImplemented();
        }
    };
    exitExpression(ctx: ExpressionContext) {
        if (this.localVars == undefined) {
            throw new Error("Local symbol table not found in parse tree")
        }
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
        if (this.localVars == undefined) {
            throw new Error("Local symbol table not found in parse tree")
        }
        if (ctx.varName() != undefined) {
            this.result += `    pop local ${this.localVars.indexOf(ctx.varName()!.IDENTIFIER().text)}\n`;
        }
        if (ctx.expression().arrayAccess() != undefined) {
            ctx.expression().arrayAccess()
            this.preventNextArrayAccessInvocation = true;
        }
    };
    //if else
    enterIfStatement(ctx: IfStatementContext) {
        ctx.endLabel = this.createLabel();
    };
    exitIfStatement(ctx: IfStatementContext) {
        const parent = ctx.parent as IfElseStatementContext;
        if (parent.elseStatement() != undefined) {
            parent.endLabel = this.createLabel();
            this.result += `    goto ${parent.endLabel}\n`;
        }
        this.result += `    label ${ctx.endLabel}\n`;
    };
    exitIfExpression(ctx: IfExpressionContext) {
        const parent = ctx._parent as IfStatementContext;
        this.ifNotGoto(parent.endLabel)
    };
    exitIfElseStatement(ctx: IfElseStatementContext) {
        if (ctx.endLabel) {
            this.result += `    label ${ctx.endLabel} \n`;
        }
    };
    //while
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

    //do
    exitSubroutineCall(ctx: SubroutineCallContext) {
        if (this.localSymbolTable == undefined) {
            throw new Error("Subroutine symbols is not defined when calling other subroutine")
        }
        //method call
        const { callType, subroutineIdText } = getCallType(ctx.subroutineId(), this.className, this.localSymbolTable)
        switch (callType) {
            case CallType.ClassFunctionOrConstructor:
                const argsCount = ctx.expressionList().expression().length
                this.result += `    call ${ctx.subroutineId().text} ${argsCount}\n`;
                // this.result += "    pop temp 0\n";
                break;
            case CallType.LocalMethod:
                //TODO: add more arguments?
                this.result += `    push pointer 0\n`;
                this.result += `    call ${subroutineIdText} 1\n`;
                // this.result += "    pop temp 0\n";
                break;
            case CallType.VarMethod:
                throw notImplemented();
            default:
                throw new Error(`Unknown call type ${callType}`)
        }
    };
    //return
    exitReturnStatement(ctx: ReturnStatementContext) {
        if (ctx.expression() == undefined) {
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