#pragma once

#include "term.h"
#include "int2str.h"

using namespace std;

string modulate(LI x) {
	const LI& one = 1;
	string res = "";
	if (x < 0) {
		res += "10";
		x = -x;
	} else {
		res += "01";
	}
	int sz = 0;
	while ((x + 1) > (one << (4 * sz))) {
		sz += 1;
	}
	res += string(sz, '1');
	res += '0';
	for (int i = 0; i < 4 * sz; ++i) {
		res += (char)('0' + x % 2);
		x /= 2;
	}
	reverse(res.end() - 4 * sz, res.end());
	return res;
}

string modulate(Term* term) {
	if (!term->name.empty()) {
		if (term->name == "nil") {
			return "00";
		} else {
			return modulate(stoli(term->name));
		}
	} else {
		assert(term->left->name.empty());
		assert(term->left->left->name == "cons");
		return "11" + modulate(term->left->right) + modulate(term->right);
	}
}

enum Type {
	Num = 0,
	Point = 1,
	List = 2
};

struct Data {
	Type type;
	LI val;
	Data *head;
	Data *tail;

	Data* get(int idx) {
		assert(type == List);
		if (idx == 0) {
			return head;
		} else {
			return tail->get(idx - 1);
		}
	}
};

Data* term2data(Term* term) {
	if (!term->name.empty()) {
		if (term->name == "nil") {
			return new Data({List, LI(0), nullptr, nullptr});
		} else {
			return new Data({Num, stoli(term->name), nullptr, nullptr});
		}
	} else {
		assert(term->left->name.empty());
		assert(term->left->left->name == "cons");
		auto right = term2data(term->right);
		return new Data({right->type == List ? List : Point, LI(0), term2data(term->left->right), right});
	}
}

ostream& operator <<(ostream& ostr, Data* data) {
	if (data->type == Num) {
		return ostr << to_string(data->val);
	} else if (data->type == Point) {
		return ostr << "(" << data->head << ", " << data->tail << ")";
	} else if (!data->head) {
		return ostr << "[]";
	} else {
		ostr << "[";
		while (data->tail->head) {
			ostr << data->head << ", ";
			data = data->tail;
		}
		return ostr << data->head << "]";
	}
}