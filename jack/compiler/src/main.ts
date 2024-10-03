import { Compiler } from "./compiler";
import { ProgramContext } from "./generated/JackParser";

function compile() {
  console.log('Compiling ...');
  const code = document.querySelector<HTMLTextAreaElement>('#input')?.value ?? "";
  const compiler = new Compiler();
  const treeOrErrors = compiler.parserAndBind(code);
  if (Array.isArray(treeOrErrors)) {
    console.error('Compilation errors:', treeOrErrors);
    document.querySelector<HTMLTextAreaElement>('#error')!.textContent = treeOrErrors.map(v => `${v.span.line}:${v.span.start} ${v.msg}`).join("\n");
  } else {
    const tree = treeOrErrors as ProgramContext;
    const res = compiler.compile(tree);
    if (Array.isArray(res)) {
      console.error('Compilation errors:', res);
      document.querySelector<HTMLTextAreaElement>('#error')!.textContent = res.map(v => `${v.span.line}:${v.span.start} ${v.msg}`).join("\n");
    } else {
      console.log(res);
      document.querySelector<HTMLTextAreaElement>('#output')!.value = res;
      document.querySelector<HTMLTextAreaElement>('#error')!.textContent = "";
    }
  } 
}
document.querySelector<HTMLButtonElement>('#compile')?.addEventListener('click', () => compile());

function error(line: number, message: string): string {
  return report(line, "", message);
}

function report(line: number, where: string, message: string): string {
  return `[line ${line} Error  ${where}:${message}`;
}
