

#include <cstdio>

#include "BaseBnfParse.h"
#include "BnfCompile.h"
#include "BnfInterp.h"


const char* names[] = {
	"RULE",
	"RULE_NAME",
	"LITERAL",
	"LITERAL_TEXT",
	"ASSIGNMENT_OPERATOR",
	"OR_OPERATOR"
};

int main() {
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	BaseBnf a = BaseBnf();

	/*
		<nibba> ::= <a-or-b> " homes, actually tried to pull one on " <a-or-b> " homes"
		<a-or-b> ::= "a" | "b" | "A" | "B"
	*/

	const char* b = R"bnfsyntax(	
		<rule-name>			::= <letter> <mid-rule-name> | <letter>
		<mid-rule-name>		::= <rule-char> <mid-rule-name> | <rule-char>
		<rule-char>			::= <letter> | <digit> | "-"

		<letter>			::= "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
		<digit>				::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
		<symbol>			::= "|" | " " | "!" | "#" | "$" | "%" | "&" | "(" | ")" | "*" | "+" | "," | "-" | "." | "/" | ":" | ";" | ">" | "=" | "<" | "?" | "@" | "[" | "\" | "]" | "^" | "_" | "`" | "{" | "}" | "~"
	)bnfsyntax";

	TokenGen::Token* t = a.GetTokens(b, 0), *c = t;
	
	//--------dbg-------//
						//
	int depth = 0;		//
	for (; c; c = c->next) {
		printf("::           %.*s", c->len, c->begin);
		auto each = c->child;
		for (; each; each = each->next) {
			printf("%s[%.*s] ", names[each->ID], each->len, each->begin);
		}				//
		printf("\n");	//
	}puts("");//--------//

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BnfCompile::Rule natives[] = {
		{"EOF", {{[](const char* s, unsigned* l_o) -> bool {return *l_o = *s == '\0'; }}}, {}},
		{"EOL", {{[](const char* s, unsigned* l_o) -> bool {return *l_o = *s == '\n'; }}}, {}}
	};

	BnfCompile comp = BnfCompile();
	if (!comp.CompileFromTokens(t, natives, 2)){
		puts("bygfak");
	} else {
		printf("compiled successfully with %u rules\n", comp.rules.size());
		for (auto& rule : comp.rules) {
			printf("%s :: ", rule.name.c_str());
			for (auto& element : rule.elements) {
				printf("%s ", element.type == element.TYPE_LITERAL ? "LITERAL" : element.type == element.TYPE_NATIVE ? "NATIVE" : "RECURSE");
			}
			puts("");
		}
		puts("\n");
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BnfInterp interp = BnfInterp();
	auto tkn = interp.GetTokens("hi-this-is-rule-nameNr69", &comp.rules);


	for (char c = getchar(); c != EOF; c = getchar()) {
		switch (c) {
			case 'w':
				if (tkn->parent)tkn = tkn->parent;
				break;
			case 'a':
				if (tkn->prev)tkn = tkn->prev;
				break;
			case 's':
				if (tkn->child)tkn = tkn->child;
				break;
			case 'd':
				if (tkn->next)tkn = tkn->next; 
				break;
			default:
				continue;
		}
		printf("%p [\"%.*s\", '%s']\n", tkn, tkn->len, tkn->begin, comp.rules[tkn->ID].name.c_str());
		printf("%p ", tkn->prev ? tkn->prev : 0);
		printf("%p\n", tkn->next ? tkn->next : 0);
		printf("%p ", tkn->parent ? tkn->parent : 0);
		printf("%p\n", tkn->child ? tkn->child : 0);
	}
	





	getchar();
	getchar();

	return 0;
}