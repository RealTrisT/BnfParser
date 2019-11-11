

#include <cstdio>

#include "BaseBnf.h"
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
	BaseBnf a = BaseBnf();

	const char* b = R"( 
		<nem> ::= <memnibbas> | 'YORKEEEEEEEEE' <memnibbas> <nem>
		<memnibbas> ::= "YORKEEEEEEEEE" | '"' | "'"
	)";

	TokenGen::Token* c = a.GetTokens(b);
	BnfInterp interp = BnfInterp();
	if(!interp.compile_from_tokens(c))
		puts("bygfak");
	
	

	//printf("%u\n", interp.rules[0].elements[0].rule_index);

	int depth = 0;
	for (; c; c = c->next) {
		auto each = c->child;
		for (; each; each = each->next) {
			printf("%s[%.*s] ", names[each->ID], each->len, each->begin);
		}
		printf("\n");
	}
	getchar();
	getchar();

	return 0;
}