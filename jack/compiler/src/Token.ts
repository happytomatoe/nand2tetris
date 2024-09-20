export enum TokenType {
    //Keywords
    class, constructor, method, function,  //Program components
    int, boolean, char, void,  //Primitive types
    var, static, field,   //Variable declarations
    let, do, if, else, while, return, //Statements
    true, false, null,  //Constant values
    this,  //Object reference



    //symbols
    LeftParen,//(
    RightParen,//)
    LeftSquareBracket,//[
    RightSquareBracket,//]
    LeftBrace,//{
    RightBrace,//}


    Comma,//,
    Semicolon,//;
    Equals, //=
    PeriodDot,//.

    Plus,//+
    Minus,//-
    Multiply,//*
    Divide, // /

    And,//&
    Or,//|
    Tilde,//~
    LessThan,//<
    GreaterThan,//>
    //TODO: find out where this is used
    Colon,//:


    //Literals
    NumberConstant,
    StringConstant,
    BooleanConstant,
    NullConstant,
    //first char [a-zA-Z_]
    //[A-Za-z0-9_]
    Identifier,

    EOF
}

export class Token {
    type: TokenType;
    literal: string | undefined;
    line: number;
    constructor(type: TokenType, line: number, literal?: string) {
        this.type = type;
        this.literal = literal;
        this.line = line;
    }
    toString(): string {
        return `[Token ${TokenType.toString(this.type)}, line ${this.line}, literal '${this.literal}']`;
    }
}

export namespace TokenType {

    export function toString(type: TokenType): string {
        switch (type) {
            case TokenType.LeftParen: return "LeftParen";
            case TokenType.RightParen: return "RightParen";
            case TokenType.LeftSquareBracket: return "LeftSquareBracket";
            case TokenType.RightSquareBracket: return "RightSquareBracket";
            case TokenType.LeftBrace: return "LeftBrace";
            case TokenType.RightBrace: return "RightBrace";
            case TokenType.Comma: return "Comma";
            case TokenType.Semicolon: return "Semicolon";
            case TokenType.Equals: return "Equals";
            case TokenType.PeriodDot: return "PeriodDot";
            case TokenType.Plus: return "Plus";
            case TokenType.Minus: return "Minus";
            case TokenType.Multiply: return "Multiply";
            case TokenType.Divide: return "Divide";
            case TokenType.And: return "And";
            case TokenType.Or: return "Or";
            case TokenType.Tilde: return "Tilde";
            case TokenType
                .LessThan: return "LessThan";
            case TokenType.GreaterThan: return "GreaterThan";
            case TokenType.NumberConstant: return "NumberConstant";
            case TokenType.StringConstant: return "StringConstant";
            case TokenType.BooleanConstant: return "BooleanConstant";
            case TokenType.NullConstant: return "NullConstant";
            case TokenType.Identifier: return "Identifier";
            case TokenType.EOF: return "EOF";
            default:
                throw new Error(`Unknown token type: ${type}`);
        }
    }
}