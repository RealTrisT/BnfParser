#include "BnfInterp.h";

BnfInterp::Element::Element(unsigned rule_index) : type(TYPE_RECURSE), rule_index(rule_index) {}
BnfInterp::Element::Element(std::string literal_text) : type(TYPE_LITERAL), literal_text(literal_text) {}
BnfInterp::Element::Element(const Element& o) : type(o.type) {
	if (type == TYPE_LITERAL) {
		new(&literal_text) std::string(o.literal_text);
	}
	else {
		this->rule_index = o.rule_index;
	}
}

BnfInterp::Element::~Element() {
	if (type == TYPE_LITERAL) {
		literal_text.~basic_string();
	}
}


bool BnfInterp::compile_from_tokens(TokenGen::Token* tokens) {
	if (!tokens)return false;
	if (!this->ready_rules(tokens)) {
		rules.clear();
		return false;
	}
	for (unsigned i = this->rules_start_index; tokens; tokens = tokens->next, i++) {
		if (!compile_rule(tokens, this->rules[i])) {
			rules.clear();
			return false;
		}
	}
	return true;
}

bool BnfInterp::get_rule_index(const char* s, unsigned s_len, unsigned* o_index) {
	for (unsigned i = 0; i < rules.size(); i++) {						//for each rule in our rules vector
		if (!rules[i].name.compare(0, s_len, s, s_len)) {				//we're gonna compare it to what we have
			puts("true");
			*o_index = i;
			return true;
		}else puts("false");
	}
	return false;
}

bool BnfInterp::compile_rule(TokenGen::Token* token, Rule& rule_expr) {
	if (
		!token->child					//rule name
		||
		!token->child->next				//assignment operator
		||
		!token->child->next->next		//first in the expression
		) {
		return false;
	}

	TokenGen::Token* curr_token = token->child->next->next;

	for (; curr_token; curr_token = curr_token->next) {
		if (curr_token->ID == (unsigned)TokenType::RULE_NAME) {
			unsigned rule_index = 0;
			if (!get_rule_index(curr_token->begin + 1, curr_token->len - 2, &rule_index))
				return false;
			rule_expr.elements.push_back(Element(
				rule_index
			));
		}
		else if (curr_token->ID == (unsigned)TokenType::LITERAL) {
			rule_expr.elements.push_back(Element{
				std::string(curr_token->begin + 1, curr_token->len - 2)
				});
		}
		else if (curr_token->ID == (unsigned)TokenType::OR_OPERATOR) {
			rule_expr.groups.push_back((unsigned)rule_expr.elements.size());
		}
	}
	return true;
}

bool BnfInterp::ready_rules(TokenGen::Token* token_list) {
	for (unsigned i = 0; token_list; token_list = token_list->next, i++) {		//now for each token (rule)
		if (!token_list->child)
			return false;
		rules.push_back({														//create a rule object in it
			std::string(token_list->child->begin + 1, token_list->child->len - 2),
			std::vector<Element>(),
			std::vector<unsigned>()
			});
	}
	return true;
}