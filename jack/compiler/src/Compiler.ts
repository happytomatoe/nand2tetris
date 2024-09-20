import Tokenizer from "./Tokenizer";

export default class Compiler {
    static compile(source: string): string {
        console.log("Compiling", source);
        const l = new Tokenizer(source);
        const tokens = l.tokenize();
        console.log("Tokens:", tokens);
        return "";
    }
}