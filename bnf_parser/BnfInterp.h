#pragma once

#include "TokenGen.h"
#include "BaseBnf.h"

#include <string>
#include <vector>

struct BnfInterp{

	struct Element {
		enum Type{
			TYPE_RECURSE,
			TYPE_LITERAL,
		};

		Element(unsigned rule_index);
		Element(std::string literal_text);
		Element(const Element& o);

		~Element();

		Type type;
		union {
			std::string literal_text;
			unsigned rule_index;
		};
	};

	struct Rule {
		std::string name;
		std::vector<Element> elements;
		std::vector<unsigned> groups;
	};

	bool compile_from_tokens(TokenGen::Token* tokens);
private:
	bool get_rule_index(const char* s, unsigned s_len, unsigned* o_index);
	bool compile_rule(TokenGen::Token* token, Rule& rule_expr);
	bool ready_rules(TokenGen::Token* token_list);

	unsigned rules_start_index = 0;
public:
	std::vector<Rule> rules = {};
};
