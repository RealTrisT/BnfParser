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

		void pluck() {
			bool has_next = this->next;
			bool has_child = this->child;
			bool has_parent = this->parent;
			bool is_first_child = has_parent && this->parent->child == this;
			bool is_single_child = this->prev == this;

			if (has_next) {
				this->next->prev = has_child ? this->child->prev : this->prev;	//next->prev handled
			}if (is_first_child) {
				this->parent->child = has_child ? this->child : this->next;		//parent->child handled
			}if (has_child) {
				auto child = this->child;										//children->parents handled
				for (;child; child = child->next)child->parent = this->parent;							
				this->child->prev->next = this->next;							//lastchild->next handled
				if(!is_single_child) this->child->prev = this->prev;			//firstchild->prev handled (only if we're not single child, since otherwise we'd point to ourselves)
			}if (!is_first_child) {
				this->prev->next = has_child ? this->child : this->next;		//prev->next handled
			}

			this->next = this->child = 0;
			this->destroy();
		}

		void remove_recursive() {
			if (this->next) this->next ->remove_recursive();
			if (this->child)this->child->remove_recursive();
			if(this->parent && this->parent->ID == this->ID)this->pluck();
		}
	};

	virtual Token* GetTokens(const char*, void*) = 0;
 };