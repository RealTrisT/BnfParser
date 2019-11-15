

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

	const char* b = R"bnfsyntax(
		<response>			::= <status-line> <message-headers> <CRLF>
		<status-line>		::= <http-version> " " <status-code> " " <reason-phrase> <CRLF>
		<http-version>		::= "HTTP/1.1"
		<status-code>		::= <digit> <digit> <digit>
		<reason-phrase>		::= <TEXT>

		<message-headers>	::= <message-header> <CRLF> <message-headers> | <message-header> <CRLF> | ""
		<message-header>	::= <field-name> ":" <field-value>
		<field-value>		::= <TEXT> | ""
		<field-name>		::= <token>

		<TEXT>				::= <TEXT-CHAR> <TEXT> | <TEXT-CHAR>
		<TEXT-CHAR>			::= <letter> | <digit> | <any-symbol>
		<any-symbol>		::= <token-symbol> | <separator>

		<token>				::= <token-char> <token> | <token-char>
		<token-char>		::= <letter> | <digit> | <token-symbol>
		<token-symbol>		::= "!" | "#" | "$" | "%" | "&" | "'" | "*" | "+" | "-" | "." | "^" | "_" | "`" | "|" | "~"

		<CRLF>				::= <CR> <LF>

		<letter>			::= "A" | "B" | "C" | "D" | "E" | "F" | "G" | "H" | "I" | "J" | "K" | "L" | "M" | "N" | "O" | "P" | "Q" | "R" | "S" | "T" | "U" | "V" | "W" | "X" | "Y" | "Z" | "a" | "b" | "c" | "d" | "e" | "f" | "g" | "h" | "i" | "j" | "k" | "l" | "m" | "n" | "o" | "p" | "q" | "r" | "s" | "t" | "u" | "v" | "w" | "x" | "y" | "z"
		<digit>				::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9"
		<separator>			::= "(" | ")" | "<" | ">" | "@" | "," | ";" | ":" | '"' | "/" | "[" | "]" | "?" | "=" | "{" | "}" | " " | "	"  | "\"
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
		{
			"CR", 
			{
				{
					[](const char* s, unsigned* l_o) -> bool {return *l_o = (*s == '\r'); }
				}
			},
			{
				1
			}
		},
		{
			"LF",
			{
				{
					[](const char* s, unsigned* l_o) -> bool {return *l_o = (*s == '\n'); }
				}
			},
			{
				1
			}
		},
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

	const char* string =
		"HTTP/1.1 200 OK\r\n"
		"Date: Mon, 27 Jul 2009 12 : 28 : 53 GMT\r\n"
		"Server: Apache / 2.2.14 (Win32)\r\n"
		"Last-Modified: Wed, 22 Jul 2009 19 : 15 : 56 GMT\r\n"
		"Content-Length: 88\r\n"
		"Content-Type: text / html\r\n"
		"Connection: Closed\r\n"
		"\r\n"
	;
	printf("STRING: %p\n", string);

	BnfInterp interp = BnfInterp();
	interp.recursion_fix = true;
	auto tkn = interp.GetTokens(string, &comp.rules);
	//tkn->remove_recursive();


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