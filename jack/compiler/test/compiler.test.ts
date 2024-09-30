import { Compiler } from "../src/compiler";

describe("Compiler", () => {
    test('empty class', () => {
        const input = `class A{}`
        const expected = ``;
    })

    test('static field', () => {
        const input = `class A{
            static int a;
            function void init(){
                let a=1;
                return;
            }
        }`
        const expected = `

        `;
    })
    test('field', () => {
        const input = `
        class A{
            field int a;
            method void init(){
                let a=1;
                return;
            }
        }`
        const expected = `

        `;
    })

    test('empty  constructor', () => {
        const input = `class A{
            constructor A new(){
                return this;
            }
        }`
        const expected = `

        `;
    })
    test('empty function', () => {
        const input = `
        class A{
            function void init(){
                return;
            }
        }
        `
        const expected = `

        `;
    })
    test('empty method', () => {
        const input = `
            class A{
                method void init(){
                    return;
                }
            }`
        const expected = `
            
        `;
    })

    test('constructor with parameters', () => {
        const input = ``
        const expected = `
        
        `;
    })
    test('method with parameters', () => {
        const input = `
            class A{
                method void init(int a, boolean b){
                    return;
                }
            }
         `
        const expected = `

        `;
    })
    test('function with parameters', () => {
        const input = `
        class A{
        function void init(int a, boolean b){
                    return;
                }
        }
         `
        const expected = `

        `;
    })
    //Assign
    test('boolean literal assign', () => {
        const input = `
        class A{
            function void a(){
                var boolean b;
                let b = true;
                return;
            }
        }`
        const expected = `
            function A.a 1
                //true(-1)
                push constant 1
                neg
                //let b =
                pop local 0
                //return
                push constant 0
                return
        `;
        testCompiler(input, expected);

    })
    test('boolean literal assign using logical operators', () => {
        const input = `
        class A{
            function void a(){
                var boolean b;
                let b = (1>2) & (3<4) | (5>6) & (7=7);
                return;
            }
        }`
        const expected = `
           function A.a 1
                //1>2
                push constant 1
                push constant 2
                gt
                //3<4
                push constant 3
                push constant 4
                lt
                and
                //5>6
                push constant 5
                push constant 6
                gt
                or
                //7=7
                push constant 7
                push constant 7
                eq
                and
                //let b =
                pop local 0
                //return
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })
    test('char literal assign', () => {
        const input = `
        class A{
            function void a(){
                var char b;
                let b = 1;
                return;
            }
        }`
        const expected = `
            function A.a 1
                push constant 1
                pop local 0
                push constant 0
                return
        `;
    })
    test('null literal assign', () => {
        const input = `
        class A{
            function void a(){
                var A b;
                let b = null;
                return;
            }
        }
         `
        const expected = `
            function A.a 1
                push constant 0
                pop local 0
                push constant 0
                return
        `;
    })
    test('this literal assign', () => {
        const input = `
        class A{
            method void a(){
                var A b;
                let b = this;
                return;
            }
        }
         `
        const expected = `
        function A.a 1
            // first arg is this
            push argument 0
            pop pointer 0
            // let b= this
            push pointer 0
            pop local 0
            //return
            push constant 0
            return
        `;
    })
    test('int literal assign', () => {
        const input = `
        class A{
            function void a(){
                var int b;
                let b = 1;
                return;
            }
        }`
        const expected = `
            function A.a 1
                push constant 1
                pop local 0
                push constant 0
                return
        `;
          testCompiler(input, expected);
    })
    test('string literal assign', () => {
        const input = `
        class A{
            function void a(){
                var String b;
                let b = "hello";
                return;
            }
        }
         `
        const expected = `
        function A.a 1
            // string length
            push constant 5
            //create string array
            call String.new 1
            //append chars one by one
            push constant 104
            call String.appendChar 2
            push constant 101
            call String.appendChar 2
            push constant 108
            call String.appendChar 2
            push constant 108
            call String.appendChar 2
            push constant 111
            call String.appendChar 2
            //leb b=
            pop local 0
            //return
            push constant 0
            return
        `;
    })
    test('assignment for binary operation', () => {
        const input = `
        class A{
            function void a(){
                var int b;
                let b = 2*3;
                return;
            }
        }
         `
        const expected = `
            function A.a 1
                push constant 2
                push constant 3
                call Math.multiply 2
                pop local 0
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })
    test('assignment for varname', () => {
        const input = `
        class A{
            function void a(){
                var boolean b,c;
                let c=true;
                let b=c;
                return;
            }
        }
         `
        const expected = `
            function A.a 2
                // create true value (-1)
                push constant 1
                neg
                // assign to c
                pop local 1
                //let b=c
                push local 1
                pop local 0
                //return
                push constant 0
                return
        `;
    })
    test('assignment for subroutine call', () => {
        const input = `
        class A{
            function int a(){
                return 1;
            }
            function void b(){
                var boolean b;
                let b = A.a();
                return;
            }
        }
         `
        const expected = `
            function A.a 0
                push constant 1
                return
            function A.b 1
                call A.a 0
                //let b =
                pop local 0
                //return
                push constant 0
                return
        `;
    })
    test('assignment for array access', () => {
        //TODO: add let arr[1] = arr[0];
        const input = `
        class A{
            function void a(){
                var Array arr;
                let arr = Array.new(10);
                let arr[5] = 1;
                return;
            }
        }`
        const expected = `
           function A.a 1
                // let arr = Array.new(10);
                push constant 10
                call Array.new 1
                pop local 0
                // let arr[5] = 1;
                push local 0
                pop pointer 1
                push constant 1
                pop that 5
                //return
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })
    test('assignment to -1', () => {
        const input = `
        class A{
            function void a(){
                var int b;
                let b = -1;
                return;
            }
        }`
        const expected = `
            function A.a 1
                push constant 1
                neg
                pop local 0
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })
    test('assignment to ~1', () => {
        const input = `
        class A{
            function void a(){
                var int b;
                let b = ~1;
                return;
            }
        }`
        const expected = `
            function A.a 1
                //  ~1;
                push constant 1
                not
                // let b =
                pop local 0
                //return
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })
    test('assignment for grouped expr', () => {
        const input = `
        class A{
            function void a(){
                var int b;
                let b = 2*(3+5);
                return;
            }
        }`
        const expected = `
            function A.a 1
                push constant 2
                push constant 3
                push constant 5
                add
                call Math.multiply 2
                pop local 0
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })

    test('if statement', () => {
        const input = `
        class A{
            function void a(){
                var boolean b;
                let b = true;
                if(b){
                    let b = false;
                }
                return;
            }
        }`
        const expected = `
            function A.a 1
                //let b=true
                push constant 1
                neg
                pop local 0
                //if(b)
                push local 0
                not
                //go to end of if
                if-goto A_0
                //let b=false
                push constant 0
                pop local 0
            label A_0
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })
    test('if else statement', () => {
        const input = `
        class A{
            function void a(){
                var boolean b;
                var int a;
                let b = true;
                if(b){
                    let a = 1;
                }else{
                    let a = 2;
                }
                return;
            }
        }`
        const expected = `
            function A.a 2
                //let b=true
                push constant 1
                neg
                pop local 0
                //if(b)
                push local 0
                not
                //go to else
                if-goto A_0
                push constant 1
                pop local 1
                // go to end of if else
                goto A_1
            label A_0
                push constant 2
                pop local 1
            label A_1
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })
    test('while constant', () => {
        const input = `
        class A{
            function void a(){
                var int i;
                let i = 1;
                while(i<10){
                    let i=i+1;
                }
                return;
            }
        }`
        const expected = `
            function A.a 1
                // let i = 1;
                push constant 1
                pop local 0
            //while
            label A_0
                // i<10
                push local 0
                push constant 10
                lt
                // if not go to end 
                not
                if-goto A_1
                // let i=i+1;
                push local 0
                push constant 1
                add
                pop local 0
                // go to beginning
                goto A_0
            label A_1
                //return
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })

    test('function call', () => {
        const input = `
        class A{
            function void b(){
                return;
            }
            function void a(){
                do A.b();
                return;
            }
        }`
        const expected = `
            function A.b 0
                push constant 0
                return
            function A.a 0
                call A.b 0
                pop temp 0
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })
    test('method call', () => {
        const input = `
        class A{
            method void b(){
                return;
            }
            method void a(){
                do b();
                return;
            }
        }`
        const expected = `
            function A.b 0
                push argument 0
                pop pointer 0
                push constant 0
                return
            function A.a 0
                push argument 0
                pop pointer 0
                push pointer 0
                //call
                call A.b 1
                //save result in temp
                pop temp 0
                //return
                push constant 0
                return
        `;
        testCompiler(input, expected);
    })
    test('constructor', () => {
        const input = `
        class A{
            constructor A new(){
                return this;
            }
        }`
        const expected = `
            function A.new 0   
            //create this     
            push constant 0
            call Memory.alloc 1
            pop pointer 0
            //return this
            push pointer 0
            return
        `;
        testCompiler(input, expected);
    })

    test('return literal', () => {
        const input = `
        class A{
            method int a(){
                return 1;
            }
        }`
        const expected = `
        function A.a 0
            // push this
            push argument 0
            pop pointer 0
            //return 1
            push constant 1
            return
        `;
        testCompiler(input, expected);
    })
    test('method return void', () => {
        const input = `
        class A{
            method void a(){
                return;
            }
        }`
        const expected = `
        function A.a 0
            push argument 0
            pop pointer 0
            //return
            push constant 0
            return
        `;
        testCompiler(input, expected);
    })

    test('return simple expression', () => {
        const input = `
        class A{
            function int a(){
                return 1+2*3;
            }
        }`
        const expected = `
            function A.a 0
                push constant 1
                push constant 2
                //1+2
                add
                push constant 3
                //3*3
                call Math.multiply 2
                return
        `;
        testCompiler(input, expected);
    })
})


function trimMultiline(text: string): string {
    return text.split("\n").map(line => line.trim())
        .filter(str => str.replace(/[\s\t]/g, '').length != 0)
        .join("\n")
}
function deleteComments(text: string): string {
    return text.split("\n")
        .filter(str => str.indexOf("//") == -1)
        .join("\n")
}

const compose = <T>(fn1: (a: T) => T, ...fns: Array<(a: T) => T>) =>
    fns.reduce((prevFn, nextFn) => value => prevFn(nextFn(value)), fn1);

const trimAndDeleteComments = compose(trimMultiline, deleteComments);
function testCompiler(input: string, expected: string): void {
    const res = Compiler.compile(input)
    if (Array.isArray(res)) {
        throw new Error(`Unexpected compilation errors: ${res.join("\n")}`);
    } else {
        expect(trimAndDeleteComments(res)).toEqual(trimAndDeleteComments(expected));
    }
}