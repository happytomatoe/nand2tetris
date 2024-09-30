import { SubroutineIdContext } from "../generated/JackParser";
import { LocalSymbolTable } from "../symbol";

export interface CallTypeResult {
    callType: CallType;
    subroutineIdText: string;
}

export enum CallType {
    VarMethod = 1,
    LocalMethod = 2,
    ClassFunctionOrConstructor = 3
}
export function getCallType(subroutineId: SubroutineIdContext, className: string, localSymbolTable: LocalSymbolTable): CallTypeResult {
    if (subroutineId.DOT() == undefined) {
        //local method
        return {
            callType: CallType.LocalMethod,
            subroutineIdText: className + "." + subroutineId.subroutineName().text
        } as CallTypeResult;
    } else {
        // var method
        const [varName, methodName] = subroutineId.text.split('.')
        const symbol = localSymbolTable.lookup(varName)
        if (symbol != undefined) {
            return {
                callType: CallType.VarMethod,
                subroutineIdText: symbol.type + "." + methodName
            } as CallTypeResult;
        } else {
            // class function/ctor
            return {
                callType: CallType.ClassFunctionOrConstructor,
                subroutineIdText: subroutineId.text
            } as CallTypeResult;
        }
    }
}