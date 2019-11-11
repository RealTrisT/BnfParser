

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
		<nibba> ::= "1" | "B" | "what u doin homi"
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
	comp.ImplementNatives(natives, 2);

	if (!comp.CompileFromTokens(t)){
		puts("bygfak");
	} else {
		puts("compiled successfully\n");
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	BnfInterp interp = BnfInterp();
	auto tkn = interp.GetTokens("what u doin homi", &comp.rules);

	printf("%p [\"%.*s\", '%s']\n", tkn, tkn->len, tkn->begin, comp.rules[tkn->ID].name.c_str());





	getchar();
	getchar();

	return 0;
}