#include <bits/stdc++.h>

template <typename T, class Compare = std::less<T>> class RangeMinQuery : private Compare {
	static const int BUCKET_SIZE = 32;
	static const int BUCKET_SIZE_LOG = 5;
	static_assert(BUCKET_SIZE == (1 << BUCKET_SIZE_LOG), "BUCKET_SIZE should be a power of 2");
	static const int CACHE_LINE_ALIGNMENT = 64;
	int n = 0;
	std::vector<T> data;
	std::vector<T> pref_data;
	std::vector<T> suff_data;
	std::vector<T> sparse_table;
	std::vector<uint32_t> range_mask;

private:
	int num_buckets() const {
		return n >> BUCKET_SIZE_LOG;
	}
	int num_levels() const {
		return num_buckets() ? 32 - __builtin_clz(num_buckets()) : 0;
	}
	int sparse_table_size() const {
		return num_buckets() * num_levels();
	}
private:
	const T& min(const T& a, const T& b) const {
		return Compare::operator()(a, b) ? a : b;
	}
	void setmin(T& a, const T& b) const {
		if (Compare::operator()(b, a)) a = b;
	}

	template <typename Vec> static int get_size(const Vec& v) { using std::size; return int(size(v)); }

public:
	RangeMinQuery() {}
	template <typename Vec> explicit RangeMinQuery(const Vec& data_, const Compare& comp_ = Compare())
		: Compare(comp_)
		, n(get_size(data_))
		, data(n)
		, pref_data(n)
		, suff_data(n)
		, sparse_table(sparse_table_size())
		, range_mask(n)
	{
		for (int i = 0; i < n; i++) data[i] = data_[i];
		for (int i = 0; i < n; i++) {
			if (i & (BUCKET_SIZE-1)) {
				uint32_t m = range_mask[i-1];
				while (m && !Compare::operator()(data[(i | (BUCKET_SIZE-1)) - __builtin_clz(m)], data[i])) {
					m -= uint32_t(1) << (BUCKET_SIZE - 1 - __builtin_clz(m));
				}
				m |= uint32_t(1) << (i & (BUCKET_SIZE - 1));
				range_mask[i] = m;
			} else {
				range_mask[i] = 1;
			}
		}
		for (int i = 0; i < n; i++) {
			pref_data[i] = data[i];
			if (i & (BUCKET_SIZE-1)) {
				setmin(pref_data[i], pref_data[i-1]);
			}
		}
		for (int i = n-1; i >= 0; i--) {
			suff_data[i] = data[i];
			if (i+1 < n && ((i+1) & (BUCKET_SIZE-1))) {
				setmin(suff_data[i], suff_data[i+1]);
			}
		}
		for (int i = 0; i < num_buckets(); i++) {
			sparse_table[i] = data[i * BUCKET_SIZE];
			for (int v = 1; v < BUCKET_SIZE; v++) {
				setmin(sparse_table[i], data[i * BUCKET_SIZE + v]);
			}
		}
		for (int l = 0; l+1 < num_levels(); l++) {
			for (int i = 0; i + (1 << (l+1)) <= num_buckets(); i++) {
				sparse_table[(l+1) * num_buckets() + i] = min(sparse_table[l * num_buckets() + i], sparse_table[l * num_buckets() + i + (1 << l)]);
			}
		}
	}

	T query(int l, int r) const {
		assert(l <= r);
		int bucket_l = (l >> BUCKET_SIZE_LOG);
		int bucket_r = (r >> BUCKET_SIZE_LOG);
		if (bucket_l == bucket_r) {
			uint32_t msk = range_mask[r] & ~((uint32_t(1) << (l & (BUCKET_SIZE-1))) - 1);
			int ind = (l & ~(BUCKET_SIZE-1)) + __builtin_ctz(msk);
			return data[ind];
		} else {
			T ans = min(suff_data[l], pref_data[r]);
			bucket_l++;
			if (bucket_l < bucket_r) {
				int level = (32 - __builtin_clz(bucket_r - bucket_l)) - 1;
				setmin(ans, sparse_table[level * num_buckets() + bucket_l]);
				setmin(ans, sparse_table[level * num_buckets() + bucket_r - (1 << level)]);
			}
			return ans;
		}
	}
};

template <typename T> using RangeMaxQuery = RangeMinQuery<T, std::greater<T>>;

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using num = modnum<998244353>;

struct split_ways_t {
	num tot;
	num lazy;
	// We store pairs of {pt, query}: ways_after, starting with {-1, -1}: ...
	std::map<std::pair<int, int>, num> ways;

	void set_single() {
		lazy = 1;
		tot = 1;
		ways = {{{-1, -1}, num(1)}};
	}

	void insert_last(std::pair<int, int> p) {
		auto [it, did_insert] = ways.insert({p, num(0)});
		assert(did_insert);
	}
	void insert_first(std::pair<int, int> p) {
		auto [it, did_insert] = ways.insert({p, num(0)});
		assert(it != ways.begin());
		std::swap(it->second, std::prev(it)->second);
	}

	void remove(std::pair<int, int> p) {
		auto it = ways.find(p);
		assert(it != ways.end());
		assert(it != ways.begin());
		std::prev(it)->second += it->second;
		ways.erase(it);
	}
};

struct treap_node;

using ptr_t = std::variant<num, treap_node*>;

std::mt19937 mt;

struct treap_key_t {
	int loc;
	bool dir;
	int q;
	friend std::strong_ordering operator <=> (treap_key_t a, treap_key_t b) = default;
	friend bool operator == (treap_key_t a, treap_key_t b) = default;
};

struct treap_node {
	std::mt19937::result_type pri = mt();
	treap_key_t key;
	std::array<ptr_t, 2> ch = {num(0), num(0)};
	num lazy = 1;
	num tot = 0;
};

num get_tot(ptr_t p) {
	if (p.index() == 0) {
		return std::get<num>(p);
	} else {
		return std::get<treap_node*>(p)->tot;
	}
}
ptr_t apply_lazy(ptr_t p, num v) {
	if (p.index() == 0) {
		return std::get<num>(p) * v;
	} else {
		auto& node = std::get<treap_node*>(p);
		node->tot *= v;
		node->lazy *= v;
		return p;
	}
}
void update(treap_node* t) {
	t->tot = get_tot(t->ch[0]) + get_tot(t->ch[1]);
}
void push_down(treap_node* t) {
	if (t->lazy != num(1)) {
		t->ch[0] = apply_lazy(t->ch[0], t->lazy);
		t->ch[1] = apply_lazy(t->ch[1], t->lazy);
		t->lazy = 1;
	}
}
ptr_t concat(ptr_t a, ptr_t b, num mid_val) {
	if (a.index() == 0 && b.index() == 0) {
		return num(std::get<num>(a) + std::get<num>(b) + mid_val);
	}
	if (a.index() == 1 && (b.index() == 0 || std::get<treap_node*>(a)->pri < std::get<treap_node*>(b)->pri)) {
		treap_node *r = std::get<treap_node*>(a);
		push_down(r);
		r->ch[1] = concat(r->ch[1], b, mid_val);
		update(r);
		return r;
	} else {
		treap_node *r = std::get<treap_node*>(b);
		push_down(r);
		r->ch[0] = concat(a, r->ch[0], mid_val);
		update(r);
		return r;
	}
}
std::tuple<ptr_t, treap_node*, ptr_t> split_at(ptr_t p, treap_key_t key) {
	assert(p.index() == 1);
	treap_node* t = std::get<treap_node*>(p);
	push_down(t);
	if (key == t->key) {
		ptr_t a = t->ch[0], c = t->ch[1];
		t->ch[0] = t->ch[1] = num(0);
		update(t);
		return {a, t, c};
	} else if (key < t->key) {
		ptr_t a; treap_node* b;
		std::tie(a, b, t->ch[0]) = split_at(t->ch[0], key);
		update(t);
		return {a, b, t};
	} else if (key > t->key) {
		treap_node* b; ptr_t c;
		std::tie(t->ch[1], b, c) = split_at(t->ch[1], key);
		update(t);
		return {t, b, c};
	} else assert(false);
}

template <typename T>
void get_suffix(std::vector<num>& v, ptr_t p, T&& cond) {
	if (p.index() == 0) {
		v.push_back(std::get<num>(p));
	} else {
		treap_node* t = std::get<treap_node*>(p);
		push_down(t);
		get_suffix(v, t->ch[1], cond);
		if (!cond(std::get<treap_node*>(p)->key)) {
			return;
		}
		get_suffix(v, t->ch[0], cond);
	}
}

template <typename T>
void get_prefix(std::vector<num>& v, ptr_t p, T&& cond) {
	if (p.index() == 0) {
		v.push_back(std::get<num>(p));
	} else {
		treap_node* t = std::get<treap_node*>(p);
		push_down(t);
		get_prefix(v, t->ch[0], cond);
		if (!cond(std::get<treap_node*>(p)->key)) {
			return;
		}
		get_prefix(v, t->ch[1], cond);
	}
}

void dump_rec(ptr_t p) {
	if (p.index() == 0) {
		std::cerr << std::get<num>(p) << ' ';
	} else {
		auto t = std::get<treap_node*>(p);
		push_down(t);
		dump_rec(std::get<treap_node*>(p)->ch[0]);
		std::cerr << t->key.loc << ',' << t->key.dir << ',' << t->key.q << ' ';
		dump_rec(std::get<treap_node*>(p)->ch[1]);
	}
}
void dump(ptr_t p) {
	dump_rec(p);
	std::cerr << '\n';
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, Q; cin >> N >> Q;
	std::vector<int> X(N-1);
	for (auto& x : X) cin >> x;
	std::vector<std::array<int, 2>> queries(Q);
	for (auto& [l, r] : queries) cin >> l >> r;

	const int V = 2 * (N+2) - 1;
	std::vector<int> split_node(N+1, -1);
	std::vector<std::array<int, 2>> vert_bounds(V);
	std::vector<int> vert_split(V);
	std::vector<std::array<int, 2>> ch(V);
	std::vector<int> path_vert; path_vert.reserve(V-1);
	{
		int idx = 0;
		auto Xit = X.begin();
		std::y_combinator([&](auto self, int l, int r) -> void {
			int cur = idx++;
			vert_bounds[cur] = {l, r};
			if (r - l == 1) {
				// we're just a leaf
				ch[cur] = {-1, -1};
				vert_split[cur] = -1;
			} else {
				int m;
				if (l == -1 && r == N+1) {
					m = N;
				} else if (l == -1 && r == N) {
					m = 0;
				} else {
					m = *Xit;
					++Xit;
				}
				vert_split[cur] = m;
				split_node[m] = cur;
				path_vert.push_back(cur);
				ch[cur][0] = idx; self(l, m);
				path_vert.push_back(cur);
				ch[cur][1] = idx; self(m, r);
			}
		})(-1, N+1);
		assert(idx == V);
		for (int i = 0; i <= N; i++) assert(split_node[i] != -1);
		assert(int(path_vert.size()) == V-1);
	}

	RangeMinQuery<int> rmq(path_vert);

	std::vector<std::vector<int>> lqueries(V);
	std::vector<std::vector<int>> rqueries(V);
	std::vector<int> query_rmq(Q);
	std::vector<std::vector<int>> rmq_queries(V);
	for (int q = 0; q < Q; q++) {
		auto [l, r] = queries[q];
		int lv = split_node[l];
		int rv = split_node[r];
		assert(lv != rv);
		lqueries[lv].push_back(q);
		rqueries[rv].push_back(q);
		query_rmq[q] = rmq.query(std::min(lv, rv), std::max(lv, rv) - 1);
		rmq_queries[query_rmq[q]].push_back(q);
	}

	struct dp_t {
		num connected_ways = 0;
		num need_ways = 0;
		ptr_t split_ways;
	};

	auto root_res = std::y_combinator([&](auto self, int cur) -> dp_t {
		auto [l, r] = vert_bounds[cur];
		dp_t res;
		if (r - l == 1) {
			// Leaf, trivial case
			res.split_ways = num(1);
			cerr << cur << ' ' << l << '-' << r << '\n';
		} else {
			auto l_val = self(ch[cur][0]);
			auto r_val = self(ch[cur][1]);
			int m = vert_split[cur];

			cerr << '\n';

			cerr << "Node " << cur << ' ' << l << '-' << m << '-' << r << '\n';
			cerr << "l_val.split_ways" << '\n';
			dump(l_val.split_ways);
			cerr << "r_val.split_ways" << '\n';
			dump(r_val.split_ways);
			std::sort(lqueries[cur].begin(), lqueries[cur].end());
			for (int q : lqueries[cur]) {
				treap_node* t = new treap_node;
				t->key = {m, 0, q};
				l_val.split_ways = concat(l_val.split_ways, t, num(0));
			}
			std::sort(rqueries[cur].begin(), rqueries[cur].end(), std::greater<>());
			for (int q : rqueries[cur]) {
				treap_node* t = new treap_node;
				t->key = {m, 1, q};
				r_val.split_ways = concat(t, r_val.split_ways, num(0));
			}

			// Easy cases
			res.connected_ways = l_val.connected_ways * r_val.connected_ways;
			res.need_ways = l_val.need_ways * r_val.connected_ways + l_val.connected_ways * r_val.need_ways;

			std::vector<treap_key_t> l_pts; l_pts.reserve(rmq_queries[cur].size());
			std::vector<treap_key_t> r_pts; r_pts.reserve(rmq_queries[cur].size());
			for (auto q : rmq_queries[cur]) {
				auto [ql, qr] = queries[q];
				l_pts.push_back({ql, 0, q});
				r_pts.push_back({qr, 1, q});
			}
			std::sort(l_pts.begin(), l_pts.end(), std::greater<>());
			std::sort(r_pts.begin(), r_pts.end());

			std::vector<num> l_split_ways; l_split_ways.reserve(rmq_queries[cur].size() + 1);
			get_suffix(l_split_ways, l_val.split_ways, [&](treap_key_t p) -> bool {
				return query_rmq[p.q] == cur;
			});
			std::vector<num> r_split_ways; r_split_ways.reserve(rmq_queries[cur].size() + 1);
			get_prefix(r_split_ways, r_val.split_ways, [&](treap_key_t p) -> bool {
				return query_rmq[p.q] == cur;
			});

			// If they're both split, then we need to include most middle things
			num mid_split_ways = 0;
			num mid_need_ways = 0;
			{
				int l_idx = m;
				int r_idx = m;
				int l_need = m;
				int r_need = m;
				for (int z = 0; z < int(l_split_ways.size()) && z < int(r_split_ways.size()); z++) {
					if (z) {
						{
							int lq = l_pts[z-1].q;
							assert(query_rmq[lq] == cur);
							l_idx = queries[lq][0];
							r_need = std::max(r_need, queries[lq][1]);
						}
						{
							int rq = r_pts[z-1].q;
							assert(query_rmq[rq] == cur);
							r_idx = queries[rq][1];
							l_need = std::min(l_need, queries[rq][0]);
						}
					}
					if (l_split_ways[z] == 0 || r_split_ways[z] == 0) continue;
					if (l_need < l_idx || r_need > r_idx) continue;
					num cur_ways = l_split_ways[z] * r_split_ways[z];
					if (z == int(rmq_queries[cur].size())) {
						mid_split_ways += cur_ways;
					} else {
						mid_need_ways += cur_ways;
					}
				}
			}

			res.need_ways += mid_need_ways;

			// Now we handle split / connected and connected / split
			l_val.split_ways = apply_lazy(l_val.split_ways, r_val.connected_ways);
			r_val.split_ways = apply_lazy(r_val.split_ways, l_val.connected_ways);

			// Everything that breaks at least one query becomes a need
			if (!rmq_queries[cur].empty()) {
				{
					auto l_pt = l_pts.back();
					// TODO: could do without splitting, but whatever
					auto [lo, md, hi] = split_at(l_val.split_ways, l_pt);
					res.need_ways += get_tot(hi);
					apply_lazy(hi, num(0));
					l_val.split_ways = concat(concat(lo, md, num(0)), hi, num(0));
				}
				{
					auto r_pt = r_pts.back();
					auto [lo, md, hi] = split_at(r_val.split_ways, r_pt);
					res.need_ways += get_tot(lo);
					apply_lazy(lo, num(0));
					r_val.split_ways = concat(concat(lo, md, num(0)), hi, num(0));
				}
			}

			// Now, remove things
			for (auto l_pt : l_pts) {
				auto [lo, md, hi] = split_at(l_val.split_ways, l_pt);
				l_val.split_ways = concat(lo, hi, num(0));
				delete md;
			}
			for (auto r_pt : r_pts) {
				auto [lo, md, hi] = split_at(r_val.split_ways, r_pt);
				r_val.split_ways = concat(lo, hi, num(0));
				delete md;
			}
			res.split_ways = concat(l_val.split_ways, r_val.split_ways, mid_split_ways);
			cerr << "split ways" << '\n';
			dump(res.split_ways);

		}
		if (0 <= l && r <= N) {
			// double the ways
			res.connected_ways += res.connected_ways + res.need_ways + get_tot(res.split_ways);
		}
		return res;
	})(0);

	cout << get_tot(root_res.split_ways) << '\n';

	return 0;
}

//                             -1-6
//            -1-5                               5-6
// -1-0                   0-5
//                0-2               2-5
//            0-1    1-2        2-4     4-5
//                           2-3   3-4
