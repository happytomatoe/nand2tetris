import { ErrorListener, RecognitionException, Recognizer, Token } from 'antlr4';
import { JackCompilerError, LexerOrParserError } from '../error';
export class CustomErrorListener<TSymbol> extends ErrorListener<TSymbol> {
    public errors: JackCompilerError[] = [];

    /**
     * Provides a default instance of {@link ConsoleErrorListener}.
    */
    override syntaxError = (recognizer: Recognizer<TSymbol>, offendingSymbol: TSymbol, line: number, column: number, msg: string, e: RecognitionException | undefined) => {
        const t = offendingSymbol as Token;
        this.errors.push(new LexerOrParserError(line, column, t.stop, msg));
    };
    // reportAmbiguity(recognizer: any, dfa: any, startIndex: any, stopIndex: any, exact: any, ambigAlts: any, configs: any) {
    //     console.log(`Ambiguity detected at ${this.filepath}:${startIndex}:${stopIndex}`);
    // }

    // //mandatory method to be able to run error listener
    // reportAttemptingFullContext(recognizer: any, dfa: any, startIndex: any, stopIndex: any, conflictingAlts: any, configs: any) {
    //     console.log(`Attempting full context at ${this.filepath}:${startIndex}:${stopIndex}`);
    // }

    // reportContextSensitivity(recognizer: any, dfa: any, startIndex: any, stopIndex: any, prediction: any, configs: any) {
    //     console.log(`Context sensitivity detected at ${this.filepath}:${startIndex}:${stopIndex}`);
    // }
}