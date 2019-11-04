#pragma once

#include "TokenGen.h"

struct BaseBnf : TokenGen {
	enum struct TokenType : unsigned{
		RULE,
		RULE_NAME,
		LITERAL,
		LITERAL_TEXT,
		ASSIGNMENT_OPERATOR,
		OR_OPERATOR
	};

	Token* GetTokens(const char*);
};