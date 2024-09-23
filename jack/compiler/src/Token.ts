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
        return `[Token ${TokenType[this.type]}, line ${this.line}, literal '${this.literal}']`;
    }
}

// export function toString(type: TokenType): string {
//     const c = nameof<TokenType>(type);
//     return c;
// }
