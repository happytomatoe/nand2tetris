// Generated from JackParser.g4 by ANTLR 4.9.0-SNAPSHOT


import { ATN } from "antlr4ts/atn/ATN";
import { ATNDeserializer } from "antlr4ts/atn/ATNDeserializer";
import { FailedPredicateException } from "antlr4ts/FailedPredicateException";
import { NotNull } from "antlr4ts/Decorators";
import { NoViableAltException } from "antlr4ts/NoViableAltException";
import { Override } from "antlr4ts/Decorators";
import { Parser } from "antlr4ts/Parser";
import { ParserRuleContext } from "antlr4ts/ParserRuleContext";
import { ParserATNSimulator } from "antlr4ts/atn/ParserATNSimulator";
import { ParseTreeListener } from "antlr4ts/tree/ParseTreeListener";
import { ParseTreeVisitor } from "antlr4ts/tree/ParseTreeVisitor";
import { RecognitionException } from "antlr4ts/RecognitionException";
import { RuleContext } from "antlr4ts/RuleContext";
//import { RuleVersion } from "antlr4ts/RuleVersion";
import { TerminalNode } from "antlr4ts/tree/TerminalNode";
import { Token } from "antlr4ts/Token";
import { TokenStream } from "antlr4ts/TokenStream";
import { Vocabulary } from "antlr4ts/Vocabulary";
import { VocabularyImpl } from "antlr4ts/VocabularyImpl";

import * as Utils from "antlr4ts/misc/Utils";

import { JackParserListener } from "./JackParserListener";
import { JackParserVisitor } from "./JackParserVisitor";


export class JackParser extends Parser {
	public static readonly CLASS = 1;
	public static readonly CONSTRUCTOR = 2;
	public static readonly FUNCTION = 3;
	public static readonly METHOD = 4;
	public static readonly FIELD = 5;
	public static readonly STATIC = 6;
	public static readonly VAR = 7;
	public static readonly INT = 8;
	public static readonly CHAR = 9;
	public static readonly BOOLEAN = 10;
	public static readonly VOID = 11;
	public static readonly LET = 12;
	public static readonly DO = 13;
	public static readonly IF = 14;
	public static readonly ELSE = 15;
	public static readonly WHILE = 16;
	public static readonly RETURN = 17;
	public static readonly LBRACE = 18;
	public static readonly RBRACE = 19;
	public static readonly LPAREN = 20;
	public static readonly RPAREN = 21;
	public static readonly LBRACKET = 22;
	public static readonly RBRACKET = 23;
	public static readonly DOT = 24;
	public static readonly COMMA = 25;
	public static readonly SEMICOLON = 26;
	public static readonly EQUALS = 27;
	public static readonly PLUS = 28;
	public static readonly MINUS = 29;
	public static readonly MUL = 30;
	public static readonly DIV = 31;
	public static readonly AND = 32;
	public static readonly OR = 33;
	public static readonly TILDE = 34;
	public static readonly LESS_THAN = 35;
	public static readonly GREATER_THAN = 36;
	public static readonly WS = 37;
	public static readonly COMMENT = 38;
	public static readonly LINE_COMMENT = 39;
	public static readonly INTEGER_LITERAL = 40;
	public static readonly STRING_LITERAL = 41;
	public static readonly BOOLEAN_LITERAL = 42;
	public static readonly NULL_LITERAL = 43;
	public static readonly THIS_LITERAL = 44;
	public static readonly IDENTIFIER = 45;
	public static readonly RULE_program = 0;
	public static readonly RULE_classDeclaration = 1;
	public static readonly RULE_className = 2;
	public static readonly RULE_classVarDec = 3;
	public static readonly RULE_subroutineDec = 4;
	public static readonly RULE_subroutineName = 5;
	public static readonly RULE_subroutineReturnType = 6;
	public static readonly RULE_varType = 7;
	public static readonly RULE_parameterList = 8;
	public static readonly RULE_parameter = 9;
	public static readonly RULE_subroutineBody = 10;
	public static readonly RULE_varDec = 11;
	public static readonly RULE_varName = 12;
	public static readonly RULE_statements = 13;
	public static readonly RULE_statement = 14;
	public static readonly RULE_letStatement = 15;
	public static readonly RULE_ifStatement = 16;
	public static readonly RULE_whileStatement = 17;
	public static readonly RULE_doStatement = 18;
	public static readonly RULE_subroutineCall = 19;
	public static readonly RULE_returnStatement = 20;
	public static readonly RULE_expressionList = 21;
	public static readonly RULE_expression = 22;
	public static readonly RULE_groupedExpression = 23;
	public static readonly RULE_unaryOp = 24;
	public static readonly RULE_arrayAccess = 25;
	public static readonly RULE_constant = 26;
	public static readonly RULE_unaryOperator = 27;
	public static readonly RULE_binaryOperator = 28;
	// tslint:disable:no-trailing-whitespace
	public static readonly ruleNames: string[] = [
		"program", "classDeclaration", "className", "classVarDec", "subroutineDec", 
		"subroutineName", "subroutineReturnType", "varType", "parameterList", 
		"parameter", "subroutineBody", "varDec", "varName", "statements", "statement", 
		"letStatement", "ifStatement", "whileStatement", "doStatement", "subroutineCall", 
		"returnStatement", "expressionList", "expression", "groupedExpression", 
		"unaryOp", "arrayAccess", "constant", "unaryOperator", "binaryOperator",
	];

	private static readonly _LITERAL_NAMES: Array<string | undefined> = [
		undefined, "'class'", "'constructor'", "'function'", "'method'", "'field'", 
		"'static'", "'var'", "'int'", "'char'", "'boolean'", "'void'", "'let'", 
		"'do'", "'if'", "'else'", "'while'", "'return'", "'{'", "'}'", "'('", 
		"')'", "'['", "']'", "'.'", "','", "';'", "'='", "'+'", "'-'", "'*'", 
		"'/'", "'&'", "'|'", "'~'", "'<'", "'>'", undefined, undefined, undefined, 
		undefined, undefined, undefined, "'null'", "'this'",
	];
	private static readonly _SYMBOLIC_NAMES: Array<string | undefined> = [
		undefined, "CLASS", "CONSTRUCTOR", "FUNCTION", "METHOD", "FIELD", "STATIC", 
		"VAR", "INT", "CHAR", "BOOLEAN", "VOID", "LET", "DO", "IF", "ELSE", "WHILE", 
		"RETURN", "LBRACE", "RBRACE", "LPAREN", "RPAREN", "LBRACKET", "RBRACKET", 
		"DOT", "COMMA", "SEMICOLON", "EQUALS", "PLUS", "MINUS", "MUL", "DIV", 
		"AND", "OR", "TILDE", "LESS_THAN", "GREATER_THAN", "WS", "COMMENT", "LINE_COMMENT", 
		"INTEGER_LITERAL", "STRING_LITERAL", "BOOLEAN_LITERAL", "NULL_LITERAL", 
		"THIS_LITERAL", "IDENTIFIER",
	];
	public static readonly VOCABULARY: Vocabulary = new VocabularyImpl(JackParser._LITERAL_NAMES, JackParser._SYMBOLIC_NAMES, []);

	// @Override
	// @NotNull
	public get vocabulary(): Vocabulary {
		return JackParser.VOCABULARY;
	}
	// tslint:enable:no-trailing-whitespace

	// @Override
	public get grammarFileName(): string { return "JackParser.g4"; }

	// @Override
	public get ruleNames(): string[] { return JackParser.ruleNames; }

	// @Override
	public override get serializedATN(): string { return JackParser._serializedATN; }

	protected createFailedPredicateException(predicate?: string, message?: string): FailedPredicateException {
		return new FailedPredicateException(this, predicate, message);
	}

	constructor(input: TokenStream) {
		super(input);
		this._interp = new ParserATNSimulator(JackParser._ATN, this);
	}
	// @RuleVersion(0)
	public program(): ProgramContext {
		let _localctx: ProgramContext = new ProgramContext(this._ctx, this.state);
		this.enterRule(_localctx, 0, JackParser.RULE_program);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 61;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === JackParser.CLASS) {
				{
				{
				this.state = 58;
				this.classDeclaration();
				}
				}
				this.state = 63;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public classDeclaration(): ClassDeclarationContext {
		let _localctx: ClassDeclarationContext = new ClassDeclarationContext(this._ctx, this.state);
		this.enterRule(_localctx, 2, JackParser.RULE_classDeclaration);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 64;
			this.match(JackParser.CLASS);
			this.state = 65;
			this.className();
			this.state = 66;
			this.match(JackParser.LBRACE);
			this.state = 70;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === JackParser.FIELD || _la === JackParser.STATIC) {
				{
				{
				this.state = 67;
				this.classVarDec();
				}
				}
				this.state = 72;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 76;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << JackParser.CONSTRUCTOR) | (1 << JackParser.FUNCTION) | (1 << JackParser.METHOD))) !== 0)) {
				{
				{
				this.state = 73;
				this.subroutineDec();
				}
				}
				this.state = 78;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 79;
			this.match(JackParser.RBRACE);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public className(): ClassNameContext {
		let _localctx: ClassNameContext = new ClassNameContext(this._ctx, this.state);
		this.enterRule(_localctx, 4, JackParser.RULE_className);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 81;
			this.match(JackParser.IDENTIFIER);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public classVarDec(): ClassVarDecContext {
		let _localctx: ClassVarDecContext = new ClassVarDecContext(this._ctx, this.state);
		this.enterRule(_localctx, 6, JackParser.RULE_classVarDec);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 83;
			_la = this._input.LA(1);
			if (!(_la === JackParser.FIELD || _la === JackParser.STATIC)) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			this.state = 84;
			this.varType();
			this.state = 85;
			this.match(JackParser.IDENTIFIER);
			this.state = 90;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === JackParser.COMMA) {
				{
				{
				this.state = 86;
				this.match(JackParser.COMMA);
				this.state = 87;
				this.match(JackParser.IDENTIFIER);
				}
				}
				this.state = 92;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 93;
			this.match(JackParser.SEMICOLON);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public subroutineDec(): SubroutineDecContext {
		let _localctx: SubroutineDecContext = new SubroutineDecContext(this._ctx, this.state);
		this.enterRule(_localctx, 8, JackParser.RULE_subroutineDec);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 95;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << JackParser.CONSTRUCTOR) | (1 << JackParser.FUNCTION) | (1 << JackParser.METHOD))) !== 0))) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			this.state = 96;
			this.subroutineReturnType();
			this.state = 97;
			this.subroutineName();
			this.state = 98;
			this.match(JackParser.LPAREN);
			this.state = 99;
			this.parameterList();
			this.state = 100;
			this.match(JackParser.RPAREN);
			this.state = 101;
			this.subroutineBody();
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public subroutineName(): SubroutineNameContext {
		let _localctx: SubroutineNameContext = new SubroutineNameContext(this._ctx, this.state);
		this.enterRule(_localctx, 10, JackParser.RULE_subroutineName);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 103;
			this.match(JackParser.IDENTIFIER);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public subroutineReturnType(): SubroutineReturnTypeContext {
		let _localctx: SubroutineReturnTypeContext = new SubroutineReturnTypeContext(this._ctx, this.state);
		this.enterRule(_localctx, 12, JackParser.RULE_subroutineReturnType);
		try {
			this.state = 107;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case JackParser.INT:
			case JackParser.CHAR:
			case JackParser.BOOLEAN:
			case JackParser.IDENTIFIER:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 105;
				this.varType();
				}
				break;
			case JackParser.VOID:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 106;
				this.match(JackParser.VOID);
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public varType(): VarTypeContext {
		let _localctx: VarTypeContext = new VarTypeContext(this._ctx, this.state);
		this.enterRule(_localctx, 14, JackParser.RULE_varType);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 109;
			_la = this._input.LA(1);
			if (!((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << JackParser.INT) | (1 << JackParser.CHAR) | (1 << JackParser.BOOLEAN))) !== 0) || _la === JackParser.IDENTIFIER)) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public parameterList(): ParameterListContext {
		let _localctx: ParameterListContext = new ParameterListContext(this._ctx, this.state);
		this.enterRule(_localctx, 16, JackParser.RULE_parameterList);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 122;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << JackParser.INT) | (1 << JackParser.CHAR) | (1 << JackParser.BOOLEAN))) !== 0) || _la === JackParser.IDENTIFIER) {
				{
				this.state = 111;
				this.varType();
				this.state = 112;
				this.parameter();
				this.state = 119;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la === JackParser.COMMA) {
					{
					{
					this.state = 113;
					this.match(JackParser.COMMA);
					this.state = 114;
					this.varType();
					this.state = 115;
					this.parameter();
					}
					}
					this.state = 121;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public parameter(): ParameterContext {
		let _localctx: ParameterContext = new ParameterContext(this._ctx, this.state);
		this.enterRule(_localctx, 18, JackParser.RULE_parameter);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 124;
			this.match(JackParser.IDENTIFIER);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public subroutineBody(): SubroutineBodyContext {
		let _localctx: SubroutineBodyContext = new SubroutineBodyContext(this._ctx, this.state);
		this.enterRule(_localctx, 20, JackParser.RULE_subroutineBody);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 126;
			this.match(JackParser.LBRACE);
			this.state = 130;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === JackParser.VAR) {
				{
				{
				this.state = 127;
				this.varDec();
				}
				}
				this.state = 132;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 133;
			this.statements();
			this.state = 134;
			this.returnStatement();
			this.state = 135;
			this.match(JackParser.RBRACE);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public varDec(): VarDecContext {
		let _localctx: VarDecContext = new VarDecContext(this._ctx, this.state);
		this.enterRule(_localctx, 22, JackParser.RULE_varDec);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 137;
			this.match(JackParser.VAR);
			this.state = 138;
			this.varType();
			this.state = 139;
			this.varName();
			this.state = 144;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while (_la === JackParser.COMMA) {
				{
				{
				this.state = 140;
				this.match(JackParser.COMMA);
				this.state = 141;
				this.match(JackParser.IDENTIFIER);
				}
				}
				this.state = 146;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			this.state = 147;
			this.match(JackParser.SEMICOLON);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public varName(): VarNameContext {
		let _localctx: VarNameContext = new VarNameContext(this._ctx, this.state);
		this.enterRule(_localctx, 24, JackParser.RULE_varName);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 149;
			this.match(JackParser.IDENTIFIER);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public statements(): StatementsContext {
		let _localctx: StatementsContext = new StatementsContext(this._ctx, this.state);
		this.enterRule(_localctx, 26, JackParser.RULE_statements);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 154;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			while ((((_la) & ~0x1F) === 0 && ((1 << _la) & ((1 << JackParser.LET) | (1 << JackParser.DO) | (1 << JackParser.IF) | (1 << JackParser.WHILE))) !== 0)) {
				{
				{
				this.state = 151;
				this.statement();
				}
				}
				this.state = 156;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public statement(): StatementContext {
		let _localctx: StatementContext = new StatementContext(this._ctx, this.state);
		this.enterRule(_localctx, 28, JackParser.RULE_statement);
		try {
			this.state = 161;
			this._errHandler.sync(this);
			switch (this._input.LA(1)) {
			case JackParser.LET:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 157;
				this.letStatement();
				}
				break;
			case JackParser.IF:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 158;
				this.ifStatement();
				}
				break;
			case JackParser.WHILE:
				this.enterOuterAlt(_localctx, 3);
				{
				this.state = 159;
				this.whileStatement();
				}
				break;
			case JackParser.DO:
				this.enterOuterAlt(_localctx, 4);
				{
				this.state = 160;
				this.doStatement();
				}
				break;
			default:
				throw new NoViableAltException(this);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public letStatement(): LetStatementContext {
		let _localctx: LetStatementContext = new LetStatementContext(this._ctx, this.state);
		this.enterRule(_localctx, 30, JackParser.RULE_letStatement);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 163;
			this.match(JackParser.LET);
			this.state = 166;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 11, this._ctx) ) {
			case 1:
				{
				this.state = 164;
				this.match(JackParser.IDENTIFIER);
				}
				break;

			case 2:
				{
				this.state = 165;
				this.arrayAccess();
				}
				break;
			}
			this.state = 168;
			this.match(JackParser.EQUALS);
			this.state = 169;
			this.expression(0);
			this.state = 170;
			this.match(JackParser.SEMICOLON);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public ifStatement(): IfStatementContext {
		let _localctx: IfStatementContext = new IfStatementContext(this._ctx, this.state);
		this.enterRule(_localctx, 32, JackParser.RULE_ifStatement);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 172;
			this.match(JackParser.IF);
			this.state = 173;
			this.match(JackParser.LPAREN);
			this.state = 174;
			this.expression(0);
			this.state = 175;
			this.match(JackParser.RPAREN);
			this.state = 176;
			this.match(JackParser.LBRACE);
			this.state = 177;
			this.statements();
			this.state = 178;
			this.match(JackParser.RBRACE);
			this.state = 184;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (_la === JackParser.ELSE) {
				{
				this.state = 179;
				this.match(JackParser.ELSE);
				this.state = 180;
				this.match(JackParser.LBRACE);
				this.state = 181;
				this.statements();
				this.state = 182;
				this.match(JackParser.RBRACE);
				}
			}

			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public whileStatement(): WhileStatementContext {
		let _localctx: WhileStatementContext = new WhileStatementContext(this._ctx, this.state);
		this.enterRule(_localctx, 34, JackParser.RULE_whileStatement);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 186;
			this.match(JackParser.WHILE);
			this.state = 187;
			this.match(JackParser.LPAREN);
			this.state = 188;
			this.expression(0);
			this.state = 189;
			this.match(JackParser.RPAREN);
			this.state = 190;
			this.match(JackParser.LBRACE);
			this.state = 191;
			this.statements();
			this.state = 192;
			this.match(JackParser.RBRACE);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public doStatement(): DoStatementContext {
		let _localctx: DoStatementContext = new DoStatementContext(this._ctx, this.state);
		this.enterRule(_localctx, 36, JackParser.RULE_doStatement);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 194;
			this.match(JackParser.DO);
			this.state = 195;
			this.subroutineCall();
			this.state = 196;
			this.match(JackParser.SEMICOLON);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public subroutineCall(): SubroutineCallContext {
		let _localctx: SubroutineCallContext = new SubroutineCallContext(this._ctx, this.state);
		this.enterRule(_localctx, 38, JackParser.RULE_subroutineCall);
		let _la: number;
		try {
			this.state = 210;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 13, this._ctx) ) {
			case 1:
				this.enterOuterAlt(_localctx, 1);
				{
				this.state = 198;
				this.match(JackParser.IDENTIFIER);
				this.state = 199;
				this.match(JackParser.LPAREN);
				this.state = 200;
				this.expressionList();
				this.state = 201;
				this.match(JackParser.RPAREN);
				}
				break;

			case 2:
				this.enterOuterAlt(_localctx, 2);
				{
				this.state = 203;
				_la = this._input.LA(1);
				if (!(_la === JackParser.THIS_LITERAL || _la === JackParser.IDENTIFIER)) {
				this._errHandler.recoverInline(this);
				} else {
					if (this._input.LA(1) === Token.EOF) {
						this.matchedEOF = true;
					}

					this._errHandler.reportMatch(this);
					this.consume();
				}
				this.state = 204;
				this.match(JackParser.DOT);
				this.state = 205;
				this.match(JackParser.IDENTIFIER);
				this.state = 206;
				this.match(JackParser.LPAREN);
				this.state = 207;
				this.expressionList();
				this.state = 208;
				this.match(JackParser.RPAREN);
				}
				break;
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public returnStatement(): ReturnStatementContext {
		let _localctx: ReturnStatementContext = new ReturnStatementContext(this._ctx, this.state);
		this.enterRule(_localctx, 40, JackParser.RULE_returnStatement);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 212;
			this.match(JackParser.RETURN);
			this.state = 214;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (((((_la - 20)) & ~0x1F) === 0 && ((1 << (_la - 20)) & ((1 << (JackParser.LPAREN - 20)) | (1 << (JackParser.MINUS - 20)) | (1 << (JackParser.TILDE - 20)) | (1 << (JackParser.INTEGER_LITERAL - 20)) | (1 << (JackParser.STRING_LITERAL - 20)) | (1 << (JackParser.BOOLEAN_LITERAL - 20)) | (1 << (JackParser.NULL_LITERAL - 20)) | (1 << (JackParser.THIS_LITERAL - 20)) | (1 << (JackParser.IDENTIFIER - 20)))) !== 0)) {
				{
				this.state = 213;
				this.expression(0);
				}
			}

			this.state = 216;
			this.match(JackParser.SEMICOLON);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public expressionList(): ExpressionListContext {
		let _localctx: ExpressionListContext = new ExpressionListContext(this._ctx, this.state);
		this.enterRule(_localctx, 42, JackParser.RULE_expressionList);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 226;
			this._errHandler.sync(this);
			_la = this._input.LA(1);
			if (((((_la - 20)) & ~0x1F) === 0 && ((1 << (_la - 20)) & ((1 << (JackParser.LPAREN - 20)) | (1 << (JackParser.MINUS - 20)) | (1 << (JackParser.TILDE - 20)) | (1 << (JackParser.INTEGER_LITERAL - 20)) | (1 << (JackParser.STRING_LITERAL - 20)) | (1 << (JackParser.BOOLEAN_LITERAL - 20)) | (1 << (JackParser.NULL_LITERAL - 20)) | (1 << (JackParser.THIS_LITERAL - 20)) | (1 << (JackParser.IDENTIFIER - 20)))) !== 0)) {
				{
				this.state = 218;
				this.expression(0);
				this.state = 223;
				this._errHandler.sync(this);
				_la = this._input.LA(1);
				while (_la === JackParser.COMMA) {
					{
					{
					this.state = 219;
					this.match(JackParser.COMMA);
					this.state = 220;
					this.expression(0);
					}
					}
					this.state = 225;
					this._errHandler.sync(this);
					_la = this._input.LA(1);
				}
				}
			}

			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}

	public expression(): ExpressionContext;
	public expression(_p: number): ExpressionContext;
	// @RuleVersion(0)
	public expression(_p?: number): ExpressionContext {
		if (_p === undefined) {
			_p = 0;
		}

		let _parentctx: ParserRuleContext = this._ctx;
		let _parentState: number = this.state;
		let _localctx: ExpressionContext = new ExpressionContext(this._ctx, _parentState);
		let _prevctx: ExpressionContext = _localctx;
		let _startState: number = 44;
		this.enterRecursionRule(_localctx, 44, JackParser.RULE_expression, _p);
		try {
			let _alt: number;
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 235;
			this._errHandler.sync(this);
			switch ( this.interpreter.adaptivePredict(this._input, 17, this._ctx) ) {
			case 1:
				{
				this.state = 229;
				this.constant();
				}
				break;

			case 2:
				{
				this.state = 230;
				this.match(JackParser.IDENTIFIER);
				}
				break;

			case 3:
				{
				this.state = 231;
				this.subroutineCall();
				}
				break;

			case 4:
				{
				this.state = 232;
				this.arrayAccess();
				}
				break;

			case 5:
				{
				this.state = 233;
				this.unaryOp();
				}
				break;

			case 6:
				{
				this.state = 234;
				this.groupedExpression();
				}
				break;
			}
			this._ctx._stop = this._input.tryLT(-1);
			this.state = 243;
			this._errHandler.sync(this);
			_alt = this.interpreter.adaptivePredict(this._input, 18, this._ctx);
			while (_alt !== 2 && _alt !== ATN.INVALID_ALT_NUMBER) {
				if (_alt === 1) {
					if (this._parseListeners != null) {
						this.triggerExitRuleEvent();
					}
					_prevctx = _localctx;
					{
					{
					_localctx = new ExpressionContext(_parentctx, _parentState);
					this.pushNewRecursionContext(_localctx, _startState, JackParser.RULE_expression);
					this.state = 237;
					if (!(this.precpred(this._ctx, 7))) {
						throw this.createFailedPredicateException("this.precpred(this._ctx, 7)");
					}
					this.state = 238;
					this.binaryOperator();
					this.state = 239;
					this.expression(8);
					}
					}
				}
				this.state = 245;
				this._errHandler.sync(this);
				_alt = this.interpreter.adaptivePredict(this._input, 18, this._ctx);
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.unrollRecursionContexts(_parentctx);
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public groupedExpression(): GroupedExpressionContext {
		let _localctx: GroupedExpressionContext = new GroupedExpressionContext(this._ctx, this.state);
		this.enterRule(_localctx, 46, JackParser.RULE_groupedExpression);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 246;
			this.match(JackParser.LPAREN);
			this.state = 247;
			this.expression(0);
			this.state = 248;
			this.match(JackParser.RPAREN);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public unaryOp(): UnaryOpContext {
		let _localctx: UnaryOpContext = new UnaryOpContext(this._ctx, this.state);
		this.enterRule(_localctx, 48, JackParser.RULE_unaryOp);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 250;
			this.unaryOperator();
			this.state = 251;
			this.expression(0);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public arrayAccess(): ArrayAccessContext {
		let _localctx: ArrayAccessContext = new ArrayAccessContext(this._ctx, this.state);
		this.enterRule(_localctx, 50, JackParser.RULE_arrayAccess);
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 253;
			this.match(JackParser.IDENTIFIER);
			this.state = 254;
			this.match(JackParser.LBRACKET);
			this.state = 255;
			this.expression(0);
			this.state = 256;
			this.match(JackParser.RBRACKET);
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public constant(): ConstantContext {
		let _localctx: ConstantContext = new ConstantContext(this._ctx, this.state);
		this.enterRule(_localctx, 52, JackParser.RULE_constant);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 258;
			_la = this._input.LA(1);
			if (!(((((_la - 40)) & ~0x1F) === 0 && ((1 << (_la - 40)) & ((1 << (JackParser.INTEGER_LITERAL - 40)) | (1 << (JackParser.STRING_LITERAL - 40)) | (1 << (JackParser.BOOLEAN_LITERAL - 40)) | (1 << (JackParser.NULL_LITERAL - 40)) | (1 << (JackParser.THIS_LITERAL - 40)))) !== 0))) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public unaryOperator(): UnaryOperatorContext {
		let _localctx: UnaryOperatorContext = new UnaryOperatorContext(this._ctx, this.state);
		this.enterRule(_localctx, 54, JackParser.RULE_unaryOperator);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 260;
			_la = this._input.LA(1);
			if (!(_la === JackParser.MINUS || _la === JackParser.TILDE)) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}
	// @RuleVersion(0)
	public binaryOperator(): BinaryOperatorContext {
		let _localctx: BinaryOperatorContext = new BinaryOperatorContext(this._ctx, this.state);
		this.enterRule(_localctx, 56, JackParser.RULE_binaryOperator);
		let _la: number;
		try {
			this.enterOuterAlt(_localctx, 1);
			{
			this.state = 262;
			_la = this._input.LA(1);
			if (!(((((_la - 27)) & ~0x1F) === 0 && ((1 << (_la - 27)) & ((1 << (JackParser.EQUALS - 27)) | (1 << (JackParser.PLUS - 27)) | (1 << (JackParser.MINUS - 27)) | (1 << (JackParser.MUL - 27)) | (1 << (JackParser.DIV - 27)) | (1 << (JackParser.AND - 27)) | (1 << (JackParser.OR - 27)) | (1 << (JackParser.LESS_THAN - 27)) | (1 << (JackParser.GREATER_THAN - 27)))) !== 0))) {
			this._errHandler.recoverInline(this);
			} else {
				if (this._input.LA(1) === Token.EOF) {
					this.matchedEOF = true;
				}

				this._errHandler.reportMatch(this);
				this.consume();
			}
			}
		}
		catch (re) {
			if (re instanceof RecognitionException) {
				_localctx.exception = re;
				this._errHandler.reportError(this, re);
				this._errHandler.recover(this, re);
			} else {
				throw re;
			}
		}
		finally {
			this.exitRule();
		}
		return _localctx;
	}

	public override sempred(_localctx: RuleContext, ruleIndex: number, predIndex: number): boolean {
		switch (ruleIndex) {
		case 22:
			return this.expression_sempred(_localctx as ExpressionContext, predIndex);
		}
		return true;
	}
	private expression_sempred(_localctx: ExpressionContext, predIndex: number): boolean {
		switch (predIndex) {
		case 0:
			return this.precpred(this._ctx, 7);
		}
		return true;
	}

	public static readonly _serializedATN: string =
		"\x03\uC91D\uCABA\u058D\uAFBA\u4F53\u0607\uEA8B\uC241\x03/\u010B\x04\x02" +
		"\t\x02\x04\x03\t\x03\x04\x04\t\x04\x04\x05\t\x05\x04\x06\t\x06\x04\x07" +
		"\t\x07\x04\b\t\b\x04\t\t\t\x04\n\t\n\x04\v\t\v\x04\f\t\f\x04\r\t\r\x04" +
		"\x0E\t\x0E\x04\x0F\t\x0F\x04\x10\t\x10\x04\x11\t\x11\x04\x12\t\x12\x04" +
		"\x13\t\x13\x04\x14\t\x14\x04\x15\t\x15\x04\x16\t\x16\x04\x17\t\x17\x04" +
		"\x18\t\x18\x04\x19\t\x19\x04\x1A\t\x1A\x04\x1B\t\x1B\x04\x1C\t\x1C\x04" +
		"\x1D\t\x1D\x04\x1E\t\x1E\x03\x02\x07\x02>\n\x02\f\x02\x0E\x02A\v\x02\x03" +
		"\x03\x03\x03\x03\x03\x03\x03\x07\x03G\n\x03\f\x03\x0E\x03J\v\x03\x03\x03" +
		"\x07\x03M\n\x03\f\x03\x0E\x03P\v\x03\x03\x03\x03\x03\x03\x04\x03\x04\x03" +
		"\x05\x03\x05\x03\x05\x03\x05\x03\x05\x07\x05[\n\x05\f\x05\x0E\x05^\v\x05" +
		"\x03\x05\x03\x05\x03\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x06\x03\x06" +
		"\x03\x06\x03\x07\x03\x07\x03\b\x03\b\x05\bn\n\b\x03\t\x03\t\x03\n\x03" +
		"\n\x03\n\x03\n\x03\n\x03\n\x07\nx\n\n\f\n\x0E\n{\v\n\x05\n}\n\n\x03\v" +
		"\x03\v\x03\f\x03\f\x07\f\x83\n\f\f\f\x0E\f\x86\v\f\x03\f\x03\f\x03\f\x03" +
		"\f\x03\r\x03\r\x03\r\x03\r\x03\r\x07\r\x91\n\r\f\r\x0E\r\x94\v\r\x03\r" +
		"\x03\r\x03\x0E\x03\x0E\x03\x0F\x07\x0F\x9B\n\x0F\f\x0F\x0E\x0F\x9E\v\x0F" +
		"\x03\x10\x03\x10\x03\x10\x03\x10\x05\x10\xA4\n\x10\x03\x11\x03\x11\x03" +
		"\x11\x05\x11\xA9\n\x11\x03\x11\x03\x11\x03\x11\x03\x11\x03\x12\x03\x12" +
		"\x03\x12\x03\x12\x03\x12\x03\x12\x03\x12\x03\x12\x03\x12\x03\x12\x03\x12" +
		"\x03\x12\x05\x12\xBB\n\x12\x03\x13\x03\x13\x03\x13\x03\x13\x03\x13\x03" +
		"\x13\x03\x13\x03\x13\x03\x14\x03\x14\x03\x14\x03\x14\x03\x15\x03\x15\x03" +
		"\x15\x03\x15\x03\x15\x03\x15\x03\x15\x03\x15\x03\x15\x03\x15\x03\x15\x03" +
		"\x15\x05\x15\xD5\n\x15\x03\x16\x03\x16\x05\x16\xD9\n\x16\x03\x16\x03\x16" +
		"\x03\x17\x03\x17\x03\x17\x07\x17\xE0\n\x17\f\x17\x0E\x17\xE3\v\x17\x05" +
		"\x17\xE5\n\x17\x03\x18\x03\x18\x03\x18\x03\x18\x03\x18\x03\x18\x03\x18" +
		"\x05\x18\xEE\n\x18\x03\x18\x03\x18\x03\x18\x03\x18\x07\x18\xF4\n\x18\f" +
		"\x18\x0E\x18\xF7\v\x18\x03\x19\x03\x19\x03\x19\x03\x19\x03\x1A\x03\x1A" +
		"\x03\x1A\x03\x1B\x03\x1B\x03\x1B\x03\x1B\x03\x1B\x03\x1C\x03\x1C\x03\x1D" +
		"\x03\x1D\x03\x1E\x03\x1E\x03\x1E\x02\x02\x03.\x1F\x02\x02\x04\x02\x06" +
		"\x02\b\x02\n\x02\f\x02\x0E\x02\x10\x02\x12\x02\x14\x02\x16\x02\x18\x02" +
		"\x1A\x02\x1C\x02\x1E\x02 \x02\"\x02$\x02&\x02(\x02*\x02,\x02.\x020\x02" +
		"2\x024\x026\x028\x02:\x02\x02\t\x03\x02\x07\b\x03\x02\x04\x06\x04\x02" +
		"\n\f//\x03\x02./\x03\x02*.\x04\x02\x1F\x1F$$\x04\x02\x1D#%&\x02\u0106" +
		"\x02?\x03\x02\x02\x02\x04B\x03\x02\x02\x02\x06S\x03\x02\x02\x02\bU\x03" +
		"\x02\x02\x02\na\x03\x02\x02\x02\fi\x03\x02\x02\x02\x0Em\x03\x02\x02\x02" +
		"\x10o\x03\x02\x02\x02\x12|\x03\x02\x02\x02\x14~\x03\x02\x02\x02\x16\x80" +
		"\x03\x02\x02\x02\x18\x8B\x03\x02\x02\x02\x1A\x97\x03\x02\x02\x02\x1C\x9C" +
		"\x03\x02\x02\x02\x1E\xA3\x03\x02\x02\x02 \xA5\x03\x02\x02\x02\"\xAE\x03" +
		"\x02\x02\x02$\xBC\x03\x02\x02\x02&\xC4\x03\x02\x02\x02(\xD4\x03\x02\x02" +
		"\x02*\xD6\x03\x02\x02\x02,\xE4\x03\x02\x02\x02.\xED\x03\x02\x02\x020\xF8" +
		"\x03\x02\x02\x022\xFC\x03\x02\x02\x024\xFF\x03\x02\x02\x026\u0104\x03" +
		"\x02\x02\x028\u0106\x03\x02\x02\x02:\u0108\x03\x02\x02\x02<>\x05\x04\x03" +
		"\x02=<\x03\x02\x02\x02>A\x03\x02\x02\x02?=\x03\x02\x02\x02?@\x03\x02\x02" +
		"\x02@\x03\x03\x02\x02\x02A?\x03\x02\x02\x02BC\x07\x03\x02\x02CD\x05\x06" +
		"\x04\x02DH\x07\x14\x02\x02EG\x05\b\x05\x02FE\x03\x02\x02\x02GJ\x03\x02" +
		"\x02\x02HF\x03\x02\x02\x02HI\x03\x02\x02\x02IN\x03\x02\x02\x02JH\x03\x02" +
		"\x02\x02KM\x05\n\x06\x02LK\x03\x02\x02\x02MP\x03\x02\x02\x02NL\x03\x02" +
		"\x02\x02NO\x03\x02\x02\x02OQ\x03\x02\x02\x02PN\x03\x02\x02\x02QR\x07\x15" +
		"\x02\x02R\x05\x03\x02\x02\x02ST\x07/\x02\x02T\x07\x03\x02\x02\x02UV\t" +
		"\x02\x02\x02VW\x05\x10\t\x02W\\\x07/\x02\x02XY\x07\x1B\x02\x02Y[\x07/" +
		"\x02\x02ZX\x03\x02\x02\x02[^\x03\x02\x02\x02\\Z\x03\x02\x02\x02\\]\x03" +
		"\x02\x02\x02]_\x03\x02\x02\x02^\\\x03\x02\x02\x02_`\x07\x1C\x02\x02`\t" +
		"\x03\x02\x02\x02ab\t\x03\x02\x02bc\x05\x0E\b\x02cd\x05\f\x07\x02de\x07" +
		"\x16\x02\x02ef\x05\x12\n\x02fg\x07\x17\x02\x02gh\x05\x16\f\x02h\v\x03" +
		"\x02\x02\x02ij\x07/\x02\x02j\r\x03\x02\x02\x02kn\x05\x10\t\x02ln\x07\r" +
		"\x02\x02mk\x03\x02\x02\x02ml\x03\x02\x02\x02n\x0F\x03\x02\x02\x02op\t" +
		"\x04\x02\x02p\x11\x03\x02\x02\x02qr\x05\x10\t\x02ry\x05\x14\v\x02st\x07" +
		"\x1B\x02\x02tu\x05\x10\t\x02uv\x05\x14\v\x02vx\x03\x02\x02\x02ws\x03\x02" +
		"\x02\x02x{\x03\x02\x02\x02yw\x03\x02\x02\x02yz\x03\x02\x02\x02z}\x03\x02" +
		"\x02\x02{y\x03\x02\x02\x02|q\x03\x02\x02\x02|}\x03\x02\x02\x02}\x13\x03" +
		"\x02\x02\x02~\x7F\x07/\x02\x02\x7F\x15\x03\x02\x02\x02\x80\x84\x07\x14" +
		"\x02\x02\x81\x83\x05\x18\r\x02\x82\x81\x03\x02\x02\x02\x83\x86\x03\x02" +
		"\x02\x02\x84\x82\x03\x02\x02\x02\x84\x85\x03\x02\x02\x02\x85\x87\x03\x02" +
		"\x02\x02\x86\x84\x03\x02\x02\x02\x87\x88\x05\x1C\x0F\x02\x88\x89\x05*" +
		"\x16\x02\x89\x8A\x07\x15\x02\x02\x8A\x17\x03\x02\x02\x02\x8B\x8C\x07\t" +
		"\x02\x02\x8C\x8D\x05\x10\t\x02\x8D\x92\x05\x1A\x0E\x02\x8E\x8F\x07\x1B" +
		"\x02\x02\x8F\x91\x07/\x02\x02\x90\x8E\x03\x02\x02\x02\x91\x94\x03\x02" +
		"\x02\x02\x92\x90\x03\x02\x02\x02\x92\x93\x03\x02\x02\x02\x93\x95\x03\x02" +
		"\x02\x02\x94\x92\x03\x02\x02\x02\x95\x96\x07\x1C\x02\x02\x96\x19\x03\x02" +
		"\x02\x02\x97\x98\x07/\x02\x02\x98\x1B\x03\x02\x02\x02\x99\x9B\x05\x1E" +
		"\x10\x02\x9A\x99\x03\x02\x02\x02\x9B\x9E\x03\x02\x02\x02\x9C\x9A\x03\x02" +
		"\x02\x02\x9C\x9D\x03\x02\x02\x02\x9D\x1D\x03\x02\x02\x02\x9E\x9C\x03\x02" +
		"\x02\x02\x9F\xA4\x05 \x11\x02\xA0\xA4\x05\"\x12\x02\xA1\xA4\x05$\x13\x02" +
		"\xA2\xA4\x05&\x14\x02\xA3\x9F\x03\x02\x02\x02\xA3\xA0\x03\x02\x02\x02" +
		"\xA3\xA1\x03\x02\x02\x02\xA3\xA2\x03\x02\x02\x02\xA4\x1F\x03\x02\x02\x02" +
		"\xA5\xA8\x07\x0E\x02\x02\xA6\xA9\x07/\x02\x02\xA7\xA9\x054\x1B\x02\xA8" +
		"\xA6\x03\x02\x02\x02\xA8\xA7\x03\x02\x02\x02\xA9\xAA\x03\x02\x02\x02\xAA" +
		"\xAB\x07\x1D\x02\x02\xAB\xAC\x05.\x18\x02\xAC\xAD\x07\x1C\x02\x02\xAD" +
		"!\x03\x02\x02\x02\xAE\xAF\x07\x10\x02\x02\xAF\xB0\x07\x16\x02\x02\xB0" +
		"\xB1\x05.\x18\x02\xB1\xB2\x07\x17\x02\x02\xB2\xB3\x07\x14\x02\x02\xB3" +
		"\xB4\x05\x1C\x0F\x02\xB4\xBA\x07\x15\x02\x02\xB5\xB6\x07\x11\x02\x02\xB6" +
		"\xB7\x07\x14\x02\x02\xB7\xB8\x05\x1C\x0F\x02\xB8\xB9\x07\x15\x02\x02\xB9" +
		"\xBB\x03\x02\x02\x02\xBA\xB5\x03\x02\x02\x02\xBA\xBB\x03\x02\x02\x02\xBB" +
		"#\x03\x02\x02\x02\xBC\xBD\x07\x12\x02\x02\xBD\xBE\x07\x16\x02\x02\xBE" +
		"\xBF\x05.\x18\x02\xBF\xC0\x07\x17\x02\x02\xC0\xC1\x07\x14\x02\x02\xC1" +
		"\xC2\x05\x1C\x0F\x02\xC2\xC3\x07\x15\x02\x02\xC3%\x03\x02\x02\x02\xC4" +
		"\xC5\x07\x0F\x02\x02\xC5\xC6\x05(\x15\x02\xC6\xC7\x07\x1C\x02\x02\xC7" +
		"\'\x03\x02\x02\x02\xC8\xC9\x07/\x02\x02\xC9\xCA\x07\x16\x02\x02\xCA\xCB" +
		"\x05,\x17\x02\xCB\xCC\x07\x17\x02\x02\xCC\xD5\x03\x02\x02\x02\xCD\xCE" +
		"\t\x05\x02\x02\xCE\xCF\x07\x1A\x02\x02\xCF\xD0\x07/\x02\x02\xD0\xD1\x07" +
		"\x16\x02\x02\xD1\xD2\x05,\x17\x02\xD2\xD3\x07\x17\x02\x02\xD3\xD5\x03" +
		"\x02\x02\x02\xD4\xC8\x03\x02\x02\x02\xD4\xCD\x03\x02\x02\x02\xD5)\x03" +
		"\x02\x02\x02\xD6\xD8\x07\x13\x02\x02\xD7\xD9\x05.\x18\x02\xD8\xD7\x03" +
		"\x02\x02\x02\xD8\xD9\x03\x02\x02\x02\xD9\xDA\x03\x02\x02\x02\xDA\xDB\x07" +
		"\x1C\x02\x02\xDB+\x03\x02\x02\x02\xDC\xE1\x05.\x18\x02\xDD\xDE\x07\x1B" +
		"\x02\x02\xDE\xE0\x05.\x18\x02\xDF\xDD\x03\x02\x02\x02\xE0\xE3\x03\x02" +
		"\x02\x02\xE1\xDF\x03\x02\x02\x02\xE1\xE2\x03\x02\x02\x02\xE2\xE5\x03\x02" +
		"\x02\x02\xE3\xE1\x03\x02\x02\x02\xE4\xDC\x03\x02\x02\x02\xE4\xE5\x03\x02" +
		"\x02\x02\xE5-\x03\x02\x02\x02\xE6\xE7\b\x18\x01\x02\xE7\xEE\x056\x1C\x02" +
		"\xE8\xEE\x07/\x02\x02\xE9\xEE\x05(\x15\x02\xEA\xEE\x054\x1B\x02\xEB\xEE" +
		"\x052\x1A\x02\xEC\xEE\x050\x19\x02\xED\xE6\x03\x02\x02\x02\xED\xE8\x03" +
		"\x02\x02\x02\xED\xE9\x03\x02\x02\x02\xED\xEA\x03\x02\x02\x02\xED\xEB\x03" +
		"\x02\x02\x02\xED\xEC\x03\x02\x02\x02\xEE\xF5\x03\x02\x02\x02\xEF\xF0\f" +
		"\t\x02\x02\xF0\xF1\x05:\x1E\x02\xF1\xF2\x05.\x18\n\xF2\xF4\x03\x02\x02" +
		"\x02\xF3\xEF\x03\x02\x02\x02\xF4\xF7\x03\x02\x02\x02\xF5\xF3\x03\x02\x02" +
		"\x02\xF5\xF6\x03\x02\x02\x02\xF6/\x03\x02\x02\x02\xF7\xF5\x03\x02\x02" +
		"\x02\xF8\xF9\x07\x16\x02\x02\xF9\xFA\x05.\x18\x02\xFA\xFB\x07\x17\x02" +
		"\x02\xFB1\x03\x02\x02\x02\xFC\xFD\x058\x1D\x02\xFD\xFE\x05.\x18\x02\xFE" +
		"3\x03\x02\x02\x02\xFF\u0100\x07/\x02\x02\u0100\u0101\x07\x18\x02\x02\u0101" +
		"\u0102\x05.\x18\x02\u0102\u0103\x07\x19\x02\x02\u01035\x03\x02\x02\x02" +
		"\u0104\u0105\t\x06\x02\x02\u01057\x03\x02\x02\x02\u0106\u0107\t\x07\x02" +
		"\x02\u01079\x03\x02\x02\x02\u0108\u0109\t\b\x02\x02\u0109;\x03\x02\x02" +
		"\x02\x15?HN\\my|\x84\x92\x9C\xA3\xA8\xBA\xD4\xD8\xE1\xE4\xED\xF5";
	public static __ATN: ATN;
	public static get _ATN(): ATN {
		if (!JackParser.__ATN) {
			JackParser.__ATN = new ATNDeserializer().deserialize(Utils.toCharArray(JackParser._serializedATN));
		}

		return JackParser.__ATN;
	}

}

export class ProgramContext extends ParserRuleContext {
	public classDeclaration(): ClassDeclarationContext[];
	public classDeclaration(i: number): ClassDeclarationContext;
	public classDeclaration(i?: number): ClassDeclarationContext | ClassDeclarationContext[] {
		if (i === undefined) {
			return this.getRuleContexts(ClassDeclarationContext);
		} else {
			return this.getRuleContext(i, ClassDeclarationContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_program; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterProgram) {
			listener.enterProgram(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitProgram) {
			listener.exitProgram(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitProgram) {
			return visitor.visitProgram(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ClassDeclarationContext extends ParserRuleContext {
	public CLASS(): TerminalNode { return this.getToken(JackParser.CLASS, 0); }
	public className(): ClassNameContext {
		return this.getRuleContext(0, ClassNameContext);
	}
	public LBRACE(): TerminalNode { return this.getToken(JackParser.LBRACE, 0); }
	public RBRACE(): TerminalNode { return this.getToken(JackParser.RBRACE, 0); }
	public classVarDec(): ClassVarDecContext[];
	public classVarDec(i: number): ClassVarDecContext;
	public classVarDec(i?: number): ClassVarDecContext | ClassVarDecContext[] {
		if (i === undefined) {
			return this.getRuleContexts(ClassVarDecContext);
		} else {
			return this.getRuleContext(i, ClassVarDecContext);
		}
	}
	public subroutineDec(): SubroutineDecContext[];
	public subroutineDec(i: number): SubroutineDecContext;
	public subroutineDec(i?: number): SubroutineDecContext | SubroutineDecContext[] {
		if (i === undefined) {
			return this.getRuleContexts(SubroutineDecContext);
		} else {
			return this.getRuleContext(i, SubroutineDecContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_classDeclaration; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterClassDeclaration) {
			listener.enterClassDeclaration(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitClassDeclaration) {
			listener.exitClassDeclaration(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitClassDeclaration) {
			return visitor.visitClassDeclaration(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ClassNameContext extends ParserRuleContext {
	public IDENTIFIER(): TerminalNode { return this.getToken(JackParser.IDENTIFIER, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_className; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterClassName) {
			listener.enterClassName(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitClassName) {
			listener.exitClassName(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitClassName) {
			return visitor.visitClassName(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ClassVarDecContext extends ParserRuleContext {
	public varType(): VarTypeContext {
		return this.getRuleContext(0, VarTypeContext);
	}
	public IDENTIFIER(): TerminalNode[];
	public IDENTIFIER(i: number): TerminalNode;
	public IDENTIFIER(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(JackParser.IDENTIFIER);
		} else {
			return this.getToken(JackParser.IDENTIFIER, i);
		}
	}
	public SEMICOLON(): TerminalNode { return this.getToken(JackParser.SEMICOLON, 0); }
	public STATIC(): TerminalNode | undefined { return this.tryGetToken(JackParser.STATIC, 0); }
	public FIELD(): TerminalNode | undefined { return this.tryGetToken(JackParser.FIELD, 0); }
	public COMMA(): TerminalNode[];
	public COMMA(i: number): TerminalNode;
	public COMMA(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(JackParser.COMMA);
		} else {
			return this.getToken(JackParser.COMMA, i);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_classVarDec; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterClassVarDec) {
			listener.enterClassVarDec(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitClassVarDec) {
			listener.exitClassVarDec(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitClassVarDec) {
			return visitor.visitClassVarDec(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class SubroutineDecContext extends ParserRuleContext {
	public subroutineReturnType(): SubroutineReturnTypeContext {
		return this.getRuleContext(0, SubroutineReturnTypeContext);
	}
	public subroutineName(): SubroutineNameContext {
		return this.getRuleContext(0, SubroutineNameContext);
	}
	public LPAREN(): TerminalNode { return this.getToken(JackParser.LPAREN, 0); }
	public parameterList(): ParameterListContext {
		return this.getRuleContext(0, ParameterListContext);
	}
	public RPAREN(): TerminalNode { return this.getToken(JackParser.RPAREN, 0); }
	public subroutineBody(): SubroutineBodyContext {
		return this.getRuleContext(0, SubroutineBodyContext);
	}
	public CONSTRUCTOR(): TerminalNode | undefined { return this.tryGetToken(JackParser.CONSTRUCTOR, 0); }
	public METHOD(): TerminalNode | undefined { return this.tryGetToken(JackParser.METHOD, 0); }
	public FUNCTION(): TerminalNode | undefined { return this.tryGetToken(JackParser.FUNCTION, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_subroutineDec; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterSubroutineDec) {
			listener.enterSubroutineDec(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitSubroutineDec) {
			listener.exitSubroutineDec(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitSubroutineDec) {
			return visitor.visitSubroutineDec(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class SubroutineNameContext extends ParserRuleContext {
	public IDENTIFIER(): TerminalNode { return this.getToken(JackParser.IDENTIFIER, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_subroutineName; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterSubroutineName) {
			listener.enterSubroutineName(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitSubroutineName) {
			listener.exitSubroutineName(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitSubroutineName) {
			return visitor.visitSubroutineName(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class SubroutineReturnTypeContext extends ParserRuleContext {
	public varType(): VarTypeContext | undefined {
		return this.tryGetRuleContext(0, VarTypeContext);
	}
	public VOID(): TerminalNode | undefined { return this.tryGetToken(JackParser.VOID, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_subroutineReturnType; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterSubroutineReturnType) {
			listener.enterSubroutineReturnType(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitSubroutineReturnType) {
			listener.exitSubroutineReturnType(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitSubroutineReturnType) {
			return visitor.visitSubroutineReturnType(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class VarTypeContext extends ParserRuleContext {
	public INT(): TerminalNode | undefined { return this.tryGetToken(JackParser.INT, 0); }
	public CHAR(): TerminalNode | undefined { return this.tryGetToken(JackParser.CHAR, 0); }
	public BOOLEAN(): TerminalNode | undefined { return this.tryGetToken(JackParser.BOOLEAN, 0); }
	public IDENTIFIER(): TerminalNode | undefined { return this.tryGetToken(JackParser.IDENTIFIER, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_varType; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterVarType) {
			listener.enterVarType(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitVarType) {
			listener.exitVarType(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitVarType) {
			return visitor.visitVarType(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ParameterListContext extends ParserRuleContext {
	public varType(): VarTypeContext[];
	public varType(i: number): VarTypeContext;
	public varType(i?: number): VarTypeContext | VarTypeContext[] {
		if (i === undefined) {
			return this.getRuleContexts(VarTypeContext);
		} else {
			return this.getRuleContext(i, VarTypeContext);
		}
	}
	public parameter(): ParameterContext[];
	public parameter(i: number): ParameterContext;
	public parameter(i?: number): ParameterContext | ParameterContext[] {
		if (i === undefined) {
			return this.getRuleContexts(ParameterContext);
		} else {
			return this.getRuleContext(i, ParameterContext);
		}
	}
	public COMMA(): TerminalNode[];
	public COMMA(i: number): TerminalNode;
	public COMMA(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(JackParser.COMMA);
		} else {
			return this.getToken(JackParser.COMMA, i);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_parameterList; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterParameterList) {
			listener.enterParameterList(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitParameterList) {
			listener.exitParameterList(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitParameterList) {
			return visitor.visitParameterList(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ParameterContext extends ParserRuleContext {
	public IDENTIFIER(): TerminalNode { return this.getToken(JackParser.IDENTIFIER, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_parameter; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterParameter) {
			listener.enterParameter(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitParameter) {
			listener.exitParameter(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitParameter) {
			return visitor.visitParameter(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class SubroutineBodyContext extends ParserRuleContext {
	public LBRACE(): TerminalNode { return this.getToken(JackParser.LBRACE, 0); }
	public statements(): StatementsContext {
		return this.getRuleContext(0, StatementsContext);
	}
	public returnStatement(): ReturnStatementContext {
		return this.getRuleContext(0, ReturnStatementContext);
	}
	public RBRACE(): TerminalNode { return this.getToken(JackParser.RBRACE, 0); }
	public varDec(): VarDecContext[];
	public varDec(i: number): VarDecContext;
	public varDec(i?: number): VarDecContext | VarDecContext[] {
		if (i === undefined) {
			return this.getRuleContexts(VarDecContext);
		} else {
			return this.getRuleContext(i, VarDecContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_subroutineBody; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterSubroutineBody) {
			listener.enterSubroutineBody(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitSubroutineBody) {
			listener.exitSubroutineBody(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitSubroutineBody) {
			return visitor.visitSubroutineBody(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class VarDecContext extends ParserRuleContext {
	public VAR(): TerminalNode { return this.getToken(JackParser.VAR, 0); }
	public varType(): VarTypeContext {
		return this.getRuleContext(0, VarTypeContext);
	}
	public varName(): VarNameContext {
		return this.getRuleContext(0, VarNameContext);
	}
	public SEMICOLON(): TerminalNode { return this.getToken(JackParser.SEMICOLON, 0); }
	public COMMA(): TerminalNode[];
	public COMMA(i: number): TerminalNode;
	public COMMA(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(JackParser.COMMA);
		} else {
			return this.getToken(JackParser.COMMA, i);
		}
	}
	public IDENTIFIER(): TerminalNode[];
	public IDENTIFIER(i: number): TerminalNode;
	public IDENTIFIER(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(JackParser.IDENTIFIER);
		} else {
			return this.getToken(JackParser.IDENTIFIER, i);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_varDec; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterVarDec) {
			listener.enterVarDec(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitVarDec) {
			listener.exitVarDec(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitVarDec) {
			return visitor.visitVarDec(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class VarNameContext extends ParserRuleContext {
	public IDENTIFIER(): TerminalNode { return this.getToken(JackParser.IDENTIFIER, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_varName; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterVarName) {
			listener.enterVarName(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitVarName) {
			listener.exitVarName(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitVarName) {
			return visitor.visitVarName(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class StatementsContext extends ParserRuleContext {
	public statement(): StatementContext[];
	public statement(i: number): StatementContext;
	public statement(i?: number): StatementContext | StatementContext[] {
		if (i === undefined) {
			return this.getRuleContexts(StatementContext);
		} else {
			return this.getRuleContext(i, StatementContext);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_statements; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterStatements) {
			listener.enterStatements(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitStatements) {
			listener.exitStatements(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitStatements) {
			return visitor.visitStatements(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class StatementContext extends ParserRuleContext {
	public letStatement(): LetStatementContext | undefined {
		return this.tryGetRuleContext(0, LetStatementContext);
	}
	public ifStatement(): IfStatementContext | undefined {
		return this.tryGetRuleContext(0, IfStatementContext);
	}
	public whileStatement(): WhileStatementContext | undefined {
		return this.tryGetRuleContext(0, WhileStatementContext);
	}
	public doStatement(): DoStatementContext | undefined {
		return this.tryGetRuleContext(0, DoStatementContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_statement; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterStatement) {
			listener.enterStatement(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitStatement) {
			listener.exitStatement(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitStatement) {
			return visitor.visitStatement(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class LetStatementContext extends ParserRuleContext {
	public LET(): TerminalNode { return this.getToken(JackParser.LET, 0); }
	public EQUALS(): TerminalNode { return this.getToken(JackParser.EQUALS, 0); }
	public expression(): ExpressionContext {
		return this.getRuleContext(0, ExpressionContext);
	}
	public SEMICOLON(): TerminalNode { return this.getToken(JackParser.SEMICOLON, 0); }
	public IDENTIFIER(): TerminalNode | undefined { return this.tryGetToken(JackParser.IDENTIFIER, 0); }
	public arrayAccess(): ArrayAccessContext | undefined {
		return this.tryGetRuleContext(0, ArrayAccessContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_letStatement; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterLetStatement) {
			listener.enterLetStatement(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitLetStatement) {
			listener.exitLetStatement(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitLetStatement) {
			return visitor.visitLetStatement(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class IfStatementContext extends ParserRuleContext {
	public IF(): TerminalNode { return this.getToken(JackParser.IF, 0); }
	public LPAREN(): TerminalNode { return this.getToken(JackParser.LPAREN, 0); }
	public expression(): ExpressionContext {
		return this.getRuleContext(0, ExpressionContext);
	}
	public RPAREN(): TerminalNode { return this.getToken(JackParser.RPAREN, 0); }
	public LBRACE(): TerminalNode[];
	public LBRACE(i: number): TerminalNode;
	public LBRACE(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(JackParser.LBRACE);
		} else {
			return this.getToken(JackParser.LBRACE, i);
		}
	}
	public statements(): StatementsContext[];
	public statements(i: number): StatementsContext;
	public statements(i?: number): StatementsContext | StatementsContext[] {
		if (i === undefined) {
			return this.getRuleContexts(StatementsContext);
		} else {
			return this.getRuleContext(i, StatementsContext);
		}
	}
	public RBRACE(): TerminalNode[];
	public RBRACE(i: number): TerminalNode;
	public RBRACE(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(JackParser.RBRACE);
		} else {
			return this.getToken(JackParser.RBRACE, i);
		}
	}
	public ELSE(): TerminalNode | undefined { return this.tryGetToken(JackParser.ELSE, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_ifStatement; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterIfStatement) {
			listener.enterIfStatement(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitIfStatement) {
			listener.exitIfStatement(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitIfStatement) {
			return visitor.visitIfStatement(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class WhileStatementContext extends ParserRuleContext {
	public WHILE(): TerminalNode { return this.getToken(JackParser.WHILE, 0); }
	public LPAREN(): TerminalNode { return this.getToken(JackParser.LPAREN, 0); }
	public expression(): ExpressionContext {
		return this.getRuleContext(0, ExpressionContext);
	}
	public RPAREN(): TerminalNode { return this.getToken(JackParser.RPAREN, 0); }
	public LBRACE(): TerminalNode { return this.getToken(JackParser.LBRACE, 0); }
	public statements(): StatementsContext {
		return this.getRuleContext(0, StatementsContext);
	}
	public RBRACE(): TerminalNode { return this.getToken(JackParser.RBRACE, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_whileStatement; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterWhileStatement) {
			listener.enterWhileStatement(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitWhileStatement) {
			listener.exitWhileStatement(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitWhileStatement) {
			return visitor.visitWhileStatement(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class DoStatementContext extends ParserRuleContext {
	public DO(): TerminalNode { return this.getToken(JackParser.DO, 0); }
	public subroutineCall(): SubroutineCallContext {
		return this.getRuleContext(0, SubroutineCallContext);
	}
	public SEMICOLON(): TerminalNode { return this.getToken(JackParser.SEMICOLON, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_doStatement; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterDoStatement) {
			listener.enterDoStatement(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitDoStatement) {
			listener.exitDoStatement(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitDoStatement) {
			return visitor.visitDoStatement(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class SubroutineCallContext extends ParserRuleContext {
	public IDENTIFIER(): TerminalNode[];
	public IDENTIFIER(i: number): TerminalNode;
	public IDENTIFIER(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(JackParser.IDENTIFIER);
		} else {
			return this.getToken(JackParser.IDENTIFIER, i);
		}
	}
	public LPAREN(): TerminalNode { return this.getToken(JackParser.LPAREN, 0); }
	public expressionList(): ExpressionListContext {
		return this.getRuleContext(0, ExpressionListContext);
	}
	public RPAREN(): TerminalNode { return this.getToken(JackParser.RPAREN, 0); }
	public DOT(): TerminalNode | undefined { return this.tryGetToken(JackParser.DOT, 0); }
	public THIS_LITERAL(): TerminalNode | undefined { return this.tryGetToken(JackParser.THIS_LITERAL, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_subroutineCall; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterSubroutineCall) {
			listener.enterSubroutineCall(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitSubroutineCall) {
			listener.exitSubroutineCall(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitSubroutineCall) {
			return visitor.visitSubroutineCall(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ReturnStatementContext extends ParserRuleContext {
	public RETURN(): TerminalNode { return this.getToken(JackParser.RETURN, 0); }
	public SEMICOLON(): TerminalNode { return this.getToken(JackParser.SEMICOLON, 0); }
	public expression(): ExpressionContext | undefined {
		return this.tryGetRuleContext(0, ExpressionContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_returnStatement; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterReturnStatement) {
			listener.enterReturnStatement(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitReturnStatement) {
			listener.exitReturnStatement(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitReturnStatement) {
			return visitor.visitReturnStatement(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ExpressionListContext extends ParserRuleContext {
	public expression(): ExpressionContext[];
	public expression(i: number): ExpressionContext;
	public expression(i?: number): ExpressionContext | ExpressionContext[] {
		if (i === undefined) {
			return this.getRuleContexts(ExpressionContext);
		} else {
			return this.getRuleContext(i, ExpressionContext);
		}
	}
	public COMMA(): TerminalNode[];
	public COMMA(i: number): TerminalNode;
	public COMMA(i?: number): TerminalNode | TerminalNode[] {
		if (i === undefined) {
			return this.getTokens(JackParser.COMMA);
		} else {
			return this.getToken(JackParser.COMMA, i);
		}
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_expressionList; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterExpressionList) {
			listener.enterExpressionList(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitExpressionList) {
			listener.exitExpressionList(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitExpressionList) {
			return visitor.visitExpressionList(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ExpressionContext extends ParserRuleContext {
	public expression(): ExpressionContext[];
	public expression(i: number): ExpressionContext;
	public expression(i?: number): ExpressionContext | ExpressionContext[] {
		if (i === undefined) {
			return this.getRuleContexts(ExpressionContext);
		} else {
			return this.getRuleContext(i, ExpressionContext);
		}
	}
	public binaryOperator(): BinaryOperatorContext | undefined {
		return this.tryGetRuleContext(0, BinaryOperatorContext);
	}
	public constant(): ConstantContext | undefined {
		return this.tryGetRuleContext(0, ConstantContext);
	}
	public IDENTIFIER(): TerminalNode | undefined { return this.tryGetToken(JackParser.IDENTIFIER, 0); }
	public subroutineCall(): SubroutineCallContext | undefined {
		return this.tryGetRuleContext(0, SubroutineCallContext);
	}
	public arrayAccess(): ArrayAccessContext | undefined {
		return this.tryGetRuleContext(0, ArrayAccessContext);
	}
	public unaryOp(): UnaryOpContext | undefined {
		return this.tryGetRuleContext(0, UnaryOpContext);
	}
	public groupedExpression(): GroupedExpressionContext | undefined {
		return this.tryGetRuleContext(0, GroupedExpressionContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_expression; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterExpression) {
			listener.enterExpression(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitExpression) {
			listener.exitExpression(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitExpression) {
			return visitor.visitExpression(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class GroupedExpressionContext extends ParserRuleContext {
	public LPAREN(): TerminalNode { return this.getToken(JackParser.LPAREN, 0); }
	public expression(): ExpressionContext {
		return this.getRuleContext(0, ExpressionContext);
	}
	public RPAREN(): TerminalNode { return this.getToken(JackParser.RPAREN, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_groupedExpression; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterGroupedExpression) {
			listener.enterGroupedExpression(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitGroupedExpression) {
			listener.exitGroupedExpression(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitGroupedExpression) {
			return visitor.visitGroupedExpression(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class UnaryOpContext extends ParserRuleContext {
	public unaryOperator(): UnaryOperatorContext {
		return this.getRuleContext(0, UnaryOperatorContext);
	}
	public expression(): ExpressionContext {
		return this.getRuleContext(0, ExpressionContext);
	}
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_unaryOp; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterUnaryOp) {
			listener.enterUnaryOp(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitUnaryOp) {
			listener.exitUnaryOp(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitUnaryOp) {
			return visitor.visitUnaryOp(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ArrayAccessContext extends ParserRuleContext {
	public IDENTIFIER(): TerminalNode { return this.getToken(JackParser.IDENTIFIER, 0); }
	public LBRACKET(): TerminalNode { return this.getToken(JackParser.LBRACKET, 0); }
	public expression(): ExpressionContext {
		return this.getRuleContext(0, ExpressionContext);
	}
	public RBRACKET(): TerminalNode { return this.getToken(JackParser.RBRACKET, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_arrayAccess; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterArrayAccess) {
			listener.enterArrayAccess(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitArrayAccess) {
			listener.exitArrayAccess(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitArrayAccess) {
			return visitor.visitArrayAccess(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class ConstantContext extends ParserRuleContext {
	public INTEGER_LITERAL(): TerminalNode | undefined { return this.tryGetToken(JackParser.INTEGER_LITERAL, 0); }
	public STRING_LITERAL(): TerminalNode | undefined { return this.tryGetToken(JackParser.STRING_LITERAL, 0); }
	public BOOLEAN_LITERAL(): TerminalNode | undefined { return this.tryGetToken(JackParser.BOOLEAN_LITERAL, 0); }
	public NULL_LITERAL(): TerminalNode | undefined { return this.tryGetToken(JackParser.NULL_LITERAL, 0); }
	public THIS_LITERAL(): TerminalNode | undefined { return this.tryGetToken(JackParser.THIS_LITERAL, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_constant; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterConstant) {
			listener.enterConstant(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitConstant) {
			listener.exitConstant(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitConstant) {
			return visitor.visitConstant(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class UnaryOperatorContext extends ParserRuleContext {
	public TILDE(): TerminalNode | undefined { return this.tryGetToken(JackParser.TILDE, 0); }
	public MINUS(): TerminalNode | undefined { return this.tryGetToken(JackParser.MINUS, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_unaryOperator; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterUnaryOperator) {
			listener.enterUnaryOperator(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitUnaryOperator) {
			listener.exitUnaryOperator(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitUnaryOperator) {
			return visitor.visitUnaryOperator(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


export class BinaryOperatorContext extends ParserRuleContext {
	public PLUS(): TerminalNode | undefined { return this.tryGetToken(JackParser.PLUS, 0); }
	public MINUS(): TerminalNode | undefined { return this.tryGetToken(JackParser.MINUS, 0); }
	public MUL(): TerminalNode | undefined { return this.tryGetToken(JackParser.MUL, 0); }
	public DIV(): TerminalNode | undefined { return this.tryGetToken(JackParser.DIV, 0); }
	public AND(): TerminalNode | undefined { return this.tryGetToken(JackParser.AND, 0); }
	public OR(): TerminalNode | undefined { return this.tryGetToken(JackParser.OR, 0); }
	public LESS_THAN(): TerminalNode | undefined { return this.tryGetToken(JackParser.LESS_THAN, 0); }
	public GREATER_THAN(): TerminalNode | undefined { return this.tryGetToken(JackParser.GREATER_THAN, 0); }
	public EQUALS(): TerminalNode | undefined { return this.tryGetToken(JackParser.EQUALS, 0); }
	constructor(parent: ParserRuleContext | undefined, invokingState: number) {
		super(parent, invokingState);
	}
	// @Override
	public override get ruleIndex(): number { return JackParser.RULE_binaryOperator; }
	// @Override
	public override enterRule(listener: JackParserListener): void {
		if (listener.enterBinaryOperator) {
			listener.enterBinaryOperator(this);
		}
	}
	// @Override
	public override exitRule(listener: JackParserListener): void {
		if (listener.exitBinaryOperator) {
			listener.exitBinaryOperator(this);
		}
	}
	// @Override
	public override accept<Result>(visitor: JackParserVisitor<Result>): Result {
		if (visitor.visitBinaryOperator) {
			return visitor.visitBinaryOperator(this);
		} else {
			return visitor.visitChildren(this);
		}
	}
}


