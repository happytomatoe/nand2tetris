parser grammar JackParser;

options {
	tokenVocab = JackLexer;
}

program: classDeclaration EOF;

classDeclaration:
	CLASS className LBRACE classVarDec* subroutineDec* RBRACE;
className: IDENTIFIER;
classVarDec:
	STATIC fieldList SEMICOLON	# staticFieldDeclaration
	| FIELD fieldList SEMICOLON	# fieldDeclaration;
fieldList: varType fieldName ( COMMA fieldName)*;
fieldName: IDENTIFIER;
subroutineDec:
	CONSTRUCTOR subroutineDecWithoutType	# constructor
	| METHOD subroutineDecWithoutType		# method
	| FUNCTION subroutineDecWithoutType		# function;
subroutineDecWithoutType:
	subroutineReturnType subroutineName LPAREN parameterList RPAREN subroutineBody;
subroutineName: IDENTIFIER;
subroutineReturnType: varType | VOID;

varType: INT | CHAR | BOOLEAN | IDENTIFIER;

parameterList: (parameter (COMMA parameter)*)?;
parameter: varType parameterName;
parameterName: IDENTIFIER;
subroutineBody:
	LBRACE varDec* statements returnStatement RBRACE;

varDec: VAR varType varName (COMMA IDENTIFIER)* SEMICOLON;
varName: IDENTIFIER;
statements: statement*;
statement:
	letStatement
	| ifStatement
	| whileStatement
	| doStatement;

letStatement:
	LET (IDENTIFIER | arrayAccess) EQUALS <assoc=right> expression SEMICOLON;

ifStatement:
	IF LPAREN expression RPAREN LBRACE statements RBRACE (
		ELSE LBRACE statements RBRACE
	)?;

whileStatement:
	WHILE LPAREN expression RPAREN LBRACE statements RBRACE;

doStatement: DO subroutineCall SEMICOLON;

subroutineCall:
	IDENTIFIER LPAREN expressionList RPAREN
	| (IDENTIFIER | THIS_LITERAL) DOT IDENTIFIER LPAREN expressionList RPAREN;

returnStatement: RETURN expression? SEMICOLON;

expressionList: (expression (COMMA expression)*)?;

expression:
	binaryOperation = expression binaryOperator expression 
	| constant
	| IDENTIFIER
	| subroutineCall
	| arrayAccess
	| unaryOp
	| groupedExpression;

groupedExpression: LPAREN expression RPAREN;
unaryOp: unaryOperator expression;
arrayAccess: IDENTIFIER LBRACKET expression RBRACKET;

constant:
	INTEGER_LITERAL
	| STRING_LITERAL
	| BOOLEAN_LITERAL
	| NULL_LITERAL
	| THIS_LITERAL;

unaryOperator: TILDE | MINUS;
binaryOperator:
	PLUS
	| MINUS
	| MUL
	| DIV
	| AND
	| OR
	| LESS_THAN
	| GREATER_THAN
	| EQUALS;