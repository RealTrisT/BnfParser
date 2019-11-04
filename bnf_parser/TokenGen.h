#pragma once
struct TokenGen {
	struct Token {
		Token* next, * prev, * parent, * child;
		const char* begin;
		unsigned len;
		unsigned ID;

		void destroy() {
			if(this->next) this->next ->destroy();
			if(this->child)this->child->destroy();
			delete this;
		}
	};

	virtual Token* GetTokens(const char*) = 0;
 };