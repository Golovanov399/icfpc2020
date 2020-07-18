#include <bits/stdc++.h>

#define all(x) (x).begin(), (x).end()
#define itn int
#define make_unique(x) sort((x).begin(), (x).end()); (x).erase(unique((x).begin(), (x).end()), (x).end())

using namespace std;

inline int nxt() {
	int x;
	scanf("%d", &x);
	return x;
}

struct Term {
	int id;
	string name; // if a terminal
	Term *left, *right; // if an ap

	explicit Term(const string& _name): id(-1), name(_name), left(nullptr), right(nullptr) {}
};

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

const vector<string> untyped_rules = {
	"ap ap t x y = x",
	"ap ap f x y = y",
	"ap ap ap s x y z = ap ap x z ap y z",
	"ap ap ap c x y z = ap ap x z y",
	"ap ap ap b x y z = ap x ap y z",
	"ap i x = x",
	"ap ap ap cons x y z = ap ap z x y",
	"ap car z = ap z t",
	"ap cdr z = ap z f",
};

vector<pair<vector<string>, vector<string>>> untyped_rules_tokens;
void fill_untyped_rules_tokens() {
	for (auto s : untyped_rules) {
		auto tkns = split(s);
		untyped_rules_tokens.emplace_back();
		bool fst = true;
		for (auto t : tkns) {
			if (t == "=") {
				assert(fst);
				fst = false;
			} else {
				(fst ? untyped_rules_tokens.back().first : untyped_rules_tokens.back().second).push_back(t);
			}
		}
	}
}

bool doesRuleFit(const vector<string>& rule, int& ptr, Term* term) {
	if (!term) {
		return false;
	}
	if (rule[ptr] == "ap") {
		if (!term->name.empty()) {
			return false;
		}
		++ptr;
		if (!doesRuleFit(rule, ptr, term->left)) {
			return false;
		}
		if (!doesRuleFit(rule, ptr, term->right)) {
			return false;
		}
		return true;
	} else {
		if (term->name.empty()) {
			return false;
		}
		++ptr;
		return true;
	}
}

void storeRuleVars(const vector<string>& rule, int& ptr, Term* term, map<string, Term*>& vars) {
	if (rule[ptr] == "ap") {
		++ptr;
		storeRuleVars(rule, ptr, term->left, vars);
		storeRuleVars(rule, ptr, term->right, vars);
	} else {
		vars[rule[ptr]] = term;
		++ptr;
	}
}

Term* buildByRhs(const vector<string>& rule, int& ptr, const map<string, Term*>& vars) {
	if (rule[ptr] == "ap") {
		auto t = new Term("");
		++ptr;
		t->left = buildByRhs(rule, ptr, vars);
		t->right = buildByRhs(rule, ptr, vars);
		return t;
	} else {
		auto t = vars.at(rule[ptr]);
		++ptr;
		return t;
	}
}

map<Term*, char> used;

bool replaceAllRules(Term*& term) {
	if (!term->name.empty()) {
		return false;
	}
	if (used[term]) {
		return false;
	}
	used[term] = 1;
	bool res = false;
	res |= replaceAllRules(term->left);
	res |= replaceAllRules(term->right);
	for (const auto& [lhs, rhs] : untyped_rules_tokens) {
		int ptr = 0;
		if (doesRuleFit(lhs, ptr, term)) {
			map<string, Term*> vars;
			ptr = 0;
			storeRuleVars(lhs, ptr, term, vars);
			ptr = 0;
			term = buildByRhs(rhs, ptr, vars);
			res = true;
		}
	}
	return true;
}

map<pair<int, int>, int> id_map;
vector<Term*> term_by_id;

map<string, Term*> term_dict;

void expand_term_dicts(Term*& term) {
	if (used[term]) {
		return;
	}
	used[term] = 1;
	if (term->name == "") {
		expand_term_dicts(term->left);
		expand_term_dicts(term->right);
	} else if (term_dict.count(term->name)) {
		term = term_dict[term->name];
	}
}

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

int main() {
	ifstream fin("galaxy.txt");
	string line;
	while (getline(fin, line)) {
		auto tokens = split(line);
		auto lhs = tokens[0];
		tokens.erase(tokens.begin(), tokens.begin() + 2);
		term_dict[lhs] = buildTerm(tokens);
	}
	fill_untyped_rules_tokens();

	// int times_changed = 0;
	// while (true) {
	// 	used.clear();
	// 	for (auto& p : term_dict) {
	// 		expand_term_dicts(p.second);
	// 	}
	// 	bool changed = false;
	// 	used.clear();
	// 	for (auto& p : term_dict) {
	// 		changed |= replaceAllRules(p.second);
	// 	}
	// 	if (!changed) {
	// 		break;
	// 	}
	// 	++times_changed;
	// 	if (times_changed == 10) {
	// 		break;
	// 	}
	// }
	// cerr << times_changed << "\n";

	return 0;
}
