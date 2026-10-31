#include<bits/stdc++.h>

/** Binary-indexed tree
 *
 *  A binary indexed tree with N nodes of type T provides the
 *  following two functions for 0 <= i <= N:
 *
 *      prefix(int i) -> prefix_iterator<T>
 *      suffix(int i) -> suffix_iterator<T>
 *
 *  such that size(suffix(i) intersect prefix(j)) = (1 if i < j else 0).
 *  Furthermore, the resulting lists always have size at most log_2(N).
 *
 *  This can be used to implement either point-update/(prefix|suffix)-query or
 *  (prefix|suffix)-update/point-query over a virtual array of size N of a
 *  commutative monoid. This can be generalized to implement
 *  point-update/range-query or range-update/point-query over a virtual array
 *  of size N of a commutative group.
 *
 *  With 0-indexed data, prefixes are more natural:
 *   * For range update/query, use for_prefix for the ranges and for_suffix for the points.
 *   * For prefix update/query, no change.
 *   * For suffix update/query, use for_prefix(point + 1); 1-index the data.
 */
template <typename T> class binary_indexed_tree {
private:
	std::vector<T> dat;
public:
	binary_indexed_tree() {}
	explicit binary_indexed_tree(size_t N) : dat(N) {}
	binary_indexed_tree(size_t N, const T& t) : dat(N, t) {}

	size_t size() const { return dat.size(); }
	const std::vector<T>& data() const { return dat; }
	std::vector<T>& data() { return dat; }

	class const_suffix_range;
	class const_suffix_iterator {
	private:
		const T* dat;
		int a;
		const_suffix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree::const_suffix_range;
	public:
		friend bool operator != (const const_suffix_iterator& i, const const_suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		const_suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		const T& operator * () const {
			return dat[a];
		}
	};
	class const_suffix_range {
	private:
		const T* dat;
		int sz;
		int a;
		const_suffix_range(const T* dat_, int sz_, int a_) : dat(dat_), sz(sz_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		const_suffix_iterator begin() const {
			return const_suffix_iterator{dat, a};
		}
		const_suffix_iterator end() const {
			return const_suffix_iterator{nullptr, sz};
		}
	};
	const_suffix_range suffix(int a) const {
		assert(0 <= a && a <= int(dat.size()));
		return const_suffix_range{dat.data(), int(dat.size()), a};
	}

	class suffix_range;
	class suffix_iterator {
	private:
		T* dat;
		int a;
		suffix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree::suffix_range;
	public:
		friend bool operator != (const suffix_iterator& i, const suffix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a < j.a;
		}
		suffix_iterator& operator ++ () {
			a |= a+1;
			return *this;
		}
		T& operator * () const {
			return dat[a];
		}
	};
	class suffix_range {
	private:
		T* dat;
		int sz;
		int a;
		suffix_range(T* dat_, int sz_, int a_) : dat(dat_), sz(sz_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		suffix_iterator begin() const {
			return suffix_iterator{dat, a};
		}
		suffix_iterator end() const {
			return suffix_iterator{nullptr, sz};
		}
	};
	suffix_range suffix(int a) {
		assert(0 <= a && a <= int(dat.size()));
		return suffix_range{dat.data(), int(dat.size()), a};
	}

	class const_prefix_range;
	class const_prefix_iterator {
	private:
		const T* dat;
		int a;
		const_prefix_iterator(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree::const_prefix_range;
	public:
		friend bool operator != (const const_prefix_iterator& i, const const_prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		const_prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		const T& operator * () const {
			return dat[a-1];
		}
	};
	class const_prefix_range {
	private:
		const T* dat;
		int a;
		const_prefix_range(const T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		const_prefix_iterator begin() const {
			return const_prefix_iterator{dat, a};
		}
		const_prefix_iterator end() const {
			return const_prefix_iterator{nullptr, 0};
		}
	};
	const_prefix_range prefix(int a) const {
		return const_prefix_range{dat.data(), a};
	}

	class prefix_range;
	class prefix_iterator {
	private:
		T* dat;
		int a;
		prefix_iterator(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree::prefix_range;
	public:
		friend bool operator != (const prefix_iterator& i, const prefix_iterator& j) {
			assert(j.dat == nullptr);
			return i.a > 0;
		}
		prefix_iterator& operator ++ () {
			a &= a-1;
			return *this;
		}
		T& operator * () const {
			return dat[a-1];
		}
	};

	class prefix_range {
	private:
		T* dat;
		int a;
		prefix_range(T* dat_, int a_) : dat(dat_), a(a_) {}
		friend class binary_indexed_tree;
	public:
		prefix_iterator begin() const {
			return prefix_iterator{dat, a};
		}
		prefix_iterator end() const {
			return prefix_iterator{nullptr, 0};
		}
	};

	prefix_range prefix(int a) {
		return prefix_range{dat.data(), a};
	}
};

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	static_assert(MOD_ > 0, "MOD must be positive");

private:
	using ll = long long;

	int v;

	static int minv(int a, int m) {
		a %= m;
		assert(a);
		return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
	}

public:

	modnum() : v(0) {}
	modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
	explicit operator int() const { return v; }
	friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
	friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }

	friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
	friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }

	modnum inv() const {
		modnum res;
		res.v = minv(v, MOD);
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
		v = int(ll(v) * ll(o.v) % MOD);
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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> A(N); for (auto& a : A) cin >> a;

	const int V = int(2e5);
	assert(*max_element(A.begin(), A.end()) <= V);

	using num = modnum<int(1e9)+7>;

	vector<int> primes; primes.reserve(V);
	vector<num> pinv(V+1);
	vector<int> pfac(V+1);
	for (int i = 2; i <= V; i++) {
		if (!pfac[i]) { pfac[i] = i; primes.push_back(i); pinv[i] = inv(num(i)); }
		for (int p : primes) {
			if (p > V/i) break;
			pfac[p*i] = p;
			if (p == pfac[i]) break;
		}
	}

	binary_indexed_tree<vector<pair<int, num>>> bit(N);

	vector<int> last_seen(V+1, -1);

	vector<num> pref(N+1);
	pref[0] = 1;
	for (int i = 0; i < N; i++) pref[i+1] = pref[i] * num(A[i]);
	vector<num> inv_pref(N+1);
	inv_pref[N] = inv(pref[N]);
	for (int i = N-1; i >= 0; i--) inv_pref[i] = inv_pref[i+1] * num(A[i]);

	// for prime p, if it divides A[k_1], A[k_2], ...
	// ans[l][r] *= p if l <= k_something < r
	// equivalently, for each i:
	//   ans[k_{i-1}..k_i][k_i..N] *= p
	vector<pair<int, num>> buf; buf.reserve(30);
	for (int i = 0; i < N; i++) {
		int a = A[i];
		buf.clear();
		while (a > 1) {
			int p = pfac[a];
			int e = 0, q = 1;

			num invP = pinv[p];

			num curInv = 1;
			while (a % p == 0) {
				a /= p;
				e++, q *= p;

				curInv *= invP;

				if (a % p == 0 && last_seen[q*p] == last_seen[q]) {
					// don't insert it yet, just wait until the next loop
				} else {
					buf.push_back({last_seen[q], curInv});
					curInv = 1;
				}
				last_seen[q] = i;
			}
		}
		for (auto& v : bit.suffix(i)) {
			v.insert(v.end(), buf.begin(), buf.end());
		};
	}
	auto cmp = [](const pair<int, num>& a, const pair<int, num>& b) { return a.first < b.first; };

	for (auto& v : bit.data()) {
		sort(v.begin(), v.end(), cmp);
		for (int i = int(v.size())-1; i > 0; i--) {
			v[i-1].second *= v[i].second;
		}
	}

	auto query = [&](int l, int r) -> num {
		assert(0 <= l && l < r && r <= N);

		num ans = pref[r] * inv_pref[l];
		for (const auto& v : as_const(bit).prefix(r)) {
			auto it = lower_bound(v.begin(), v.end(), pair<int, num>{l, num(0)}, cmp);
			if (it != v.end()) ans *= it->second;
		};
		return ans;
	};

	int Q; cin >> Q;
	int last = 0;
	while (Q--) {
		int x, y; cin >> x >> y;
		int l = (last + x) % N + 1;
		int r = (last + y) % N + 1;
		if (l > r) swap(l, r);
		// half-open interval
		l--;
		num ans = query(l, r);
		cout << (last = int(ans)) << '\n';
	}

	return 0;
}
