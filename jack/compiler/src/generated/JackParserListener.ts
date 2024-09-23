// Generated from JackParser.g4 by ANTLR 4.9.0-SNAPSHOT


import { ParseTreeListener } from "antlr4ts/tree/ParseTreeListener";

import { ProgramContext } from "./JackParser";
import { ClassDeclarationContext } from "./JackParser";
import { ClassNameContext } from "./JackParser";
import { ClassVarDecContext } from "./JackParser";
import { SubroutineDecContext } from "./JackParser";
import { SubroutineNameContext } from "./JackParser";
import { SubroutineReturnTypeContext } from "./JackParser";
import { VarTypeContext } from "./JackParser";
import { ParameterListContext } from "./JackParser";
import { ParameterContext } from "./JackParser";
import { SubroutineBodyContext } from "./JackParser";
import { VarDecContext } from "./JackParser";
import { VarNameContext } from "./JackParser";
import { StatementsContext } from "./JackParser";
import { StatementContext } from "./JackParser";
import { LetStatementContext } from "./JackParser";
import { IfStatementContext } from "./JackParser";
import { WhileStatementContext } from "./JackParser";
import { DoStatementContext } from "./JackParser";
import { SubroutineCallContext } from "./JackParser";
import { ReturnStatementContext } from "./JackParser";
import { ExpressionListContext } from "./JackParser";
import { ExpressionContext } from "./JackParser";
import { GroupedExpressionContext } from "./JackParser";
import { UnaryOpContext } from "./JackParser";
import { ArrayAccessContext } from "./JackParser";
import { ConstantContext } from "./JackParser";
import { UnaryOperatorContext } from "./JackParser";
import { BinaryOperatorContext } from "./JackParser";


/**
 * This interface defines a complete listener for a parse tree produced by
 * `JackParser`.
 */
export interface JackParserListener extends ParseTreeListener {
	/**
	 * Enter a parse tree produced by `JackParser.program`.
	 * @param ctx the parse tree
	 */
	enterProgram?: (ctx: ProgramContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.program`.
	 * @param ctx the parse tree
	 */
	exitProgram?: (ctx: ProgramContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.classDeclaration`.
	 * @param ctx the parse tree
	 */
	enterClassDeclaration?: (ctx: ClassDeclarationContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.classDeclaration`.
	 * @param ctx the parse tree
	 */
	exitClassDeclaration?: (ctx: ClassDeclarationContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.className`.
	 * @param ctx the parse tree
	 */
	enterClassName?: (ctx: ClassNameContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.className`.
	 * @param ctx the parse tree
	 */
	exitClassName?: (ctx: ClassNameContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.classVarDec`.
	 * @param ctx the parse tree
	 */
	enterClassVarDec?: (ctx: ClassVarDecContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.classVarDec`.
	 * @param ctx the parse tree
	 */
	exitClassVarDec?: (ctx: ClassVarDecContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.subroutineDec`.
	 * @param ctx the parse tree
	 */
	enterSubroutineDec?: (ctx: SubroutineDecContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.subroutineDec`.
	 * @param ctx the parse tree
	 */
	exitSubroutineDec?: (ctx: SubroutineDecContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.subroutineName`.
	 * @param ctx the parse tree
	 */
	enterSubroutineName?: (ctx: SubroutineNameContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.subroutineName`.
	 * @param ctx the parse tree
	 */
	exitSubroutineName?: (ctx: SubroutineNameContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.subroutineReturnType`.
	 * @param ctx the parse tree
	 */
	enterSubroutineReturnType?: (ctx: SubroutineReturnTypeContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.subroutineReturnType`.
	 * @param ctx the parse tree
	 */
	exitSubroutineReturnType?: (ctx: SubroutineReturnTypeContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.varType`.
	 * @param ctx the parse tree
	 */
	enterVarType?: (ctx: VarTypeContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.varType`.
	 * @param ctx the parse tree
	 */
	exitVarType?: (ctx: VarTypeContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.parameterList`.
	 * @param ctx the parse tree
	 */
	enterParameterList?: (ctx: ParameterListContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.parameterList`.
	 * @param ctx the parse tree
	 */
	exitParameterList?: (ctx: ParameterListContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.parameter`.
	 * @param ctx the parse tree
	 */
	enterParameter?: (ctx: ParameterContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.parameter`.
	 * @param ctx the parse tree
	 */
	exitParameter?: (ctx: ParameterContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.subroutineBody`.
	 * @param ctx the parse tree
	 */
	enterSubroutineBody?: (ctx: SubroutineBodyContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.subroutineBody`.
	 * @param ctx the parse tree
	 */
	exitSubroutineBody?: (ctx: SubroutineBodyContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.varDec`.
	 * @param ctx the parse tree
	 */
	enterVarDec?: (ctx: VarDecContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.varDec`.
	 * @param ctx the parse tree
	 */
	exitVarDec?: (ctx: VarDecContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.varName`.
	 * @param ctx the parse tree
	 */
	enterVarName?: (ctx: VarNameContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.varName`.
	 * @param ctx the parse tree
	 */
	exitVarName?: (ctx: VarNameContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.statements`.
	 * @param ctx the parse tree
	 */
	enterStatements?: (ctx: StatementsContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.statements`.
	 * @param ctx the parse tree
	 */
	exitStatements?: (ctx: StatementsContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.statement`.
	 * @param ctx the parse tree
	 */
	enterStatement?: (ctx: StatementContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.statement`.
	 * @param ctx the parse tree
	 */
	exitStatement?: (ctx: StatementContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.letStatement`.
	 * @param ctx the parse tree
	 */
	enterLetStatement?: (ctx: LetStatementContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.letStatement`.
	 * @param ctx the parse tree
	 */
	exitLetStatement?: (ctx: LetStatementContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.ifStatement`.
	 * @param ctx the parse tree
	 */
	enterIfStatement?: (ctx: IfStatementContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.ifStatement`.
	 * @param ctx the parse tree
	 */
	exitIfStatement?: (ctx: IfStatementContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.whileStatement`.
	 * @param ctx the parse tree
	 */
	enterWhileStatement?: (ctx: WhileStatementContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.whileStatement`.
	 * @param ctx the parse tree
	 */
	exitWhileStatement?: (ctx: WhileStatementContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.doStatement`.
	 * @param ctx the parse tree
	 */
	enterDoStatement?: (ctx: DoStatementContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.doStatement`.
	 * @param ctx the parse tree
	 */
	exitDoStatement?: (ctx: DoStatementContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.subroutineCall`.
	 * @param ctx the parse tree
	 */
	enterSubroutineCall?: (ctx: SubroutineCallContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.subroutineCall`.
	 * @param ctx the parse tree
	 */
	exitSubroutineCall?: (ctx: SubroutineCallContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.returnStatement`.
	 * @param ctx the parse tree
	 */
	enterReturnStatement?: (ctx: ReturnStatementContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.returnStatement`.
	 * @param ctx the parse tree
	 */
	exitReturnStatement?: (ctx: ReturnStatementContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.expressionList`.
	 * @param ctx the parse tree
	 */
	enterExpressionList?: (ctx: ExpressionListContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.expressionList`.
	 * @param ctx the parse tree
	 */
	exitExpressionList?: (ctx: ExpressionListContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.expression`.
	 * @param ctx the parse tree
	 */
	enterExpression?: (ctx: ExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.expression`.
	 * @param ctx the parse tree
	 */
	exitExpression?: (ctx: ExpressionContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.groupedExpression`.
	 * @param ctx the parse tree
	 */
	enterGroupedExpression?: (ctx: GroupedExpressionContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.groupedExpression`.
	 * @param ctx the parse tree
	 */
	exitGroupedExpression?: (ctx: GroupedExpressionContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.unaryOp`.
	 * @param ctx the parse tree
	 */
	enterUnaryOp?: (ctx: UnaryOpContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.unaryOp`.
	 * @param ctx the parse tree
	 */
	exitUnaryOp?: (ctx: UnaryOpContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.arrayAccess`.
	 * @param ctx the parse tree
	 */
	enterArrayAccess?: (ctx: ArrayAccessContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.arrayAccess`.
	 * @param ctx the parse tree
	 */
	exitArrayAccess?: (ctx: ArrayAccessContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.constant`.
	 * @param ctx the parse tree
	 */
	enterConstant?: (ctx: ConstantContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.constant`.
	 * @param ctx the parse tree
	 */
	exitConstant?: (ctx: ConstantContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.unaryOperator`.
	 * @param ctx the parse tree
	 */
	enterUnaryOperator?: (ctx: UnaryOperatorContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.unaryOperator`.
	 * @param ctx the parse tree
	 */
	exitUnaryOperator?: (ctx: UnaryOperatorContext) => void;

	/**
	 * Enter a parse tree produced by `JackParser.binaryOperator`.
	 * @param ctx the parse tree
	 */
	enterBinaryOperator?: (ctx: BinaryOperatorContext) => void;
	/**
	 * Exit a parse tree produced by `JackParser.binaryOperator`.
	 * @param ctx the parse tree
	 */
	exitBinaryOperator?: (ctx: BinaryOperatorContext) => void;
}

