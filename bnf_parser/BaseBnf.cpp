#include "BaseBnf.h"

unsigned newline(const char* s) {
	unsigned result = 0;
	if (*s == '\r') { s++; result++; }
	if (*s == '\n') { s++; result++; }
	else			{ return 0; }
	return result;
}

unsigned possible_newlines(const char* s) {
	unsigned result = 0;
	while (unsigned newline_len = newline(s)) {
		result += newline_len;
		s += newline_len;
	}
	return result;
}

unsigned possible_spaces(const char* s) {
	unsigned result = 0;
	for (; *s == ' ' || *s == '\t';s++, result++);
	return result;
}

unsigned possible_newlines_or_spaces(const char* s) {
	unsigned result = 0;
	while (unsigned length = possible_newlines(s) || (length = possible_spaces(s))) {
		result += length;
		s += length;
	}
	return result;
}

bool is_letter(const char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool is_number(const char c) {
	return (c >= '0' && c <= '9');
}
bool is_symbol(const char c) {
	return c != '\'' && c != '"' && (c >= ' ' && c <= '/') || (c >= ':' && c <= '@') || (c >= '[' && c <= '`') || (c >= '{' && c <= '~');
}

unsigned rule_name(const char* s) {
	unsigned result = 0;
	if (!is_letter(*(s++ + result++)))return 0;
	while (is_letter(*s) || is_number(*s) || *s == '-') { s++; result++; }
	return result;
}

BaseBnf::Token* literal(const char** s_p, BaseBnf::Token* parent = 0, BaseBnf::Token* prev = 0) {
	const char* s = *s_p;
	bool quotes;
	if (*s == '"')quotes = true;
	else if (*s == '\'')quotes = false;
	else return 0;
	s++;

	while (is_letter(*s) || is_number(*s) || is_symbol(*s) || (quotes && *s == '\'') || (!quotes && *s == '"')) {
		s++;
	}

	if (!((quotes && *s == '"') || (!quotes && *s == '\'')))return 0;
	s++;

	const char* begin = *s_p; *s_p = s;
	return new BaseBnf::Token{
		0,
		prev,
		parent,
		0,
		begin,
		unsigned(s - begin),
		(unsigned)BaseBnf::TokenType::LITERAL
	};
}

BaseBnf::Token* rule_designator(const char** s_p, BaseBnf::Token* parent = 0, BaseBnf::Token* prev = 0) {
	const char* s = *s_p;
	if (*s++ != '<')return 0;
	unsigned rname_len = rule_name(s);
	if (!rname_len)return 0;
	s += rname_len;
	if (*s++ != '>')return 0;

	const char* begin = *s_p; *s_p = s;
	return new BaseBnf::Token{
		0,
		prev,
		parent, 
		0, 
		begin, 
		unsigned(s - begin),
		(unsigned)BaseBnf::TokenType::RULE_NAME
	};
}

BaseBnf::Token* rule(const char** s_p, BaseBnf::Token* parent = 0, BaseBnf::Token* prev = 0) {
	const char* s = *s_p;
	BaseBnf::Token* rule = new BaseBnf::Token{
		0, prev, parent, 0, s, 0, (unsigned)BaseBnf::TokenType::RULE
	};

	BaseBnf::Token* rule_element = rule->child = rule_designator(&s, rule, 0);		//we got the rule name
	if (!rule_element) { rule->destroy(); return false; }
	s += possible_spaces(s);

	if(!((*s++ == ':') && (*s++ == ':') && (*s++ == '='))) { rule->destroy(); return false; }
	rule_element = rule_element->next = new BaseBnf::Token{
		0, rule_element, rule, 0, s-3, 3, (unsigned)BaseBnf::TokenType::ASSIGNMENT_OPERATOR
	};
	s += possible_spaces(s);

	while (true) {//missing literal
		if (*s == '|') {
			rule_element = rule_element->next = new BaseBnf::Token{
				0, rule_element, rule, 0, s++, 1, (unsigned)BaseBnf::TokenType::OR_OPERATOR
			};
		}else if(BaseBnf::Token* next_rule = rule_designator(&s, rule, rule_element)){
			rule_element = rule_element->next = next_rule;
		}else if(BaseBnf::Token* literal_element = literal(&s, rule, rule_element)) {
			rule_element = rule_element->next = literal_element;
		}else if (unsigned nl_len = newline(s)) {
			s += nl_len;
			break;
		}else{
			rule->destroy();
			return false;
		}
		s += possible_spaces(s);
	}
	
	rule->len = s - *s_p;
	*s_p = s;
	return rule;
}


BaseBnf::Token* BaseBnf::GetTokens(const char* s){
	const char* old_s = s;
	Token* begin_token = new Token();
	Token* current_rule = begin_token;

	s += possible_newlines_or_spaces(s);

	while (*s != '\0') {
		Token* new_rule = rule(&s, 0, current_rule);
		if (!new_rule) { begin_token->destroy(); return 0; }
		current_rule = current_rule->next = new_rule;
		s += possible_newlines_or_spaces(s);
	}

	current_rule = begin_token->next;
	delete begin_token;
	return current_rule;

	//ashit;
}
