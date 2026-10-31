#include <bits/stdc++.h>

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

using num = modnum<998244353>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int Q, K; cin >> Q >> K;
		vector<array<int64_t, 2>> queries(Q);
		for (auto& a : queries) { cin >> a[0] >> a[1]; a[1]++; }

		auto p2 = [](int a) -> int64_t {
			return int64_t(1) << a;
		};

		auto reverse_bits = [&](int64_t v) -> int64_t {
			int64_t o = 0;
			int64_t x = v;
			for (int i = 0; i < K; i++) {
				o <<= 1;
				o |= (x & 1);
				x >>= 1;
			}
			return o;
		};

		auto num_pairs = [&](int64_t a, int64_t b) -> num {
			if (a > b) swap(a, b);
			assert(a <= b);
			if (b == p2(K)) {
				return num(a) * num(p2((K+1)/2));
			}
			num ans = 0;
			// number of things < a and < b which xor to a palindrome
			int64_t c = a ^ b;
			int64_t p = a & b;
			int64_t cr = reverse_bits(c);
			if (cr == c) {
				ans += num(p);
			}
			for (int k = 0; k < K; k++) {
				// upper K-k bits are fixed
				num weight = num(p >> (k+1) << (k+1));
				if ((a >> k) & 1) weight += num(b - (b >> k << k));
				if ((b >> k) & 1) weight += num(a - (a >> k << k));
				if (2 * k <= K) {
					int64_t cur_c = c ^ p2(k);
					int64_t cur_cr = cr ^ p2(K-1-k);
					if ((cur_c & (p2(K-k)-p2(k))) == (cur_cr & (p2(K-k)-p2(k)))) {
						ans += weight;
					}
				} else {
					ans += weight * num(p2((2*k-K + 1) / 2));
				}
			}
			return ans;
		};

		num ans = 0;
		for (int i = 0; i < Q; i++) {
			for (int j = 0; j < i; j++) {
				num cur = 0;
				cur += num_pairs(queries[i][0], queries[j][0]);
				cur -= num_pairs(queries[i][1], queries[j][0]);
				cur -= num_pairs(queries[i][0], queries[j][1]);
				cur += num_pairs(queries[i][1], queries[j][1]);
				ans += cur;
			}
			num cur = 0;
			cur -= num_pairs(queries[i][0], queries[i][1]) * 2;
			cur += num_pairs(queries[i][0], queries[i][0]);
			cur += num_pairs(queries[i][1], queries[i][1]);
			cur -= num(queries[i][1] - queries[i][0]);

			ans += cur / 2;

			cout << ans << '\n';
		}
	}

	return 0;
}



// x = 1010101110????
// y = 1010??????????
// z = 1010101110110?
//
// x -> low_bits(x) ^ reverse(high_bits(x))
// Then, x^y is a palindrome iff folded(x) ^ folded(y) == 0

