#pragma once

using namespace std;

using LI = __int128_t;
LI stoli(const string& s) {
	if (s.empty()) {
		throw invalid_argument("qwe");
	}
	int sign = 1;
	if (s[0] == '-') {
		sign = -1;
	}
	LI res = 0;
	for (int i = (sign == -1); i < (int)s.length(); ++i) {
		if (!isdigit(s[i])) {
			throw invalid_argument("qwe");
		}
		res = res * 10 + (s[i] - '0');
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
	if (res.empty()) {
		res = "0";
	}
	reverse(res.begin(), res.end());
	if (sign < 0) {
		res = "-" + res;
	}
	return res;
}
