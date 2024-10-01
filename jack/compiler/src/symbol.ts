/**
 * Generic symbol. Can be used for both class and function symbols
 */
export enum SubroutineType {
    Constructor,
    Function,
    Method,
}
export interface SubroutineInfo {
    paramsCount: number;
    type: SubroutineType
    localVarsCount?: number;
}
/**
 * Symbol that represents class or a subroutine
 */
export interface GenericSymbol {
    //TODO: crate class and function symbols
    subroutineInfo?: SubroutineInfo;
}
export function createSubroutineSymbol(paramsCount: number, type: SubroutineType, localVarsCount?: number): GenericSymbol {
    const s = { paramsCount, type } as SubroutineInfo;
    if (localVarsCount != undefined) {
        s.localVarsCount = localVarsCount
    }
    return { subroutineInfo: s } as GenericSymbol
}

type VariableType = string
export enum ScopeType {
    Static,
    This,
    Argument,
    Local,
}

const scopeTypeToStringMap: Record<ScopeType, string> = {
    [ScopeType.Static]: "static",
    [ScopeType.This]: "this",
    [ScopeType.Argument]: "argument",
    [ScopeType.Local]: "local",
}
export function scopeTypeToString(scopeType: ScopeType): string {
    if (scopeTypeToStringMap[scopeType] === undefined) {
        throw new Error(`Unknown scope type: ${scopeType}`)
    }
    return scopeTypeToStringMap[scopeType];
}
export type VariableSymbol = {
    name: string;
    type: VariableType;
    scope: ScopeType;
    index: number;
}

export type SubroutineScope = {
    arguments: VariableSymbol[];
    locals: VariableSymbol[];
}
/**
 *   Symbol table that provides lookup for variables in different scopes in a file
 */
export class LocalSymbolTable {
    private scopes: Record<ScopeType, VariableSymbol[]> = {
        [ScopeType.Static]: [],
        [ScopeType.This]: [],
        [ScopeType.Argument]: [],
        [ScopeType.Local]: [],
    }
    private static scopesLookupOrder = [ScopeType.Local, ScopeType.Argument, ScopeType.This, ScopeType.Static]
    constructor(subroutineScope?: SubroutineScope) {
        if (subroutineScope != undefined) {
            this.scopes[ScopeType.Argument] = subroutineScope.arguments;
            this.scopes[ScopeType.Local] = subroutineScope.locals;
        }
    }
    lookup(name: string): VariableSymbol | undefined {
        for (let scope of LocalSymbolTable.scopesLookupOrder) {
            const symbol = this.scopes[scope].find(v => v.name == name)
            if (symbol != undefined) return symbol
        }
        return undefined;
    }

    //define symbol in scope
    define(scope: ScopeType, varName: string, type: VariableType) {
        this.scopes[scope].push({ name: varName, type, scope, index: this.scopes[scope].length });
    }

    popStack() {
        const f = { arguments: this.scopes[ScopeType.Argument], locals: this.scopes[ScopeType.Local] } as SubroutineScope;
        this.scopes[ScopeType.Local] = []
        this.scopes[ScopeType.Argument] = []
        return f;
    }
}