#pragma once

using namespace std;

struct Term {
	string name;
	Term *evaluated;
	Term *left, *right;

	explicit Term(const string& _name): name(_name), evaluated(nullptr), left(nullptr), right(nullptr) {}
	explicit Term(Term* _left, Term* _right): name(""), evaluated(nullptr), left(_left), right(_right) {}
};

ostream& operator <<(ostream& ostr, const Term* term) {
	if (term->name == "") {
		ostr << "ap ";
		ostr << term->left;
		ostr << " ";
		ostr << term->right;
		return ostr;
	} else {
		return ostr << term->name;
	}
}

vector<string> split(const string& s) {
	vector<string> res = {""};
	for (char c : s) {
		if (isspace(c)) {
			if (!res.back().empty()) {
				res.push_back("");
			}
		} else {
			res.back() += c;
		}
	}
	return res;
}

void build_rec(const vector<string>& tokens, int& ptr, Term*& res) {
	if (tokens[ptr] == "ap") {
		res = new Term("");
		++ptr;
		build_rec(tokens, ptr, res->left);
		build_rec(tokens, ptr, res->right);
	} else {
		res = new Term(tokens[ptr]);
		++ptr;
	}
}

Term* buildTerm(const vector<string>& tokens) {
	Term* res = nullptr;
	int ptr = 0;
	build_rec(tokens, ptr, res);
	return res;
}
