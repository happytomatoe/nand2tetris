export default class TokenizerError extends Error {
    line: number;
    constructor(line: number, msg: string) {
        super(msg);
        this.line = line;

        // Set the prototype explicitly.
        Object.setPrototypeOf(this, TokenizerError.prototype);
    }

}