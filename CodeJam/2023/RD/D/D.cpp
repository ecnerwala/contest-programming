#include <bits/stdc++.h>

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

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		using num = modnum<int(1e9)+7>;

		string S;
		{
			string S_; cin >> S_;
			int N_ = int(S_.size());
			S.reserve(3*N_ + 4);
			S += 'o';
			for (int i = 0; i < N_; i++) S += 'x';
			S += '>';
			S += S_;
			S += '<';
			for (int i = 0; i < N_; i++) S += 'x';
			S += 'o';
		}

		int N = int(S.size());

		//cerr << "Case #" << case_num << "\n";
		//cerr << N << '\n';
		//cerr << S << '\n';

		std::vector<int> prv_W(N+1);
		prv_W[0] = 0;
		for (int i = 0; i < N; i++) {
			prv_W[i+1] = S[i] == '<' ? i : prv_W[i];
		}
		std::vector<int> nxt_W(N+1);
		nxt_W[N] = N;
		for (int i = N-1; i >= 0; i--) {
			nxt_W[i] = S[i] == '<' ? i : nxt_W[i+1];
		}

		std::vector<int> prv_E(N+1);
		prv_E[0] = 0;
		for (int i = 0; i < N; i++) {
			prv_E[i+1] = S[i] == '>' ? i : prv_E[i];
		}
		std::vector<int> nxt_E(N+1);
		nxt_E[N] = N;
		for (int i = N-1; i >= 0; i--) {
			nxt_E[i] = S[i] == '>' ? i : nxt_E[i+1];
		}

		std::vector<int> prv_M(N+1);
		prv_M[0] = 0;
		for (int i = 0; i < N; i++) {
			prv_M[i+1] = S[i] == '=' ? i : prv_M[i];
		}
		std::vector<int> nxt_M(N+1);
		nxt_M[N] = N;
		for (int i = N-1; i >= 0; i--) {
			nxt_M[i] = S[i] == '=' ? i : nxt_M[i+1];
		}

		std::vector<int> prv_O(N+1);
		prv_O[0] = 0;
		for (int i = 0; i < N; i++) {
			prv_O[i+1] = S[i] == 'o' ? i : prv_O[i];
		}
		std::vector<int> nxt_O(N+1);
		nxt_O[N] = N;
		for (int i = N-1; i >= 0; i--) {
			nxt_O[i] = S[i] == 'o' ? i : nxt_O[i+1];
		}

		auto is_valid = [&](int l, int r) -> bool {
			if (l >= r) return false;
			// cannot have an o strictly in between
			if (prv_O[r] > l) return false;
			// at most one equal
			if (nxt_M[l] <= r) {
				if (nxt_M[l] != prv_M[r+1]) return false;
				if (nxt_M[l] * 2 != l + r) return false;
			}
			// first E after l
			if (!(l + r < 2 * nxt_E[l])) return false;
			if (!(l + r > 2 * prv_W[r+1])) return false;
			return true;
		};

		std::vector<num> dp(N);
		std::vector<num> dp_updates(N+1);

		dp_updates[0] += 1;
		dp_updates[1] -= 1;
		for (int i = 0; i < N; i++) {
			num v = dp[i] = (i == 0 ? 0 : dp[i-1]) + dp_updates[i];

			//cerr << S[i] << ' ' << i << ' ' << v << '\n';

			if (S[i] == 'x' || S[i] == '=' || S[i] == '>' || S[i] == '<') {
				// not actually allowed
				continue;
			}

			assert(S[i] == 'o' || S[i] == '.');

			if (i == N-1) {
				// we're also done
				break;
			}

			// jump forwards from v
			assert(nxt_O[i+1] <= N-1);
			int known_bad = std::min(nxt_O[i+1]+1, 2 * nxt_E[i] - i);
			assert(known_bad <= N);
			{
				int m = nxt_M[i];
				int j = 2 * m - i;
				if (j < known_bad && is_valid(i, j)) {
					dp_updates[j] += v;
					dp_updates[j+1] -= v;
				}
				known_bad = std::min(known_bad, m);
			}
			int cnd_lo = i+1;
			while (cnd_lo < known_bad) {
				int pw = prv_W[cnd_lo+1];
				if (!(cnd_lo + i > 2 * pw)) {
					cnd_lo = 2 * pw - i + 1;
					continue;
				}
				assert(nxt_W[cnd_lo] > cnd_lo);
				int cnd_hi = std::min(known_bad, nxt_W[cnd_lo]);
				dp_updates[cnd_lo] += v;
				dp_updates[cnd_hi] -= v;
				cnd_lo = cnd_hi;
			}
		}

		num ans = dp[N-1];

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
