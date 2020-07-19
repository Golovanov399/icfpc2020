#include <bits/stdc++.h>
#include "term.h"
#include "int2str.h"
#include "human.h"

#define all(x) (x).begin(), (x).end()
#define itn int
#define make_unique(x) sort((x).begin(), (x).end()); (x).erase(unique((x).begin(), (x).end()), (x).end())

using namespace std;

const map<string, string> renames = {
	{":1141", ":take"},	// :take list idx = list[idx]
	{":1128", ":length"},	// :length list = |list|
	{":1126", ":map"},	// :map list f = [f(x) for x in list]
};

map<string, Term*> readGalaxy(const string& filename = "galaxy.txt") {
	map<string, Term*> term_dict;
	ifstream fin(filename);
	string line;
	while (getline(fin, line)) {
		auto tokens = split(line);
		auto lhs = tokens[0];
		tokens.erase(tokens.begin(), tokens.begin() + 2);
		if (renames.count(lhs)) {
			lhs = renames.at(lhs);
		}
		for (auto& s : tokens) {
			if (renames.count(s)) {
				s = renames.at(s);
			}
		}
		term_dict[lhs] = buildTerm(tokens);
	}
	return term_dict;
}

const map<string, Term*> term_dict = readGalaxy();

auto term_t = new Term("t");
auto term_f = new Term("f");

Term* try_eval(Term*);
Term* eval(Term*);

Term* try_eval(Term* term) {
	if (term->evaluated) {
		return term->evaluated;
	} else if (!term->name.empty() && term_dict.count(term->name)) {
		return term_dict.at(term->name);
	} else if (term->name.empty()) {
		auto f = eval(term->left);
		auto x = term->right;
		if (!f->name.empty()) {
			if (f->name == "neg") {
				try {
					return new Term(to_string(-stoli(eval(x)->name)));
				} catch (const invalid_argument&) {
					return term;
				}
			} else if (f->name == "i") {
				return x;
			} else if (f->name == "nil") {
				return term_t;
			} else if (f->name == "isnil") {
				return new Term(x, new Term(term_t, new Term(term_t, term_f)));
			} else if (f->name == "car") {
				return new Term(x, term_t);
			} else if (f->name == "cdr") {
				return new Term(x, term_f);
			}
		} else {
			auto f2 = eval(f->left);
			auto y = f->right;
			if (!f2->name.empty()) {
				if (f2->name == "t") {
					return y;
				} else if (f2->name == "f") {
					return x;
				} else if (f2->name == "add") {
					try {
						return new Term(to_string(stoli(eval(y)->name) + stoli(eval(x)->name)));
					} catch (const invalid_argument&) {
						return term;
					}
				} else if (f2->name == "mul") {
					try {
						return new Term(to_string(stoli(eval(y)->name) * stoli(eval(x)->name)));
					} catch (const invalid_argument&) {
						return term;
					}
				} else if (f2->name == "div") {
					try {
						return new Term(to_string(stoli(eval(y)->name) / stoli(eval(x)->name)));
					} catch (const invalid_argument&) {
						return term;
					}
				} else if (f2->name == "eq") {
					try {
						return new Term(stoli(eval(y)->name) == stoli(eval(x)->name) ? "t" : "f");
					} catch (const invalid_argument&) {
						return term;
					}
				} else if (f2->name == "lt") {
					try {
						return new Term(stoli(eval(y)->name) < stoli(eval(x)->name) ? "t" : "f");
					} catch (const invalid_argument&) {
						return term;
					}
				} else if (f2->name == "cons") {
					Term* cons_term = new Term(new Term(new Term("cons"), eval(y)), eval(x));
					cons_term->evaluated = cons_term;
					return cons_term;
				}
			} else {
				auto f3 = eval(f2->left);
				auto z = f2->right;
				if (!f3->name.empty()) {
					if (f3->name == "s") {
						return new Term(new Term(z, x), new Term(y, x));
					} else if (f3->name == "c") {
						return new Term(new Term(z, x), y);
					} else if (f3->name == "b") {
						return new Term(z, new Term(y, x));
					} else if (f3->name == "cons") {
						return new Term(new Term(x, z), y);
					}
				} else {
					return term;
				}
			}
		}
	}
	return term;
}

Term* eval(Term* term) {
	if (term->evaluated) {
		return term->evaluated;
	}
	auto init_term = term;
	while (true) {
		auto res = try_eval(term);
		if (res == term) {
			init_term->evaluated = res;
			return res;
		}
		term = res;
	}
}

Term* eval(const string& s) {
	return eval(buildTerm(split(s)));
}

int main() {
	string s;
	getline(cin, s);
	// cout << term2data(eval(s)) << "\n";
	cerr << str2data(s) << "\n";
	// cerr << term2data(eval("ap ap galaxy ap ap cons 1 ap ap cons ap ap cons 1 nil ap ap cons 0 ap ap cons nil nil ap ap cons 14 88"))->get(2) << "\n";
	// cerr << demodulate("11111010000110100011111101010100011111101100001101000111111011000101010001011111010001010100001111110100001101000011111010101000011111011000111010000111111010001101011111010000101011110110000101011110110001101011111010001101100001111101001100001111101100001011000011111011000100110000111111010001001100010111110100001011000111111010011000111111011000010110001100") << "\n";

	return 0;
}
