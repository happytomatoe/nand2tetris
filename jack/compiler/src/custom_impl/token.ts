export enum TokenType {
    //Keywords
    class = 'class',
    constructor = 'constructor',
    method = 'method',
    function = 'function',
    int = 'int',
    boolean = 'boolean',
    char = 'char',
    void = 'void',
    var = 'var',
    static = 'static',
    field = 'field',
    let = 'let',
    do = 'do',
    if = 'if',
    else = 'else',
    while = 'while',
    return = 'return',
    this = 'this',

    //symbols
    LeftParen = '(',
    RightParen = ')',
    LeftSquareBracket = '[',
    RightSquareBracket = ']',
    LeftBrace = '{',
    RightBrace = '}',

    Comma = ',',
    Semicolon = ';',
    Equals = '=',
    PeriodDot = '.',

    Plus = '+',
    Minus = '-',
    Multiply = '*',
    Divide = '\\',

    And = '&',
    Or = '|',
    Tilde = '~',
    LessThan = '<',
    GreaterThan = '>',
    // Colon = ':',

    //Literals
    NumberConstant = 'NumberConstant',
    StringConstant = 'StringConstant',
    BooleanConstant = 'BooleanConstant',
    NullConstant = 'NullConstant',

    Identifier = 'Identifier',

    EOF = 'EOF'
}


export class Token {
    //TODO: should we change api to be like in ts scanner?
    type: TokenType;
    literal: number | string | boolean | undefined;
    line: number;
    constructor(type: TokenType, line: number, literal?: string | number | boolean) {
        this.type = type;
        this.literal = literal;
        this.line = line;
    }
}