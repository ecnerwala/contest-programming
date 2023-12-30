#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::cerr;

// We'll represent catalan numbers by strings of ()

using poly_t = std::vector<std::string>;
using perm_t = std::vector<int>;
using tria_t = std::vector<std::array<int, 3>>;

using obj_t = std::variant<poly_t, perm_t, tria_t>;

void print_poly(const poly_t& v) {
	cout << "poly" << '\n';
	cout << int(v.size()) << ' ' << int(v[0].size()) << '\n';
	for (auto& r : v) cout << r << '\n';
}
void print_perm(const perm_t& v) {
	cout << "perm" << '\n';
	for (int i = 0; i < int(v.size()); i++) {
		cout << v[i] << " \n"[i+1==int(v.size())];
	}
}
void print_tria(const tria_t& v) {
	cout << "triang" << '\n';
	for (auto& r : v) cout << r[0] << ' ' << r[1] << ' ' << r[2] << '\n';
}

void print_obj(const obj_t& v) {
	if (std::holds_alternative<poly_t>(v)) {
		print_poly(std::get<poly_t>(v));
	} else if (std::holds_alternative<perm_t>(v)) {
		print_perm(std::get<perm_t>(v));
	} else if (std::holds_alternative<tria_t>(v)) {
		print_tria(std::get<tria_t>(v));
	} else assert(false);
}

poly_t read_poly() {
	int n, m; cin >> n >> m;
	poly_t res(n);
	for (auto& r : res) cin >> r;
	return res;
}
perm_t read_perm(int L) {
	perm_t res(L);
	for (auto& v : res) cin >> v;
	return res;
}
tria_t read_tria(int L) {
	tria_t res(L);
	for (auto& v : res) cin >> v[0] >> v[1] >> v[2];
	return res;
}
obj_t read_obj(int L) {
	std::string type; cin >> type;
	if (type == "poly") {
		return read_poly();
	} else if (type == "perm") {
		return read_perm(L);
	} else if (type == "triang") {
		return read_tria(L);
	} else assert(false);
}

poly_t bracket_to_poly(std::string s) {
	int L = int(s.size()) / 2;
	s = '(' + s + ')';
	poly_t res(L+1, std::string(L+1, '.'));
	int xlo = 0, ylo = 0;
	int xhi = 0, yhi = 0;
	for (int i = 0; i < (L+1); i++) {
		if (s[2*i+0] == '(') {
			xlo++;
		} else {
			ylo++;
		}
		if (s[2*i+1] == '(') {
			yhi++;
		} else {
			xhi++;
		}
		assert(xlo + ylo == i+1);
		assert(xhi + yhi == i+1);
		if (i == L) {
			assert(xlo == xhi && ylo == yhi);
			break;
		}
		assert(xlo > xhi);

		// Now, go do all the diagonals
		for (int x = xlo, y = ylo; x != xhi; ) {
			x--;
			res[x][y] = '#';
			y++;
		}
	}
	res.resize(xhi);
	for (auto& r : res) r.resize(yhi);
	for (auto& r : res) { std::reverse(r.begin(), r.end()); }
	return res;
}

std::string poly_to_bracket(poly_t res) {
	int L = int(res.size()) + int(res[0].size()) - 1;
	for (auto& r : res) { std::reverse(r.begin(), r.end()); }
	int xlo = 0, ylo = 0, xhi = 0, yhi = 0;
	std::string s; s.reserve(1);
	for (int i = 0; i < (L+1); i++) {
		if (xlo < int(res.size()) && res[xlo][ylo] == '#') {
			s += '(';
			xlo++;
		} else {
			s += ')';
			ylo++;
		}
		if (yhi < int(res[xhi].size()) && res[xhi][yhi] == '#') {
			s += '(';
			yhi++;
		} else {
			s += ')';
			xhi++;
		}
	}
	s.erase(s.begin());
	s.pop_back();
	return s;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int L, T; cin >> L >> T;

	while (T--) {
		auto o = read_obj(L);
		auto s = poly_to_bracket(std::get<poly_t>(o));
		cerr << s << '\n';
		auto b = bracket_to_poly(s);
		assert(o == obj_t(b));
	}

	return 0;
}
