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
export type VariableSymbol = {
    name: string;
    type: VariableType;
    scope: ScopeType;
}

export type SubroutineScope = {
    arguments: VariableSymbol[];
    locals: VariableSymbol[];
}
/**
 *   Symbol table that provides lookup for variables in different scopes in a file
 */
export class LocalSymbolTable {
    //create scopes hierarchy
    private classVars: Record<string, VariableSymbol> = {}
    constructor(private subroutineVars: SubroutineScope = { arguments: [], locals: [] } as SubroutineScope) { }
    lookup(name: string): VariableSymbol | undefined {
        const localVar = this.subroutineVars.locals.find(v => v.name === name)
        if (localVar != undefined) return localVar;

        const sym = this.subroutineVars.arguments.find(v => v.name === name)
        if (sym != undefined) return sym;

        return this.classVars[name];
    }

    //define symbol in scope
    define(scope: ScopeType, varName: string, type: VariableType) {
        if (scope === ScopeType.Static || scope == ScopeType.This) {
            this.classVars[varName] = { name: varName, type, scope }
        } else if (scope === ScopeType.Argument) {
            this.subroutineVars.arguments.push({ name: varName, type, scope })
        } else if (scope === ScopeType.Local) {
            this.subroutineVars.locals.push({ name: varName, type, scope })
        } else {
            throw new Error("Invalid scope: " + scope)
        }
    }

    popStack() {
        const oldScope = this.subroutineVars
        this.subroutineVars = { arguments: [], locals: [] } as SubroutineScope;
        return oldScope;
    }
}