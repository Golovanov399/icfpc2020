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

int main(int argc, char** argv) {
	string mode = "eval";
	if (argc > 1) {
		mode = argv[1];
	}
	string s;
	getline(cin, s);

	if (mode == "galaxy") {
		string state = s.substr(0, s.find(':'));
		string point = s.substr(s.find(':') + 1);
		cout << term2data(eval(new Term(new Term(new Term("galaxy"), data2term(str2data(state))), data2term(str2data(point))))) << "\n";
	} else if (mode == "eval") {
		cout << term2data(eval(s)) << "\n";
	} else if (mode == "reduce") {
		cout << eval(s) << "\n";
	} else if (mode == "apcons") {
		cout << data2term(str2data(s)) << "\n";
	} else if (mode == "modulate") {
		cout << modulate(data2term(str2data(s))) << "\n";
	} else if (mode == "demodulate") {
		cout << term2data(demodulate(s)) << "\n";
	} else {
		cerr << "weird mode\n";
	}

	return 0;
}
