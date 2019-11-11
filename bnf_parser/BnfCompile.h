#pragma once

#include "TokenGen.h"
//#include "BaseBnfParse.h"

#include <string>
#include <vector>

struct BnfCompile{

	struct Element {
		enum Type{
			TYPE_RECURSE,
			TYPE_LITERAL,
			TYPE_NATIVE
		};

		Element(unsigned rule_index);
		Element(std::string literal_text);
		Element(bool(*native)(const char*, unsigned*));
		Element(const Element& o);

		~Element();

		Type type;
		union {
			std::string literal_text;
			unsigned rule_index;
			bool(*native)(const char*, unsigned*);
		};
	};

	struct Rule {
		std::string name;
		std::vector<Element> elements;
		std::vector<unsigned> groups;
	};

public:
	void ImplementNatives(Rule* rules, unsigned amount);
	bool CompileFromTokens(TokenGen::Token* tokens);
	//TokenGen::Token* Interpret(const char* s, bool recursion_fix);

private:
	bool GetRuleIndex(const char* s, unsigned s_len, unsigned* o_index);
	bool CompileRule(TokenGen::Token* token, Rule& rule_expr);
	bool ReadyRules(TokenGen::Token* token_list);


public:
	std::vector<Rule> rules = {};
public:
	unsigned rules_start_index = 0;
};
