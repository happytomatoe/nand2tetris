import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { JackCompilerError } from "../error";
import { ArrayAccessContext, ClassDeclarationContext, ConstantContext, EqualsContext, ExpressionContext, IfElseStatementContext, IfExpressionContext, IfStatementContext, LetStatementContext, ReturnStatementContext, StatementContext, SubroutineCallContext, SubroutineDeclarationContext, WhileExpressionContext, WhileStatementContext } from "../generated/JackParser";
import { JackParserListener } from "../generated/JackParserListener";
import { GenericSymbol, LocalSymbolTable, scopeTypeToString, VariableSymbol } from "../symbol";
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

/**
 * Transforms parse tree into VM code
 */
export class VMWriter implements JackParserListener {
    public result: string = ""
    private className: string = "";
    private currentLabelInd: number = 0;
    private localSymbolTable: LocalSymbolTable | undefined;
    public errors: JackCompilerError[] = [];
    private afterEquals = false;
    constructor(private globalSymbolTable: Record<string, GenericSymbol>) { }

    enterClassDeclaration(ctx: ClassDeclarationContext) {
        if (this.className != "") {
            throw new Error("Cannot change class name")
        }
        if (ctx.localSymbolTable == undefined) {
            throw new Error("Local symbol table not found in parse tree")
        }
        this.className = ctx.className()!.text;
        this.localSymbolTable = ctx.localSymbolTable;
    }

    enterSubroutineDeclaration(ctx: SubroutineDeclarationContext) {
        const name = ctx.subroutineDecWithoutType().subroutineName().IDENTIFIER().text
        const symbol = this.globalSymbolTable[this.className + "." + name]
        if (symbol == undefined) {
            throw new Error(`Can't find subroutine ${name} in class ${this.className} in symbol table`)
        }
        this.result += `function ${this.className}.${name} ${symbol.subroutineInfo!.localVarsCount}\n`;

        if (ctx.subroutineType().CONSTRUCTOR() != undefined) {

            this.result += `    push constant ${this.localSymbolTable!.fieldsCount()}\n`
            this.result += "    call Memory.alloc 1\n"
            this.result += "    pop pointer 0\n"
        } else if (ctx.subroutineType().METHOD() != undefined) {
            this.result += "    push argument 0\n";
            this.result += "    pop pointer 0\n";
        }

        if (ctx.symbols == undefined) {
            throw new Error("Subroutine symbols not found in parse tree")
        }
        this.localSymbolTable!.setSubroutineScope(ctx.symbols);
    };
    exitArrayAccess(ctx: ArrayAccessContext) {
        const varName = ctx.varName().IDENTIFIER().text
        const symbol = this.localSymbolTable!.lookup(varName);
        if (symbol == undefined) {
            throw new Error(`Can't find variable ${varName} in local symbol table`)
        }
        this.result += `    push local ${symbol.index}\n`;
        this.result += `    add\n`;
        if (this.afterEquals) {
            this.result += `    pop pointer 1\n`;
            this.result += `    push that 0\n`;
        }
    };
    enterEquals(ctx: EqualsContext) {
        this.afterEquals = true;
    };
    exitStatement(ctx: StatementContext) {
        this.afterEquals = false;
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
        } else if (ctx.STRING_LITERAL() != undefined) {

            const str = ctx.STRING_LITERAL()!.text
                //cutoff ""
                .slice(1, -1)
            this.result += `    push constant ${str.length}\n`;
            this.result += `    call String.new 1\n`;
            for (const char of str) {
                this.result += `//${char}\n`;
                this.result += ` push constant ${char.charCodeAt(0)}\n`;
                this.result += ' call String.appendChar 2\n';
            }
        } else if (ctx.NULL_LITERAL() != undefined) {
            this.result += `    push constant 0\n`;
        } else {
            throw new Error("Unknown constant type")
        }
    };
    exitExpression(ctx: ExpressionContext) {

        if (ctx.varName() != undefined) {
            const varName = ctx.varName()!.IDENTIFIER().text
            const symbol = this.localSymbolTable!.lookup(varName)
            if (symbol == undefined) {
                throw new Error(`Cannot find variable ${varName} in arguments or local variables`);
            }
            this.pushVarOntoStack(symbol);
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
    pushVarOntoStack(symbol: VariableSymbol) {
        this.result += `    push ${scopeTypeToString(symbol.scope)} ${symbol.index}\n`;
    }
    exitLetStatement(ctx: LetStatementContext) {

        if (ctx.varName() != undefined) {
            const symbol = this.localSymbolTable!.lookup(ctx.varName()!.IDENTIFIER().text)
            if (symbol == undefined) {
                throw new Error(`Can't find variable ${ctx.varName()!.IDENTIFIER().text} in local symbol table`);
            }
            this.result += `    pop ${scopeTypeToString(symbol.scope)} ${symbol.index}\n`;
        } else if (ctx.arrayAccess() != undefined) {
            this.result += "pop temp 0\n";
            this.result += "pop pointer 1\n";
            this.result += "push temp 0\n";
            this.result += "pop that 0\n";
        } else {
            throw new Error(`Unknown let statement type`)
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

    enterSubroutineCall(ctx: SubroutineCallContext) {
        const { callType, symbol } = getCallType(ctx.subroutineId(), this.className, this.localSymbolTable!)
        if (callType === CallType.VarMethod) {
            this.pushVarOntoStack(symbol!);
        }
    };
    //do
    exitSubroutineCall(ctx: SubroutineCallContext) {
        //method call
        const { callType, subroutineIdText, symbol } = getCallType(ctx.subroutineId(), this.className, this.localSymbolTable!)
        switch (callType) {
            case CallType.ClassFunctionOrConstructor:
                const argsCount = ctx.expressionList().expression().length
                this.result += `    call ${ctx.subroutineId().text} ${argsCount}\n`;
                break;
            case CallType.LocalMethod: {
                // //TODO: deduplicate 
                const expressionsCount = ctx.expressionList().expression().length
                this.result += `    push pointer 0\n`;
                this.result += `    call ${subroutineIdText} ${expressionsCount + 1}\n`;
                break;
            }
            case CallType.VarMethod: {
                const expressionsCount = ctx.expressionList().expression().length
                this.result += `    call ${subroutineIdText} ${expressionsCount + 1}\n`;
                break;
            }
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
        return `${this.className}_${ind} `;
    }

    createLabel() {
        return this.getLabel(this.currentLabelInd++);
    }

    //to fix compiler error
    visitTerminal?: (/*@NotNull*/ node: TerminalNode) => void;
}
class LabelManager {

}