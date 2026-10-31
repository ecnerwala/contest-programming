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

template <typename T> T pow(T a, long long b) {
	assert(b >= 0);
	T r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

using mnum = modnum<int(1e9)+7>;

auto solve(int64_t N, std::string S) -> std::optional<std::pair<mnum, mnum>> {
	if (N == 0) {
		if (S == "a") return {{1, 1}};
		else return std::nullopt;
	} else if (N == 1) {
		if (S == "a" || S == "b" || S == "ab") {
			return {{1, 3}};
		} else {
			return std::nullopt;
		}
	}
	assert(N >= 2);
	if (int(S.size()) == 1) {
		// This is far more common than anything other than 1-character strings, so we can just special case it
		return {{pow(mnum(2), N-1), mnum(2)}};
	}
	assert(int(S.size()) >= 2);
	int64_t cur_level = N;
	while (true) {
		std::optional<bool> offset;
		for (int i = 1; ; i++) {
			if (i == 4) {
				offset = 0;
				break;
			} else if (i == int(S.size())) {
				offset = std::nullopt;
				break;
			} else if (S[i-1] == S[i]) {
				offset = i & 1;
				break;
			} else {
				continue;
			}
			assert(false);
		}

		if (!offset) {
			break;
		}
		cur_level--;
		if (cur_level <= 0) {
			return std::nullopt;
		}

		std::string NS; NS.reserve((int(S.size()) + *offset + 1) / 2);
		for (int i = -*offset; i < int(S.size()); i += 2) {
			if (i == -1) NS.push_back('a' ^ 'b' ^ S[i+1]);
			else if (i+1 < int(S.size()) && S[i] == S[i+1]) return std::nullopt;
			else NS.push_back(S[i]);
		}
		S = std::move(NS);
		assert(int(S.size()) >= 2);
	}

	// how many things reduce to 2 at this level
	auto get_2_cnt = [&](int64_t level) -> mnum {
		if (N - level < 0) {
			return 0;
		} else if (N-level == 0) {
			return 1;
		} else if (N-level == 1) {
			return 4;
		} else {
			return mnum(13) * pow(mnum(4), N - level - 2);
		}
	};
	// how many things reduce to 3 at this level
	auto get_3_cnt = [&](int64_t level) -> mnum {
		if (N - level < 0) {
			return 0;
		} else {
			return pow(mnum(4), N - level);
		}
	};

	assert(cur_level > 0);
	assert(int(S.size()) >= 2);
	assert(int(S.size()) < 4);

	// These have the same count, so we get a little lucky
	if (S == "aba" || S == "bab") {
		cur_level--;
		S = "ba";
	}

	if (S == "ab" && cur_level == 1) cur_level = 2;
	if (cur_level <= 1) return std::nullopt;

	assert(cur_level >= 2);

	if (S == "ab" && !(cur_level & 1)) S = "ba";

	if (S == "ab") {
		assert(cur_level & 1);
		assert(cur_level >= 3);
		return {{(pow(mnum(2), cur_level) + 1) / mnum(3), get_2_cnt(cur_level)}};
	} else if (S == "ba") {
		if (cur_level == 2) {
			return {{1, get_2_cnt(cur_level-1) + get_2_cnt(cur_level) * 2 + 2 * get_3_cnt(cur_level+1)}};
		} else {
			return {{(pow(mnum(2), cur_level) - pow(mnum(2), cur_level & 1)) / mnum(3), get_2_cnt(cur_level) * (cur_level & 1 ? 1 : 2) + 2 * get_3_cnt(cur_level+1)}};
		}
	} else assert(false);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t N; string S; cin >> N >> S;
		auto res = solve(N, std::move(S));
		if (res) {
			cout << res->first << ' ' << res->second << '\n';
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
