#pragma once

using ld = long double;

struct pt {
	ld x, y;

	pt operator *(const ld& k) const {
		return {x * k, y * k};
	}
};

struct SVG {
	FILE *out;
	ld sc = 50;

	explicit SVG(const string& filename) {
		out = fopen(filename.c_str(), "w");
		fprintf(out, "<svg xmlns='http://www.w3.org/2000/svg' viewBox='-1000 -1000 3000 3000'>\n");
	}

	void line(pt a, pt b) {
		a = a * sc, b = b * sc;
		fprintf(out, "<line x1='%Lf' y1='%Lf' x2='%Lf' y2='%Lf' stroke='black'/>\n", a.x, a.y, b.x, b.y);
	}

	void circle(pt a, ld r = -1, string col = "red") {
		r = (r == -1 ? 10 : sc * r);
		a = a * sc;
		fprintf(out, "<circle cx='%Lf' cy='%Lf' r='%Lf' fill='%s'/>\n", a.x, a.y, r, col.c_str());
	}

	void text(pt a, string s) {
		a = a * sc;
		fprintf(out, "<text x='%Lf' y='%Lf' font-size='10px'>%s</text>\n", a.x, a.y, s.c_str());
	}

	void close() {
		fprintf(out, "</svg>\n");
		fclose(out);
		out = 0;
	}

	~SVG() {
		if (out) {
			close();
		}
	}
};
