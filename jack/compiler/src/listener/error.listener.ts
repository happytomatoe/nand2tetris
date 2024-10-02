import { ErrorListener, RecognitionException, Recognizer } from 'antlr4';
import { JackCompilerError, LexerOrParserError } from '../error';
export class CustomErrorListener extends ErrorListener<any> {
    static instance: CustomErrorListener;
    public filepath: string = "";
    public errors: JackCompilerError[] = [];

    /**
     * Provides a default instance of {@link ConsoleErrorListener}.
    */
    override syntaxError = (recognizer: Recognizer<any>, offendingSymbol: any, line: number, column: number, msg: string, e: RecognitionException | undefined) => {
        this.errors.push(new LexerOrParserError(this.filepath, line, column, msg));
        console.error(`${this.filepath}:${line}:${column} \n${msg}`);
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
