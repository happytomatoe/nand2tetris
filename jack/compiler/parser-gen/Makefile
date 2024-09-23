trace:
	antlr4-parse  JackLexer.g4 JackParser.g4  program -tokens -trace< test.jack

tokens:
	antlr4-parse JackLexer.g4 JackParser.g4 program  -tokens < test.jack

gui:
	antlr4-parse JackLexer.g4  JackParser.g4   program -gui  < test.jack
gen:
	antlr4 JackLexer.g4 JackParser.g4 -o ./output