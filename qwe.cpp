#include <bits/stdc++.h>

#define all(x) (x).begin(), (x).end()
#define itn int
#define make_unique(x) sort((x).begin(), (x).end()); (x).erase(unique((x).begin(), (x).end()), (x).end())

using namespace std;
#include "svg.h"

inline int nxt() {
	int x;
	scanf("%d", &x);
	return x;
}

struct Term {
	int id;
	string name; // if a terminal
	Term *left, *right; // if an ap
	bool is_non_recursive;

	explicit Term(const string& _name):
			id(-1),
			name(_name),
			left(nullptr),
			right(nullptr),
			is_non_recursive(false) {}
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

void write_svg(Term* term, const string& filename) {
	vector<pair<Term*, int>> stack_with_layers;
	vector<vector<string>> names;
	stack_with_layers.push_back({term, 0});
	while (!stack_with_layers.empty()) {
		auto [term, layer] = stack_with_layers.back();
		stack_with_layers.pop_back();
		if (layer >= (int)names.size()) {
			names.emplace_back();
		}
		names[layer].push_back(term->name.empty() ? "ap" : term->name);
		if (term->name.empty()) {
			stack_with_layers.push_back({term->right, layer + 1});
			stack_with_layers.push_back({term->left, layer + 1});
		}
	}
	int n = names.size();
	ld w = 50;
	vector<int> cur(n);
	stack_with_layers.push_back({term, 0});
	vector<pt> pars = {{-1., -1.}};
	SVG svg(filename);
	while (!stack_with_layers.empty()) {
		auto [term, layer] = stack_with_layers.back();
		stack_with_layers.pop_back();
		pt up = pars.back();
		pars.pop_back();
		pt center = {w * (cur[layer] + 0.5) / names[layer].size(), layer + 0.5};
		svg.text({center.x - 0.1, center.y}, names[layer][cur[layer]]);
		if (up.x > -0.5) {
			svg.line(up, {center.x, center.y - 0.2});
		}
		if (term->name.empty()) {
			stack_with_layers.push_back({term->right, layer + 1});
			pars.push_back({w * (cur[layer] + 0.5) / names[layer].size(), center.y + 0.2});
			stack_with_layers.push_back({term->left, layer + 1});
			pars.push_back({w * (cur[layer] + 0.5) / names[layer].size(), center.y + 0.2});
		}
		++cur[layer];
	}
}

using LI = __int128_t;
LI stoli(const string& s) {
	int sign = 1;
	if (s[0] == '-') {
		sign = -1;
	}
	LI res = 0;
	for (int i = (sign == -1); i < (int)s.length(); ++i) {
		res = res * 10 + s[i];
	}
	return res * sign;
}

string to_string(LI a) {
	int sign = 1;
	if (a < 0) {
		sign = -1;
		a *= -1;
	}
	string res = "";
	while (a) {
		res += (char)('0' + a % 10);
		a /= 10;
	}
	reverse(all(res));
	if (sign < 0) {
		res = "-" + res;
	}
	return res;
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

const vector<string> typed_rules = {
	"ap neg x",
	"ap ap add x y",
	"ap ap mul x y",
};

vector<pair<vector<string>, vector<string>>> untyped_rules_tokens;
vector<vector<string>> typed_rules_tokens;
void fill_rules_tokens() {
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
	for (auto s : typed_rules) {
		auto tkns = split(s);
		typed_rules_tokens.push_back(tkns);
	}
}

bool doesRuleFit(const vector<string>& rule, const string& keyterm, int& ptr, const Term* term) {
	if (!term) {
		return false;
	}
	if (rule[ptr] == "ap") {
		if (!term->name.empty()) {
			return false;
		}
		++ptr;
		if (!doesRuleFit(rule, keyterm, ptr, term->left)) {
			return false;
		}
		if (!doesRuleFit(rule, keyterm, ptr, term->right)) {
			return false;
		}
		return true;
	} else if (rule[ptr] == keyterm) {
		if (term->name != keyterm) {
			return false;
		}
		++ptr;
		return true;
	} else {
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

vector<Term*> topsort;
map<string, Term*> term_dict;

bool check(Term* term) {
	if (term->name.empty()) {
		if (!term->left) {
			return false;
		}
		if (!term->right) {
			return false;
		}
	}
	return true;
}

map<Term*, char> used;
void mark_non_recursiveness(Term* term) {
	if (used[term]) {
		return;
	}
	used[term] = 1;
	if (!term->name.empty()) {
		if (term_dict.count(term->name)) {
			mark_non_recursiveness(term_dict[term->name]);
			term->is_non_recursive = term_dict[term->name]->is_non_recursive;
		} else {
			term->is_non_recursive = true;
		}
	} else {
		mark_non_recursiveness(term->left);
		mark_non_recursiveness(term->right);
		term->is_non_recursive = term->left->is_non_recursive && term->right->is_non_recursive;
	}
	if (term->is_non_recursive) {
		topsort.push_back(term);
	}
}

bool replaceAllRules(Term*& term) {
	if (!term) {
		return false;
	}
	if (!term->name.empty()) {
		return false;
	}
	bool res = false;
	if (!check(term)) {
		assert(false);
	}
	res |= replaceAllRules(term->left);
	res |= replaceAllRules(term->right);
	for (const auto& [lhs, rhs] : untyped_rules_tokens) {
		int ptr = 0;
		string keyterm = "";
		for (auto s : lhs) {
			if (s != "ap") {
				keyterm = s;
				break;
			}
		}
		if (doesRuleFit(lhs, keyterm, ptr, term)) {
			map<string, Term*> vars;
			ptr = 0;
			storeRuleVars(lhs, ptr, term, vars);
			ptr = 0;
			term = buildByRhs(rhs, ptr, vars);
			res = true;
		}
	}
	for (const auto& rule : typed_rules_tokens) {
		int ptr = 0;
		string keyterm = "";
		for (auto s : rule) {
			if (s != "ap") {
				keyterm = s;
				break;
			}
		}
		if (doesRuleFit(rule, keyterm, ptr, term)) {
			map<string, Term*> vars;
			ptr = 0;
			storeRuleVars(rule, ptr, term, vars);
			try {
				if (keyterm == "neg") {
					term = new Term(to_string(-stoli(vars["x"]->name)));
				} else if (keyterm == "add") {
					term = new Term(to_string(stoli(vars["x"]->name) + stoli(vars["y"]->name)));
				} else if (keyterm == "mul") {
					term = new Term(to_string(stoli(vars["x"]->name) * stoli(vars["y"]->name)));
				} else {
					assert(false);
				}
				if (!check(term)) {
					assert(false);
				}
				term->is_non_recursive = true;
				res = true;
			} catch (const invalid_argument&) {
				//
			}
		}
	}
	if (!check(term)) {
		assert(false);
	}

	return res;
}

map<pair<int, int>, int> id_map;
vector<Term*> term_by_id;

void expand_term_dicts(Term*& term) {
	if (term->name == "") {
		expand_term_dicts(term->left);
		expand_term_dicts(term->right);
	} else if (term_dict.count(term->name)) {
		term = term_dict[term->name];
	}
}

void expand_nonrec_term_dicts(Term*& term) {
	if (term->name == "") {
		expand_nonrec_term_dicts(term->left);
		expand_nonrec_term_dicts(term->right);
	} else if (term_dict.count(term->name) && term_dict[term->name]->is_non_recursive) {
		term = term_dict[term->name];
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
	fill_rules_tokens();

	// for (int it = 0; it < 10; ++it) {
	// 	cerr << term_dict["galaxy"] << "\n";
	// 	write_svg(term_dict["galaxy"], "out.svg");
	// 	expand_term_dicts(term_dict["galaxy"]);
	// 	while (replaceAllRules(term_dict["galaxy"]));
	// }

	for (auto& p : term_dict) {
		mark_non_recursiveness(p.second);
	}
	// for (const auto& p : term_dict) {
	// 	cerr << p.first << ": " << !!p.second->is_non_recursive << "\n";
	// }

	for (auto term : topsort) {
		// cerr << "reducing " << term << "...\n";
		while (true) {
			expand_nonrec_term_dicts(term);
			bool changed = false;
			while (replaceAllRules(term)) {
				changed = true;
			}
			if (!changed) {
				break;
			}
		}
		// cerr << "reduced: " << term << "\n";
	}

	/*{
		cerr << term_dict[":1202"] << "\n";
		expand_nonrec_term_dicts(term_dict[":1202"]);
		cerr << term_dict[":1202"] << "\n";
		while (replaceAllRules(term_dict[":1202"])) {
			cerr << term_dict[":1202"] << "\n";
		}
		write_svg(term_dict[":1202"], "out.svg");
	}*/

	// int times_changed = 0;
	// while (true) {
	// 	used.clear();
	// 	for (auto& p : term_dict) {
	// 		mark_non_recursiveness(p.second);
	// 	}
	// 	bool changed = false;
	// 	for (auto& p : term_dict) {
	// 		if (p.second->is_non_recursive) {
	// 			expand_nonrec_term_dicts(p.second);
	// 			while (replaceAllRules(p.second)) {
	// 				changed = true;
	// 			}
	// 		}
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
