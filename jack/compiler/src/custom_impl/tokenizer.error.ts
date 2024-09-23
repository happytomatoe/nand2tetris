export default class TokenizerError extends Error {
    line: number;
    constructor(line: number, msg: string) {
        super(msg);
        this.line = line;

        // Set the prototype explicitly.
        Object.setPrototypeOf(this, TokenizerError.prototype);
    }
    static unexpectedToken(line: number, token: string): TokenizerError {
        return new TokenizerError(line, ErrorMessages.unexpectedToken + `: \`${token}\``);
    }
}

export enum ErrorMessages {
    unterminatedString = 'Unterminated string',
    unexpectedToken = 'Unexpected token',

}