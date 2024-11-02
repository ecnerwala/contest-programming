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

using num = modnum<998244353>;

std::mt19937 mt(48);

struct treap_node {
	std::array<treap_node*, 2> c{nullptr, nullptr};
	std::mt19937::result_type pri = mt();
	int64_t val;
	int64_t sum = 0;
	int sz = 0;
	num sum_incr = 0;
	void update() {
		sz = 1;
		sum = val;
		sum_incr = val;
		if (c[0]) {
			sum_incr += num(c[0]->sz) * num(sum);
			sum_incr += c[0]->sum_incr;
			sum += c[0]->sum;
			sz += c[0]->sz;
		}
		if (c[1]) {
			sum_incr += num(sz) * num(c[1]->sum);
			sum_incr += c[1]->sum_incr;
			sum += c[1]->sum;
			sz += c[1]->sz;
		}
	}
};
int get_sz(treap_node* t) { return t ? t->sz : 0; }
int64_t get_sum(treap_node* t) { return t ? t->sum : 0; }

[[nodiscard]] std::pair<treap_node*, treap_node*> split_value(treap_node* a, int64_t v) {
	if (!a) return {nullptr, nullptr};
	treap_node *l, *r;
	if (a->val > v) {
		l = a;
		std::tie(l->c[1], r) = split_value(a->c[1], v);
	} else {
		r = a;
		std::tie(l, r->c[0]) = split_value(a->c[0], v);
	}
	a->update();
	return {l, r};
}
[[nodiscard]] treap_node* insert(treap_node* t, treap_node* n) {
	assert(n);
	if (!t || n->pri < t->pri) {
		std::tie(n->c[0], n->c[1]) = split_value(t, n->val);
		n->update();
		return n;
	} else {
		if (n->val > t->val) {
			t->c[0] = insert(t->c[0], n);
		} else {
			t->c[1] = insert(t->c[1], n);
		}
		t->update();
		return t;
	}
}

[[nodiscard]] treap_node* pop_back(treap_node* a) {
	assert(a);
	if (a->c[1]) {
		a->c[1] = pop_back(a->c[1]);
		a->update();
		return a;
	} else {
		treap_node* r = std::exchange(a->c[0], nullptr);
		a->update();
		return r;
	}
}

[[nodiscard]] treap_node* pop_front(treap_node* a) {
	assert(a);
	if (a->c[0]) {
		a->c[0] = pop_back(a->c[0]);
		a->update();
		return a;
	} else {
		treap_node* r = std::exchange(a->c[1], nullptr);
		a->update();
		return r;
	}
}

[[nodiscard]] treap_node* merge(treap_node *l, treap_node *r) {
	if (!r) return l;
	if (!l) return r;
	treap_node* a;
	if (l->pri < r->pri) {
		a = l;
		a->c[1] = merge(l->c[1], r);
	} else {
		a = r;
		a->c[0] = merge(l, r->c[0]);
	}
	a->update();
	return a;
}

[[nodiscard]] std::pair<treap_node*, treap_node*> split_sz(treap_node* a, int64_t cnt) {
	assert(0 <= cnt && cnt <= get_sz(a));
	if (!a) return {nullptr, nullptr};
	treap_node *l, *r;
	assert(get_sz(a) == 1 + get_sz(a->c[0]) + get_sz(a->c[1]));
	if (cnt > get_sz(a->c[0])) {
		l = a;
		std::tie(l->c[1], r) = split_sz(a->c[1], cnt - 1 - get_sz(a->c[0]));
	} else {
		r = a;
		std::tie(l, r->c[0]) = split_sz(a->c[0], cnt);
	}
	a->update();
	return {l, r};
}

void adjust_first(treap_node* t, int64_t delta) {
	if (!t) {
		return;
	}
	assert(t);
	if (t->c[0]) adjust_first(t->c[0], delta);
	else t->val += delta;
	t->update();
}

void flatten(treap_node* a, std::vector<int64_t>& b) {
	if (!a) return;
	flatten(a->c[0], b);
	b.push_back(b.back() + a->val);
	flatten(a->c[1], b);
}

void unflatten(treap_node* a, std::vector<int64_t>& b, int& idx) {
	if (!a) return;
	flatten(a->c[0], b);
	a->val = b[idx+1] - b[idx];
	idx++;
	flatten(a->c[1], b);
	a->update();
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M; cin >> N >> M;
		cerr << "Case #" << case_num << " " << "N = " << N << " M = " << M << '\n';
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<std::vector<int>> adj(N);
		for (int e = 0; e < N-1; e++) {
			int u, v; cin >> u >> v; u--, v--;
			adj[u].push_back(v);
			adj[v].push_back(u);
		}
		std::vector<treap_node> nodes(N);
		num ans = 0;
		std::vector<int64_t> buf_cur; buf_cur.reserve(N+1);
		std::vector<int64_t> buf_nxt; buf_nxt.reserve(N+1);
		[&](this auto&& self, int cur, int prv) -> treap_node* {
			bool has_ch = false;
			treap_node* t_cur = nullptr;
			for (int nxt : adj[cur]) {
				if (nxt == prv) continue;
				auto t_nxt = self(nxt, cur);
				if (!has_ch) {
					has_ch = true;
					t_cur = t_nxt;
					continue;
				}

				if (get_sz(t_nxt) > get_sz(t_cur)) {
					std::swap(t_cur, t_nxt);
				}
				while (get_sz(t_cur) > get_sz(t_nxt) && get_sum(t_nxt) <= get_sum(t_cur)) {
					t_cur = pop_back(t_cur);
				}
				if (get_sz(t_cur) > get_sz(t_nxt)) {
					assert(get_sum(t_nxt) > get_sum(t_cur));
				}
				assert(get_sz(t_cur) >= get_sz(t_nxt));
				// now, replace the prefix of t_cur with the min of t_cur and t_nxt
				auto [t_cur_pref, t_cur_suff] = split_sz(t_cur, get_sz(t_nxt));
				adjust_first(t_cur_suff, +get_sum(t_cur_pref));

				buf_cur.assign(1, int64_t(0));
				flatten(t_cur_pref, buf_cur);
				buf_nxt.assign(1, int64_t(0));
				flatten(t_nxt, buf_nxt);
				assert(int(buf_nxt.size()) == int(buf_cur.size()));
				for (int i = 0; i < int(buf_nxt.size()); i++) {
					buf_cur[i] = std::min(buf_cur[i], buf_nxt[i]);
				}
				{
					int idx = 0;
					unflatten(t_cur_pref, buf_cur, idx);
					assert(idx + 1 == int(buf_cur.size()));
				}

				adjust_first(t_cur_suff, -get_sum(t_cur_pref));
				t_cur = merge(t_cur_pref, t_cur_suff);
				cerr << "merge " << cur << ' ' << nxt << '\n';
				{
					buf_cur.assign(1, int64_t(0));
					flatten(t_cur, buf_cur);
					for (auto v : buf_cur) { cerr << v << ' '; } cerr << '\n';
				}
			}
			nodes[cur].val = A[cur];
			t_cur = insert(t_cur, &nodes[cur]);
			assert(t_cur);
			while (t_cur->sz > M) {
				t_cur = pop_back(t_cur);
			}
			assert(t_cur);
			ans += num(t_cur->sum) * num(M) - t_cur->sum_incr;
			cerr << "recurse " << cur << '\n';
			buf_cur.assign(1, int64_t(0));
			flatten(t_cur, buf_cur);
			for (auto v : buf_cur) { cerr << v << ' '; } cerr << '\n';
			return t_cur;
		}(0, -1);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
