#include<bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
	// coeff of a in x and y
	T vx = 1, vy = 0;
	while (x) {
		T k = y / x;
		y %= x;
		vy -= k * vx;
		std::swap(x, y);
		std::swap(vx, vy);
	}
	assert(y == 1);
	return vy < 0 ? m + vy : vy;
}

template <typename T> struct extended_gcd_result {
	T gcd;
	T coeff_a, coeff_b;
};
template <typename T> extended_gcd_result<T> extended_gcd(T a, T b) {
	T x = a, y = b;
	// coeff of a and b in x and y
	T ax = 1, ay = 0;
	T bx = 0, by = 1;
	while (x) {
		T k = y / x;
		y %= x;
		ay -= k * ax;
		by -= k * bx;
		std::swap(x, y);
		std::swap(ax, ay);
		std::swap(bx, by);
	}
	return {y, ay, by};
}

template <typename T> T mod_inv(T a, T m) {
	a %= m;
	a = a < 0 ? a + m : a;
	return mod_inv_in_range(a, m);
}

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	int v;

public:

	modnum() : v(0) {}
	modnum(int64_t v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { int64_t v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = mod_inv_in_range(v, MOD);
		return res;
	}
	friend modnum inv(const modnum& m) { return m.inv(); }
	modnum neg() const {
		modnum res;
		res.v = v ? MOD-v : 0;
		return res;
	}
	friend modnum neg(const modnum& m) { return m.neg(); }

	modnum operator- () const {
		return neg();
	}
	modnum operator+ () const {
		return modnum(*this);
	}

	modnum& operator ++ () {
		v ++;
		if (v == MOD) v = 0;
		return *this;
	}
	modnum& operator -- () {
		if (v == 0) v = MOD;
		v --;
		return *this;
	}
	modnum& operator += (const modnum& o) {
		v -= MOD-o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator -= (const modnum& o) {
		v -= o.v;
		v = (v < 0) ? v + MOD : v;
		return *this;
	}
	modnum& operator *= (const modnum& o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	modnum& operator /= (const modnum& o) {
		return *this *= o.inv();
	}

	friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
	friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
	friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
	friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
	friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
	friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

std::mt19937 mt(48);
struct treap_node {
	std::array<treap_node*, 2> c{nullptr, nullptr};
	std::mt19937::result_type pri = mt();
	int val;
	int cnt;
	std::pair<int, int> best;
	void update() {
		best = {cnt, val};
		if (c[0]) best = std::min(best, c[0]->best);
		if (c[1]) best = std::min(best, c[1]->best);
	}
};

std::tuple<treap_node*, treap_node*, treap_node*> split(treap_node* a, int v) {
	if (!a) return {nullptr, nullptr, nullptr};
	treap_node *l, *m, *r;
	if (a->val < v) {
		l = a;
		std::tie(l->c[1], m, r) = split(a->c[1], v);
	} else if (a->val > v) {
		r = a;
		std::tie(l, m, r->c[0]) = split(a->c[0], v);
	} else {
		m = a;
		l = std::exchange(a->c[0], nullptr);
		r = std::exchange(a->c[1], nullptr);
	}
	a->update();
	return {l, m, r};
}

treap_node* treap_union(treap_node* a, treap_node* b) {
	if (!a) return b;
	if (!b) return a;
	if (a->pri > b->pri) std::swap(a, b);
	auto [l, m, r] = split(b, a->val);
	if (m) {
		assert(m->val == a->val);
		a->cnt += m->cnt;
	}
	a->c[0] = treap_union(a->c[0], l);
	a->c[1] = treap_union(a->c[1], r);
	a->update();
	return a;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		std::vector<int> P(N);
		std::vector<std::pair<std::string, int>> S(N);
		for (int i = 0; i < N; i++) {
			cin >> P[i] >> S[i].first;
			if (i > 0) P[i]--;
			S[i].second = i;
		}
		std::sort(S.begin(), S.end());
		std::vector<int> V(N);
		int U = 0;
		for (int i = 0; i < N; i++) {
			if (i == 0 || S[i].first != S[i-1].first) U++;
			V[S[i].second] = U;
		}

		for (auto& p : P) cin >> p;

		std::vector<int> A(N);
		{
			std::vector<std::vector<int>> ch(N);
			for (int i = 1; i < N; i++) {
				cerr << i << ' ' << P[i] << '\n';
				ch[P[i]].push_back(i);
			}
			std::vector<int> cnt(U+1);
			std::set<std::pair<int, int>> best;
			[&](this auto&& self, int cur) -> void {
				A[cur] = best.empty() ? 0 : best.begin()->second;
				int v = V[cur];
				if (cnt[v]) best.erase({cnt[v], v});
				++cnt[v];
				if (cnt[v]) best.insert({cnt[v], v});
				for (int nxt : ch[cur]) {
					self(nxt);
				}
				if (cnt[v]) best.erase({cnt[v], v});
				--cnt[v];
				if (cnt[v]) best.insert({cnt[v], v});
			}(0);
		}

		std::vector<int> D(N);
		{
			std::vector<treap_node> nodes(N);
			std::vector<treap_node*> treaps(N, nullptr);
			for (int i = N-1; i >= 0; i--) {
				D[i] = treaps[i] ? treaps[i]->best.second : 0;
				nodes[i].val = V[i];
				nodes[i].update();
				treaps[i] = treap_union(treaps[i], &nodes[i]);
				int p = P[i];
				if (p >= 0) {
					treaps[p] = treap_union(treaps[p], treaps[i]);
				}
			}
		}

		modnum<998244353> hsh = 0;
		for (int i = 0; i < N; i++) {
			hsh *= U+1;
			hsh += A[i];
			hsh *= U+1;
			hsh += D[i];
		}

		cout << "Case #" << case_num << ": " << hsh << '\n';
	}

	return 0;
}
