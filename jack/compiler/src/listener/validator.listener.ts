import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { builtInTypes, intRange } from "../builtins";
import { ConstructorMushReturnThis, DuplicatedVariableException, FieldCantBeReferencedInFunction, FunctionCalledAsMethodError, IncorrectConstructorReturnType, IncorrectParamsNumberInSubroutineCallError, IntLiteralIsOutOfRange, JackCompilerError, MethodCalledAsFunctionError, NonVoidFunctionNoReturnError, SubroutineNotAllPathsReturnError, ThisCantBeReferencedInFunction, UndeclaredVariableError, UnknownClassError, UnknownSubroutineCallError, UnreachableCodeError, VoidSubroutineReturnsValueError, WrongLiteralTypeError } from "../error";
import { ClassDeclarationContext, ClassNameContext, ClassVarDecContext, ConstantContext, ElseStatementContext, IfStatementContext, LetStatementContext, ParameterContext, RBraceContext, ReturnStatementContext, StatementContext, SubroutineBodyContext, SubroutineCallContext, SubroutineDeclarationContext, SubroutineDecWithoutTypeContext, VarDeclarationContext, VarNameContext, VarTypeContext, WhileStatementContext } from "../generated/JackParser";
import { JackParserListener } from "../generated/JackParserListener";
import { GenericSymbol, LocalSymbolTable, ScopeType, SubroutineType } from "../symbol";
import { CallType, getCallType } from "./common";


/**
 * Validates Jack file
 */
export class ValidatorListener implements JackParserListener {
    private localSymbolTable: LocalSymbolTable = new LocalSymbolTable();
    private subroutineShouldReturnVoidType: boolean = false;
    private controlFlowGraphNode: BinaryTreeNode = new BinaryTreeNode();
    private subroutineName: string = ""
    private className = ""
    private stopProcessingErrorsInThisScope = false;
    private subroutineType?: SubroutineType;
    constructor(private globalSymbolTable: Record<string, GenericSymbol>, public errors: JackCompilerError[] = []) { }

    enterClassDeclaration(ctx: ClassDeclarationContext) {
        const newName = ctx.className()?.text
        if (this.className != "") {
            throw new Error("Cannot change class name")
        }
        this.className = newName;
        ctx.localSymbolTable = this.localSymbolTable;
    };

    enterClassVarDec(ctx: ClassVarDecContext) {
        let scope: ScopeType;
        if (ctx.STATIC() != undefined) {
            scope = ScopeType.Static;
        } else if (ctx.FIELD() != undefined) {
            scope = ScopeType.This;
        } else {
            throw new Error("Unknown field modifier ")
        }
        const type = ctx.fieldList().varType().text
        ctx.fieldList().fieldName().forEach(field => {
            this.#localSymbolTableAdd(ctx.start.line, ctx.start.startIndex, scope, field.text, type);
        });
    };
    enterSubroutineDeclaration(ctx: SubroutineDeclarationContext) {
        if (ctx.subroutineType().CONSTRUCTOR() != undefined) {
            this.subroutineType = SubroutineType.Constructor;
            if (ctx.subroutineDecWithoutType().subroutineReturnType().text !== this.className) {
                this.#addError(new IncorrectConstructorReturnType(ctx.start.line, ctx.start.startIndex));
            }
        } else if (ctx.subroutineType().FUNCTION() != undefined) {
            this.subroutineType = SubroutineType.Function;
        } else if (ctx.subroutineType().METHOD != undefined) {
            this.subroutineType = SubroutineType.Method;
        } else {
            throw new Error("Unknown subroutine type ")
        }
    }
    enterSubroutineDecWithoutType(ctx: SubroutineDecWithoutTypeContext) {
        const returnType = ctx.subroutineReturnType()
        this.subroutineShouldReturnVoidType = returnType.VOID() != undefined
        this.controlFlowGraphNode = new BinaryTreeNode(undefined);
        this.subroutineName = ctx.subroutineName().text
    };

    enterParameter(ctx: ParameterContext) {
        this.#defineArgument(ctx.start.line,
            ctx.start.startIndex,
            ctx.parameterName().text,
            ctx.varType().text,
            this.subroutineType == SubroutineType.Method);
    };
    //Var
    enterVarType(ctx: VarTypeContext) {
        if (ctx.IDENTIFIER() != undefined) {
            const type = ctx.IDENTIFIER()!.text
            if (this.globalSymbolTable[type] === undefined) {
                this.#addError(new UnknownClassError(ctx.start.line, ctx.start.startIndex, type));
            }
        }
    };

    enterVarDeclaration(ctx: VarDeclarationContext) {
        const type = ctx.varType().text
        ctx.varNameInDeclaration().forEach(name => {
            this.#localSymbolTableAdd(ctx.start.line, ctx.start.startIndex, ScopeType.Local, name.text, type);
        })
    };

    /**
     * Var name when using it - do Statement, let ... as opposed to varNameInDeclaration
     */
    enterVarName(ctx: VarNameContext) {
        const symbol = this.localSymbolTable.lookup(ctx.text)
        if (symbol == undefined) {
            this.#addError(new UndeclaredVariableError(ctx.start.line, ctx.start.startIndex, ctx.text));
        } else if (this.subroutineType == SubroutineType.Function && symbol.scope == ScopeType.This) {
            this.#addError(new FieldCantBeReferencedInFunction(ctx.start.line, ctx.start.startIndex));
        }
    };

    enterConstant(ctx: ConstantContext) {
        if (ctx.THIS_LITERAL() != undefined && this.subroutineType == SubroutineType.Function) {
            this.#addError(new ThisCantBeReferencedInFunction(ctx.start.line, ctx.start.startIndex));
        }
    };

    enterStatement(ctx: StatementContext) {
        if (this.controlFlowGraphNode._returns == true) {
            this.#addError(new UnreachableCodeError(ctx.start.line, ctx.start.startIndex));
            this.stopProcessingErrorsInThisScope = true;
        }
    };
    enterRBrace(ctx: RBraceContext) {
        this.stopProcessingErrorsInThisScope = false;
    };
    /**
     * Control flow
     */
    enterWhileStatement(ctx: WhileStatementContext) {
        this.controlFlowGraphNode = this.controlFlowGraphNode.left = new BinaryTreeNode(this.controlFlowGraphNode);
    };

    exitWhileStatement(ctx: WhileStatementContext) {
        if (this.controlFlowGraphNode?.parent != null) {
            this.controlFlowGraphNode = this.controlFlowGraphNode.parent
        }
    };
    enterIfStatement(ctx: IfStatementContext) {
        this.controlFlowGraphNode = this.controlFlowGraphNode.left = new BinaryTreeNode(this.controlFlowGraphNode);
    };
    exitIfStatement(ctx: IfStatementContext) {
        if (this.controlFlowGraphNode?.parent != null) {
            this.controlFlowGraphNode = this.controlFlowGraphNode.parent
        }
    };
    enterElseStatement(ctx: ElseStatementContext) {
        this.controlFlowGraphNode = this.controlFlowGraphNode.right = new BinaryTreeNode(this.controlFlowGraphNode);
    };
    exitElseStatement(ctx: ElseStatementContext) {
        if (this.controlFlowGraphNode?.parent != null) {
            this.controlFlowGraphNode = this.controlFlowGraphNode.parent
        }

    };
    enterLetStatement(ctx: LetStatementContext) {
        const varName = ctx.varName()
        const constCtx = ctx.expression().constant()
        //corresponding literal type check
        if (varName != undefined && constCtx != undefined &&
            this.localSymbolTable.lookup(ctx.varName()!.text) &&
            ctx.expression().constant()!.NULL_LITERAL() == undefined) {
            const type = this.localSymbolTable.lookup(ctx.varName()!.text)!.type
            if (literalTypes.indexOf(type) != -1) {
                const constantCtx = ctx.expression().constant()!
                switch (type) {
                    case "char":
                    case "int":
                        if (constantCtx.INTEGER_LITERAL() === undefined) {
                            this.#addError(new WrongLiteralTypeError(ctx.start.line, ctx.start.startIndex, type));
                        } else {
                            const value = parseInt(constantCtx.INTEGER_LITERAL()!.text)
                            if (value > intRange.max) {
                                this.#addError(new IntLiteralIsOutOfRange(ctx.start.line, ctx.start.startIndex, value, intRange.min, intRange.max))
                            }
                        }
                        break;
                    case "boolean":
                        if (constantCtx.booleanLiteral() === undefined) {
                            this.#addError(new WrongLiteralTypeError(ctx.start.line, ctx.start.startIndex, type));
                        }
                        break;
                    case "String":
                        if (constantCtx.STRING_LITERAL() === undefined) {
                            this.#addError(new WrongLiteralTypeError(ctx.start.line, ctx.start.startIndex, type.toLowerCase()));
                        }
                        break;
                    default:
                        throw new Error(`Unknown literal type ${type}`)
                }
            }
        }
        //int min value check
        const unaryOp = ctx.expression().unaryOperation()
        if (varName && unaryOp != undefined && unaryOp.unaryOperator().MINUS() !== undefined &&
            unaryOp!.expression().constant() != undefined && unaryOp!.expression().constant()?.INTEGER_LITERAL() !== undefined) {
            const value = parseInt(unaryOp!.expression().constant()!.INTEGER_LITERAL()!.text)
            if (-value < intRange.min) {
                this.#addError(new IntLiteralIsOutOfRange(ctx.start.line, ctx.start.startIndex, value, intRange.min, intRange.max));
            }
        }
    };

    enterSubroutineCall(ctx: SubroutineCallContext) {
        //check if variable exists with the name before dot
        const subroutineId = ctx.subroutineId()
        const { callType, subroutineIdText } = getCallType(subroutineId, this.className, this.localSymbolTable)

        const symbol = this.globalSymbolTable[subroutineIdText]
        if (symbol == undefined) {
            this.#addError(new UnknownSubroutineCallError(ctx.start.line, ctx.start.startIndex,
                subroutineId.subroutineName().text,
                subroutineId.className()?.text
            ));
        } else {
            //method called as a function
            if (symbol.subroutineInfo?.type == SubroutineType.Method
                && callType == CallType.ClassFunctionOrConstructor) {
                this.#addError(new MethodCalledAsFunctionError(ctx.start.line, ctx.start.startIndex,
                    subroutineId.subroutineName().text));
            }
            // function called as a method
            else if (symbol.subroutineInfo?.type == SubroutineType.Function
                && callType == CallType.LocalMethod) {
                this.#addError(new FunctionCalledAsMethodError(ctx.start.line, ctx.start.startIndex,
                    subroutineId.subroutineName().text));
            } else {
                //check parameter count 
                const l = ctx.expressionList().expression().length
                if (symbol.subroutineInfo!.paramsCount != l) {
                    this.#addError(new IncorrectParamsNumberInSubroutineCallError(ctx.start.line, ctx.start.startIndex, subroutineId.text,
                        symbol.subroutineInfo!.paramsCount, l));
                }
            }
        }

    };
    enterReturnStatement(ctx: ReturnStatementContext) {
        const returnsVoid = ctx.expression() == undefined
        if (returnsVoid && !this.subroutineShouldReturnVoidType) {
            this.#addError(new NonVoidFunctionNoReturnError(ctx.start.line, ctx.start.startIndex));
        }
        if (!returnsVoid && this.subroutineShouldReturnVoidType) {
            this.#addError(new VoidSubroutineReturnsValueError(ctx.start.line, ctx.start.startIndex));
        }
        this.controlFlowGraphNode._returns = true;
        if (this.subroutineType == SubroutineType.Constructor) {
            if (returnsVoid || ctx.expression()!.expression().length > 1 ||
                ctx.expression()!.constant() == undefined || ctx.expression()!.constant()!.THIS_LITERAL() == undefined) {
                this.#addError(new ConstructorMushReturnThis(ctx.start.line, ctx.start.startIndex))
            }
        }
    };

    exitSubroutineBody(ctx: SubroutineBodyContext) {
        if (!this.controlFlowGraphNode.returns) {
            this.#addError(new SubroutineNotAllPathsReturnError(ctx.start.line, ctx.start.startIndex, this.subroutineName));
        }
        this.subroutineType = undefined;

    };
    exitSubroutineDeclaration(ctx: SubroutineDeclarationContext) {
        ctx.symbols = this.localSymbolTable.popStack();
    };

    exitClassDeclaration(ctx: ClassDeclarationContext) {
        while (this.controlFlowGraphNode?.parent != undefined) {
            this.controlFlowGraphNode = this.controlFlowGraphNode.parent
        }
    };

    //Utils
    #defineArgument(line: number, position: number, name: string, type: string, inMethod: boolean) {
        if (this.localSymbolTable.lookup(name)) {
            this.#addError(new DuplicatedVariableException(line, position, name));
        } else {
            this.localSymbolTable.defineArgument(name, type, inMethod);
        }
    }
    #localSymbolTableAdd(line: number, position: number, scope: ScopeType, name: string, type: string) {
        if (this.localSymbolTable.lookup(name)) {
            this.#addError(new DuplicatedVariableException(line, position, name));
        } else {
            this.localSymbolTable.define(scope, name, type);
        }
    }
    #addError<T extends JackCompilerError>(error: T) {
        if (!this.stopProcessingErrorsInThisScope)
            this.errors.push(error);
    }
    //to fix compiler error
    visitTerminal?: (/*@NotNull*/ node: TerminalNode) => void;
}


class BinaryTreeNode {
    _returns?: boolean;
    constructor(
        public parent?: BinaryTreeNode,
        public left?: BinaryTreeNode,
        public right?: BinaryTreeNode) { }

    get returns(): boolean {
        if (this._returns) {
            return this._returns;
        } else if (this.right == undefined && this.left == undefined) {
            return false;
        } else if (this.right != undefined && this.left != undefined) {
            return this.left.returns && this.right.returns;
        } else if (this.left != undefined) {
            return false;
        } else {
            throw new Error("Something went wrong - CFG has only right  subtree")
        }
    }
    print() {
        console.log("Branch returns value")
        console.log(".")
        console.log(this.printBT());
    }

    printBT(prefix: string = "", side: Side = Side.LEFT) {
        let res: string = ""
        if (this._returns) {
            res += this.#pad(prefix, side)
            res += " " + this._returns + "\n";
            return res;
        } else {
            if (this.right == undefined && this.left == undefined) {
                res += this.#pad(prefix, side)
                res += " " + false + "\n";
            } else {
                res += this.left?.printBT((side == Side.LEFT ? "|   " : "    "), Side.LEFT);
                if (this.right) {
                    res += prefix
                    res += this.right?.printBT((side == Side.LEFT ? "|\t" : "\t"), Side.RIGHT);
                } else {
                    res += "\n";
                }

            }
        }
        return res;
    }
    #pad(prefix: string, side: Side): string {
        return side == Side.LEFT ? "├──" : "└──";
    }
}
enum Side {
    LEFT,
    RIGHT
}
const literalTypes = [
    ...builtInTypes,
    "String"
]