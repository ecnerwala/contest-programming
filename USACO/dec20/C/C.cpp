#include <bits/stdc++.h>

template <int MOD_> struct modnum {
	static const int MOD = MOD_;
	static_assert(MOD > 0, "MOD must be positive");
private:
	int v;
public:
	modnum() : v(0) {}
	modnum(std::int64_t v_) : v(int(v_ % MOD)) {}

	explicit operator int() const { return v; }

	friend std::ostream& operator << (std::ostream& o, const modnum& m) {
		return o << int(m);
	}
	friend std::istream& operator >> (std::istream& i, modnum& m) {
		std::int64_t val;
		i >> val;
		m = modnum(val);
		return i;
	}

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum& operator ++ () {
		++v;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum operator ++ (int) {
		modnum r = *this;
		++*this;
		return r;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		--v;
		return *this;
	}
	modnum operator -- (int) {
		modnum r = *this;
		--*this;
		return r;
	}

	modnum neg() const {
		modnum r = *this;
		if (v == 0) r.v = 0;
		else r.v = MOD - v;
		return r;
	}

private:
	static int minv(int a, int m) {
		if (a <= 1) return a;
		return m - int(std::int64_t(minv(m % a, a)) * m / a);
	}
public:

	modnum inv() const {
		assert(v);
		modnum r;
		r.v = minv(v, MOD);
		return r;
	}

	modnum& operator += (const modnum& o) {
		v -= MOD - o.v;
		v = (v < 0 ? v + MOD : v);
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0 ? v + MOD : v);
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(std::int64_t(v) * std::int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return (*this) *= o.inv();
	}
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

struct trie_node {
	int sz = 0;
	int ch[2] = {0,0};
};

std::vector<trie_node> nodes(2);
const int ROOT = 1;
const int L = 30;

void add_node(int l, int r, int a = ROOT, int lo = 0, int hi = 1 << L) {
	assert(a != 0);
	assert(lo <= l && l < r && r <= hi);

	// we're assuming they're non-overlapping, so we don't have to check
	nodes[a].sz += r - l;

	if (l == lo && r == hi) {
		assert(nodes[a].sz == hi - lo);
		assert(nodes[a].ch[0] == 0 && nodes[a].ch[1] == 0);
		return;
	}

	int md = lo + ((hi - lo) >> 1);
	if (l < md) {
		if (nodes[a].ch[0] == 0) {
			nodes[a].ch[0] = int(nodes.size());
			nodes.push_back(trie_node());
		}
		add_node(l, std::min(r, md), nodes[a].ch[0], lo, md);
	}
	if (md < r) {
		if (nodes[a].ch[1] == 0) {
			nodes[a].ch[1] = int(nodes.size());
			nodes.push_back(trie_node());
		}
		add_node(std::max(l, md), r, nodes[a].ch[1], md, hi);
	}
}

int B, K;
using num = modnum<int(1e9)+7>;

num ans = 0;

num choose_3(int a) {
	return num(a) * num(a-1) * num(a-2) / num(6);
}

num choose_2(int a) {
	return num(a) * num(a-1) / num(2);
}

void solve_cross(int a, int oppo, int level, int num_oppo) {
	if (a == 0) return;
	assert(level <= B);
	if (a != -1 && nodes[a].sz == (1 << level)) a = -1;
	if (nodes[oppo].sz == (1 << level)) {
		num_oppo += K & ((1 << level) - 1);
		oppo = 0;
	}
	if (oppo == 0) {
		ans += choose_2(num_oppo) * num(a == -1 ? (1 << level) : nodes[a].sz);
		return;
	}
	assert(level >= 1);
	if (K & (1 << (level-1))) {
		for (int z = 0; z < 2; z++) {
			solve_cross(
				a == -1 ? -1 : nodes[a].ch[z],
				nodes[oppo].ch[!z],
				level-1,
				num_oppo + nodes[nodes[oppo].ch[z]].sz
			);
		}
	} else {
		for (int z = 0; z < 2; z++) {
			solve_cross(
				a == -1 ? -1 : nodes[a].ch[z],
				nodes[oppo].ch[z],
				level-1,
				num_oppo
			);
		}
	}
}

void solve(int a = ROOT, int level = L) {
	if (a == 0) return;
	assert(level > B);
	auto& n = nodes[a];
	if (n.sz == (1 << level)) {
		ans += choose_3(1 << B) * num(1 << (level - B));
		ans += choose_2(K - (1 << B)) * num(1 << level);
		return;
	}
	if (level == B+1) {
		ans += choose_3(nodes[n.ch[0]].sz);
		ans += choose_3(nodes[n.ch[1]].sz);
		solve_cross(n.ch[0], n.ch[1], B, 0);
		solve_cross(n.ch[1], n.ch[0], B, 0);
	} else {
		solve(n.ch[0], level-1);
		solve(n.ch[1], level-1);
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N_INTERVALS; cin >> N_INTERVALS >> K;
	K++; // make it exclusive

	nodes.reserve(2 + 2 * N_INTERVALS * L);

	B = 31 - __builtin_clz(K);
	assert(K & (1 << B));
	assert(0 <= B && B < L);

	for (int i = 0; i < N_INTERVALS; i++) {
		int l, r; cin >> l >> r; r++; // make it half-open
		add_node(l, r);
	}

	solve();
	cout << ans << '\n';

	return 0;
}
