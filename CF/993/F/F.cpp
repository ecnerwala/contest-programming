#include<bits/stdc++.h>
using namespace std;
typedef unsigned long long ull;

enum class OP {
	OR, AND, NOR, NAND
};

OP from_string(string s) {
	if (s == "or") {
		return OP::OR;
	} else if (s == "and") {
		return OP::AND;
	} else if (s == "nor") {
		return OP::NOR;
	} else if (s == "nand") {
		return OP::NAND;
	} else assert(false);
}

bool apply(OP o, bool a, bool b) {
	if (o == OP::OR) {
		return a || b;
	} else if (o == OP::AND) {
		return a && b;
	} else if (o == OP::NOR) {
		return !(a || b);
	} else if (o == OP::NAND) {
		return !(a && b);
	} else assert(false);
}

struct node {
	OP o;
	pair<int, int> i;
};

const int MAXN = 100;
const int MAXM = 100;
const int MAXK = 100;
int N, M, K;

node L1[MAXM];
node L2[MAXK];

pair<int, int> get_x(string s) {
	vector<int> res;
	for (size_t i = 0; i < s.size(); i++) {
		if (s[i] == 'x') res.push_back(int(i));
	}
	assert(res.size() == 2);
	return pair<int, int>(res[0], res[1]);
}

inline void push_inputs_L1(vector<int> &v, int a) {
	v.push_back(L1[a].i.first);
	v.push_back(L1[a].i.second);
}

inline void push_inputs_L2(vector<int> &v, int a) {
	push_inputs_L1(v, L2[a].i.first);
	push_inputs_L1(v, L2[a].i.second);
}

inline bool eval_L1(const unordered_map<int, bool> &m, int a) {
	return apply(L1[a].o, m.at(L1[a].i.first), m.at(L1[a].i.second));
}

inline bool eval_L2(const unordered_map<int, bool> &m, int a, bool l1_flip) {
	bool x = eval_L1(m, L2[a].i.first);
	bool y = eval_L1(m, L2[a].i.second);
	if (l1_flip) {
		x = !x, y = !y;
	}
	return apply(L2[a].o, x, y);
}

bool check_valid(int a, int b) {
	cerr << a << ' ' << b << '\n';
	vector<int> pts;
	push_inputs_L2(pts, a);
	push_inputs_L2(pts, b);
	sort(pts.begin(), pts.end());
	pts.resize(size_t(unique(pts.begin(), pts.end()) - pts.begin()));
	for (int i = 0; i < (1 << pts.size()); i++) {
		unordered_map<int, bool> m;
		for (int j = 0; j < int(pts.size()); j++) {
			m[pts[j]] = bool(i & (1 << j));
		}
		bool va = eval_L2(m, a, false);
		bool vb = eval_L2(m, b, true);
		if (va && !vb) {
			cerr << "invalid\n";
			return false;
		}
	}
	cerr << "valid\n";
	return true;
}

bool valid[MAXK][MAXK];
ull vmask[MAXK];

bool inverts(OP o) {
	return o == OP::NOR || o == OP::NAND;
}
int cnt_req[MAXN][2];
int tot_cnt = 0;
inline void add_L0(int a, int v, int inc) {
	tot_cnt -= (cnt_req[a][v] && cnt_req[a][!v]);
	cnt_req[a][v] += inc;
	tot_cnt += (cnt_req[a][v] && cnt_req[a][!v]);
}
inline void add_L1(int a, int v, int inc) {
	if (inverts(L1[a].o)) {
		v = !v;
	}
	add_L0(L1[a].i.first, v, inc);
	add_L0(L1[a].i.second, v, inc);
}
inline void add_L2(int a, int inc) {
	bool v = false;
	if (inverts(L2[a].o)) {
		v = !v;
	}

	add_L1(L2[a].i.first, v, inc);
	add_L1(L2[a].i.second, v, inc);
}

ull S(int a) {
	return (ull(1)) << a;
}

int res = -1;

void get_cliques(ull r, ull p, ull x) {
	if (p == 0 && x == 0) {
		cerr << r << '\n';
		cerr << tot_cnt << '\n';
		if (tot_cnt) {
			res = max(res, int(__builtin_popcountll(r)));
		}
		return;
	}
	int best = -1;
	int deg = -1;
	for (int u = 0; u < K; u++) {
		if (S(u) & (p | x)) {
			int d = __builtin_popcountll(p & vmask[u]);
			if (best == -1 || d > deg) {
				best = u;
				deg = d;
			}
		}
	}
	int u = best;
	for (int v = 0; v < K; v++) {
		if ((S(v) & p) && !(S(v) & vmask[u])) {
			add_L2(v, +1);
			get_cliques(r | S(v), p & vmask[v], x & vmask[v]);
			add_L2(v, -1);
			p -= S(v);
			x |= S(v);
		}
	}
}

int main() {
	cin >> N >> M >> K;

	for (int i = 0; i < M; i++) {
		string o, l; cin >> o >> l;
		L1[i].o = from_string(o);
		assert(int(l.size()) == N);
		L1[i].i = get_x(l);
	}

	for (int i = 0; i < K; i++) {
		string o, l; cin >> o >> l;
		L2[i].o = from_string(o);
		assert(int(l.size()) == M);
		L2[i].i = get_x(l);
	}

	for (int i = 0; i < K; i++) {
		for (int j = i; j < K; j++) {
			valid[i][j] = valid[j][i] = check_valid(i, j) && check_valid(j, i);
		}
	}
	ull p = 0;
	for (int i = 0; i < K; i++) {
		if (!valid[i][i]) {
			for (int j = 0; j < K; j++) {
				valid[i][j] = valid[j][i] = false;
			}
		} else {
			p |= S(i);
		}
	}

	for (int i = 0; i < K; i++) {
		for (int j = 0; j < K; j++) {
			if (j == i) continue;
			if (valid[i][j]) {
				vmask[i] |= S(j);
			}
		}
	}

	get_cliques(0, p, 0);
	cout << res << '\n';

	return 0;
}
