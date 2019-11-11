#pragma once

#include "TokenGen.h"

enum struct TokenType : unsigned {
	RULE,
	RULE_NAME,
	LITERAL,
	LITERAL_TEXT,
	ASSIGNMENT_OPERATOR,
	OR_OPERATOR
};

struct BaseBnf : TokenGen {
	

	Token* GetTokens(const char*, void*);
};