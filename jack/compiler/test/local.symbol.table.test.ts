import { LocalSymbolTable, VariableSymbol, ScopeType, SubroutineScope } from "../src/symbol";

describe('LocalSymbolTable', () => {
    const jestConsole = console;

    beforeEach(() => {
        global.console = require('console');
    });

    afterEach(() => {
        global.console = jestConsole;
    });

    test('add', () => {
        const localSymbolTable = new LocalSymbolTable();
        const variableNames = ['a1', 'a2', 'a3', "a4", "a5"];
        const types = ["int", "boolean", "char", "String"];
        const scopes = [ScopeType.Argument, ScopeType.Local, ScopeType.This, ScopeType.Static];
        variableNames.forEach(variableName => {
            const type = types[Math.floor(Math.random() * types.length)];
            const scope = scopes[Math.floor(Math.random() * scopes.length)];
            localSymbolTable.define(scope, variableName, type)
            expect(localSymbolTable.lookup(variableName)).toEqual({ name: variableName, type, scope } as VariableSymbol);
            expect(localSymbolTable.lookup(variableName + "_")).toBeUndefined();
        });
    })


    test('shadowing', () => {
        const localSymbolTable = new LocalSymbolTable();
        localSymbolTable.define(ScopeType.This, "a", "int")
        expect(localSymbolTable.lookup("a")).toEqual({ name: "a", type: "int", scope: ScopeType.This } as VariableSymbol);
        localSymbolTable.define(ScopeType.Argument, "a", "int")
        expect(localSymbolTable.lookup("a")).toEqual({ name: "a", type: "int", scope: ScopeType.Argument } as VariableSymbol);
    })

    test('pop stack', () => {
        const localSymbolTable = new LocalSymbolTable();
        localSymbolTable.define(ScopeType.This, "a", "int")
        expect(localSymbolTable.lookup("a")).toEqual({ name: "a", type: "int", scope: ScopeType.This } as VariableSymbol);
        localSymbolTable.popStack();
        localSymbolTable.define(ScopeType.Argument, "a", "int")
        expect(localSymbolTable.lookup("a")).toEqual({ name: "a", type: "int", scope: ScopeType.Argument } as VariableSymbol);
    })

    test('function vars', () => {
        const localSymbolTable = new LocalSymbolTable();
        const arg = { name: "a", type: "int", scope: ScopeType.Argument } as VariableSymbol
        const localVar = { name: "b", type: "int", scope: ScopeType.Local } as VariableSymbol
        localSymbolTable.define(ScopeType.Argument, "a", "int")
        localSymbolTable.define(ScopeType.Local, "b", "int")
        expect(localSymbolTable.popStack()).toEqual({ arguments: [arg], locals: [localVar] } as SubroutineScope)

    })
});