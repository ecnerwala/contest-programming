#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::cerr;

// We'll represent catalan numbers by strings of ()

using poly_t = std::vector<std::string>;
using perm_t = std::vector<int>;
using tria_t = std::vector<std::array<int, 3>>;

using obj_t = std::variant<poly_t, perm_t, tria_t>;

void print_poly(std::ostream& os, const poly_t& v) {
	os << "poly" << '\n';
	os << int(v.size()) << ' ' << int(v[0].size()) << '\n';
	for (auto& r : v) os << r << '\n';
}
void print_perm(std::ostream& os, const perm_t& v) {
	os << "perm" << '\n';
	for (int i = 0; i < int(v.size()); i++) {
		os << v[i] << " \n"[i+1==int(v.size())];
	}
}
void print_tria(std::ostream& os, const tria_t& v) {
	os << "triang" << '\n';
	for (auto& r : v) os << r[0] << ' ' << r[1] << ' ' << r[2] << '\n';
}

void print_obj(std::ostream& os, const obj_t& v) {
	if (std::holds_alternative<poly_t>(v)) {
		print_poly(os, std::get<poly_t>(v));
	} else if (std::holds_alternative<perm_t>(v)) {
		print_perm(os, std::get<perm_t>(v));
	} else if (std::holds_alternative<tria_t>(v)) {
		print_tria(os, std::get<tria_t>(v));
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

std::string perm_to_bracket(perm_t res) {
	int L = int(res.size());
	std::string s;
	int nxt_val = 1;
	for (int i = 0; i < int(res.size()); i++) {
		if (res[i] < nxt_val) {
			s += ')';
		} else {
			while (res[i] > nxt_val) {
				s += '(';
				nxt_val++;
			}
			s += '(';
			s += ')';
			nxt_val++;
		}
	}
	assert(nxt_val == L+1);
	return s;
}

perm_t bracket_to_perm(std::string s) {
	int L = int(s.size()) / 2;
	std::vector<int> q; q.reserve(L);
	perm_t res; res.reserve(L);
	int nxt_val = 1;
	int idx = 0;
	for (char c : s) {
		if (c == '(') {
			q.push_back(nxt_val);
			nxt_val++;
		} else if (c == ')') {
			assert(idx < int(q.size()));
			if (q.back() == nxt_val - 1) {
				res.push_back(q.back());
				q.pop_back();
			} else {
				res.push_back(q[idx]);
				idx++;
			}
		} else assert(false);
	}
	return res;
}

std::string tria_to_bracket(tria_t res) {
	int L = int(res.size());
	// We have an L+2-gon, let's break it at 0..L+2
	// Then, for each triangle, we keep only its first edge
	std::vector<int> cnt_open(L+1), cnt_close(L+1);
	for (auto [a, b, c] : res) {
		cnt_open[a-1]++;
		cnt_close[b-1]++;
	}
	std::string s;
	for (int i = 0; i < L+1; i++) {
		for (int z = 0; z < cnt_close[i]; z++) s += ')';
		for (int z = 0; z < cnt_open[i]; z++) s += '(';
	}
	return s;
}

tria_t bracket_to_tria(std::string s) {
	int L = int(s.size()) / 2;
	tria_t res; res.reserve(L);
	tria_t stk; stk.reserve(L);
	int cur_vert = 1;
	for (char c : s) {
		if (c == '(') {
			stk.push_back({cur_vert, -1, -1});
		} else if (c == ')') {
			// we need to close some guys
			cur_vert++;
			while (true) {
				if (stk.back()[1] == -1) {
					stk.back()[1] = cur_vert;
					break;
				} else {
					stk.back()[2] = cur_vert;
					res.push_back(stk.back());
					stk.pop_back();
				}
			}
		} else assert(false);
	}
	cur_vert++;
	while (!stk.empty()) {
		stk.back()[2] = cur_vert;
		res.push_back(stk.back());
		stk.pop_back();
	}
	assert(int(res.size()) == L);
	std::sort(res.begin(), res.end());
	return res;
}

std::pair<std::string, int> obj_to_bracket(obj_t v) {
	if (std::holds_alternative<poly_t>(v)) {
		return {poly_to_bracket(std::get<poly_t>(v)), 0};
	} else if (std::holds_alternative<perm_t>(v)) {
		return {perm_to_bracket(std::get<perm_t>(v)), 1};
	} else if (std::holds_alternative<tria_t>(v)) {
		return {tria_to_bracket(std::get<tria_t>(v)), 2};
	} else assert(false);
}

obj_t bracket_to_obj(std::pair<std::string, int> a) {
	auto [s, t] = a;
	if (t == 0) {
		return obj_t(bracket_to_poly(s));
	} else if (t == 1) {
		return obj_t(bracket_to_perm(s));
	} else if (t == 2) {
		return obj_t(bracket_to_tria(s));
	} else assert(false);
}

std::vector<int64_t> catalan;
int64_t bracket_to_number(std::string s) {
	int L = int(s.size());
}
int64_t number_to_bracket(std::string s) {
}

std::pair<std::string, int> biject_brackets(std::pair<std::string, int> s) {
	a.second = (a.second + (parity(a.first) ? 1 : 2)) % 3;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int L, T; cin >> L >> T;
	catalan.reserve(35+1);
	catalan.push_back(1);
	for (int i = 1; i <= 35; i++) {
		for (int a = 0; a < i; a++) {
			catalan[i] += catalan[a] * catalan[i-a-1];
		}
	}

	while (T--) {
		auto o = read_obj(L);

		print_obj(cerr, o);
		auto v = obj_to_bracket(o);
		cerr << v.first << ' ' << v.second << '\n';
		auto b = bracket_to_obj(v);
		print_obj(cerr, b);
		assert(o == b);
		cerr << '\n';

	}

	return 0;
}
