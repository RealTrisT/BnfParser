

#include <cstdio>

#include "BaseBnf.h"


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
		<nem> ::= <mem> | 'YORKEEEEEEEEE' <memnibbas> <yabba>
		<memnibbas> ::= "YORKEEEEEEEEE" | '"' | "'"
	)";
	auto c = a.GetTokens(b);


	//printf("str ptr : %p\n", b);
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