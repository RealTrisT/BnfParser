

#include <cstdio>

#include "BaseBnfParse.h"
#include "BnfCompile.h"


const char* names[] = {
	"RULE",
	"RULE_NAME",
	"LITERAL",
	"LITERAL_TEXT",
	"ASSIGNMENT_OPERATOR",
	"OR_OPERATOR"
};

int main() {
	BaseBnf a = BaseBnf();

	const char* b = R"bnfsyntax(
		<syntax>			::= <opt-nl-ws> <rule> <syntax> | <opt-nl-ws> <rule> <EOF>

		<rule>				::= "<" <rule-name> ">" <opt-whitespace> "::=" <opt-whitespace> <expression> <opt-whitespace> <newline>

		<expression>		::= <list> <opt-nl-ws> "|" <opt-whitespace> <expression> | <list>
		<list>				::= <term> <opt-whitespace> <list> | <term>

		<term>				::= <literal> | "<" <rule-name> ">"



		<opt-nl-ws>			::= <newlines> <opt-nl-ws> | <whitespaces> <opt-nl-ws> | ""

		<opt-newline>		::= <newlines> | ""
		<newlines>			::= <newline> <newlines> | <newline>
		<newline>			::= <EOL>

		<opt-whitespace>	::= <whitespaces> | ""
		<whitespaces>		::= <whitespace> <whitespaces> | <whitespace>
		<whitespace>		::= "	" | " "

		<literal>			::= "'" <literal-text-q> "'" | '"' <literal-text-a> '"' | "'" "'" | '"' '"'

		<literal-text-q>	::= <character-q> <literal-text-q> | <character-q>
		<character-q>		::= <letter> | <digit> | <symbol> | '"'

		<literal-text-a>	::= <character-a> <literal-text-a> | <character-a>
		<character-a>		::= <letter> | <digit> | <symbol> | "'"

		<rule-name>			::= <letter> <mid-rule-name> | <letter>
		<mid-rule-name>		::= <rule-char> <mid-rule-name> | <rule-char>
		<rule-char>			::= <letter> | <digit> | "-"
	
		<letter>			::= "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
		<digit>				::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
		<symbol>			::= "|" | " " | "!" | "#" | "$" | "%" | "&" | "(" | ")" | "*" | "+" | "," | "-" | "." | "/" | ":" | ";" | ">" | "=" | "<" | "?" | "@" | "[" | "\" | "]" | "^" | "_" | "`" | "{" | "}" | "~"
	
	)bnfsyntax";

	TokenGen::Token* t = a.GetTokens(b, 0), *c = t;
	

	int depth = 0;
	for (; c; c = c->next) {
		printf("::%.*s", c->len, c->begin);
		auto each = c->child;
		for (; each; each = each->next) {
			printf("%s[%.*s] ", names[each->ID], each->len, each->begin);
		}
		printf("\n\n");
	}puts("\n");


	BnfInterp::Rule natives[] = {
		{"EOF", {{[](const char* s, unsigned* l_o) -> bool {return *l_o = *s == '\0'; }}}, {}},
		{"EOL", {{[](const char* s, unsigned* l_o) -> bool {return *l_o = *s == '\n'; }}}, {}}
	};
	BnfInterp interp = BnfInterp();
	interp.ImplementNatives(natives, 2);

	if (!interp.CompileFromTokens(t))
		puts("bygfak");
	else
		puts("compiled successfully");





	getchar();
	getchar();

	return 0;
}