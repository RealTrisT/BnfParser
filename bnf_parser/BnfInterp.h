#pragma once

#include "TokenGen.h"
#include "BnfCompile.h"

struct FilterRange {
	FilterRange(unsigned index_start, unsigned index_end) : index_start(index_start), index_end(index_end) {}
	FilterRange(unsigned index) : index_start(index), index_end(index+1) {}
	unsigned index_start, index_end;
};

struct BnfInterp : TokenGen{

	Token* GetTokens(const char* s, void* data){
		rules = (std::vector<BnfCompile::Rule>*)data;
		const char* temp = 0;
		Token* result = recurse(0, 0, s, &temp);
		if (!result)return 0;
		if (recursion_fix) { result->remove_recursive(); }
		return result;
	}


	std::vector<BnfCompile::Rule>* rules;
	
	bool recursion_fix = false;
	std::vector<FilterRange> plucks;
	std::vector<FilterRange> purges;

	Token* recurse(unsigned rule_index, Token* parent, const char* string, const char** end) {
		Token* token = new Token{ 0, 0, parent, 0, string, 0, rule_index };
		Token* child = (Token*)((char*)token + offsetof(Token, child));								//child points to the child element of token, so ( &child->next == &token->child )
		BnfCompile::Rule& rule = (*rules)[rule_index];
		const char* location = string;
		unsigned element_index = 0;
		unsigned group_index = 0;
		bool works = false;
		
		//printf("recurs [%s] %p\n", rule.name.c_str(), string);
		while (element_index != rule.groups[group_index]) {
			BnfCompile::Element& e = rule.elements[element_index];
			switch (e.type) {
				case BnfCompile::Element::TYPE_LITERAL:{
					auto size = e.literal_text.size();
					if (works = !e.literal_text.compare(0, size, location, size)) {
						location += size;
					}break;
				}
				case BnfCompile::Element::TYPE_NATIVE: {
					unsigned size = 0;
					if (works = e.native(location, &size)) {
						location += size;
					}break;
				}
				case BnfCompile::Element::TYPE_RECURSE: {
					Token* result = recurse(e.rule_index, token, location, &location);
					if (works = result) {
						result->prev = child;
						child = child->next = result;
					}break;
				}
			}

			element_index++;

			if (!works) {
				location = string;
				element_index = rule.groups[group_index++]; 
				if (token->child) {
					child = (Token*)((char*)token + offsetof(Token, child));
					token->child->destroy();
					token->child = 0;
				}

				if (group_index == rule.groups.size()) {
					break;
				} else {
					works = true;
				}
			}
		}

		if (works) {
			if (token->child) {
				token->child->prev = 0;
				token->child->prev = child;
			}
			token->len = location - string;
			*end = location;
			return token;
		} else {
			token->destroy();
			return 0;
		}
	}

};