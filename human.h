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

Data* parse_str_2_data(const string& s, int l, int r, const vector<int>& opposite) {
	if (s[l] == '[') {
		assert(s[r - 1] == ']');
		Data* res = new Data({List, LI(0), nullptr, nullptr});
		Data* cur = res;
		for (int i = l + 1; i < r - 1;) {
			if (opposite[i] != -1) {
				cur->head = parse_str_2_data(s, i, opposite[i] + 1, opposite);
				cur->tail = new Data({List, LI(0), nullptr, nullptr});
				cur = cur->tail;
				i = opposite[i] + 2;
			} else {
				int j = i;
				if (s[j] == '-') {
					++j;
				}
				while (isdigit(s[j])) {
					j += 1;
				}
				cur->head = parse_str_2_data(s, i, j, opposite);
				cur->tail = new Data({List, LI(0), nullptr, nullptr});
				cur = cur->tail;
				i = j + 1;
			}
		}
		return res;
	} else if (s[l] == '(') {
		assert(s[r - 1] == ')');
		Data* res = new Data({Point, LI(0), nullptr, nullptr});
		int i = l + 1;
		{
			int j = i;
			if (s[j] == '-') {
				++j;
			}
			while (isdigit(s[j])) {
				j += 1;
			}
			res->head = parse_str_2_data(s, i, j, opposite);
			i = j + 1;
		}
		{
			int j = i;
			if (s[j] == '-') {
				++j;
			}
			while (isdigit(s[j])) {
				j += 1;
			}
			res->tail = parse_str_2_data(s, i, j, opposite);
			i = j + 1;
		}
		return res;
	} else {
		return new Data({Num, stoli(s.substr(l, r - l)), nullptr, nullptr});
	}
}

Data* str2data(string s) {
	{
		string t;
		for (char c : s) {
			if (!isspace(c)) {
				t += c;
			}
		}
		s = t;
	}
	vector<int> opposite(s.length(), -1);
	vector<int> st;
	for (int i = 0; i < (int)s.length(); ++i) {
		if (s[i] == '(' || s[i] == '[') {
			st.push_back(i);
		} else if (s[i] == ')' || s[i] == ']') {
			opposite[i] = st.back();
			opposite[st.back()] = i;
			st.pop_back();
		}
	}
	assert(st.empty());
	return parse_str_2_data(s, 0, (int)s.length(), opposite);
}

Term* data2term(Data* data) {
	if (data->type == Num) {
		return new Term(to_string(data->val));
	} else if (!data->head) {
		return new Term("nil");
	} else {
		return new Term(new Term(new Term("cons"), data2term(data->head)), data2term(data->tail));
	}
}

void demodulate_token(vector<string>& tkns, const string& s, int& i) {
	if (s.substr(i, 2) == "00") {
		tkns.push_back("nil");
		i += 2;
	} else if (s.substr(i, 2) == "11") {
		tkns.push_back("ap");
		tkns.push_back("ap");
		tkns.push_back("cons");
		i += 2;
	} else {
		int sign = (s.substr(i, 2) == "01") ? 1 : -1;
		i += 2;
		int sz = 0;
		while (s[i] == '1') {
			++i;
			++sz;
		}
		++i;
		LI x = 0;
		for (int j = 0; j < 4 * sz; ++j) {
			x = x * 2 + (s[i++] - '0');
		}
		tkns.push_back(to_string(x * sign));
	}
}

vector<string> demodulate_to_tokens(const string& s) {
	vector<string> res;
	for (int i = 0; i < (int)s.length();) {
		demodulate_token(res, s, i);
	}
	return res;
}

Term* demodulate(const string& s) {
	return buildTerm(demodulate_to_tokens(s));
}

ostream& operator <<(ostream& ostr, Data* data) {
	if (data->type == Num) {
		return ostr << to_string(data->val);
	} else if (data->type == Point) {
		return ostr << "(" << data->head << ", " << data->tail << ")";
	} else {
		ostr << "[";
		bool fp = true;
		while (data->head) {
			if (fp) {
				fp = false;
			} else {
				ostr << ", ";
			}
			ostr << data->head;
			data = data->tail;
		}
		return ostr << "]";
	}
}