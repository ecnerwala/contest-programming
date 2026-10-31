#include <bits/stdc++.h>

template <typename T> T mod_inv_in_range(T a, T m) {
	// assert(0 <= a && a < m);
	T x = a, y = m;
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

	string S; cin >> S;

	using state_t = std::array<std::pair<int, int>, 4>;
	auto apply_a = [&](state_t s) -> state_t {
		for (auto& [cur, cnt] : s) {
			if (cur & 1) {
				cur = 0;
				cnt++;
			} else {
				cur |= 1;
			}
		}
		return s;
	};

	auto apply_b = [&](state_t s) -> state_t {
		for (auto& [cur, cnt] : s) {
			if (cur & 2) {
				cur = 0;
				cnt--;
			} else {
				cur |= 2;
			}
		}
		return s;
	};

	// 0 is a win, 1 is tie, 2 is b win
	auto eval_state = [&](const state_t& s) -> int {
		int a = 0;
		std::array<bool, 4> seen{false, false, false, false};
		while (!seen[a]) {
			seen[a] = true;
			a = s[a].first;
		}
		int tot = 0;
		int b = a;
		do {
			tot += s[b].second;
			b = s[b].first;
		} while (b != a);
		if (tot > 0) {
			return 0;
		} else if (tot == 0) {
			return 1;
		} else if (tot < 0) {
			return 2;
		} else assert(false);
	};

	state_t start;
	for (int i = 0; i < 4; i++) start[i] = {i, 0};

	using num = modnum<998244353>;

	auto strip_state = [&](state_t s) -> state_t {
		for (auto& [_, v] : s) v = 0;
		return s;
	};
	std::vector<state_t> poss;
	poss.push_back(strip_state(start));
	for (int z = 0; z < int(poss.size()); z++) {
		for (state_t nxt : {strip_state(apply_a(poss[z])), strip_state(apply_b(poss[z]))}){
			if (std::find(poss.begin(), poss.end(), nxt) == poss.end()) {
				poss.push_back(nxt);
			}
		}
	}
	std::vector<state_t> pos_loops;
	for (state_t s : poss) {
		state_t looped = s;
		std::array<bool, 4> seen{false, false, false, false};
		{
			int a = 0;
			while (!seen[a]) {
				seen[a] = true;
				a = s[a].first;
			}
		}
		for (int i = 0; i < 4; i++) if (!seen[i]) looped[i].first = -1;
		if (std::find(pos_loops.begin(), pos_loops.end(), looped) == pos_loops.end()) {
			pos_loops.push_back(looped);
		}
	}
	cerr << pos_loops.size() << '\n';

	std::array<num, 3> ans{0, 0, 0};
	for (auto target_state : pos_loops) {
		std::array<bool, 4> seen{false, false, false, false};
		std::array<bool, 4> in_loop{false, false, false, false};
		{
			int a = 0;
			while (!seen[a]) {
				seen[a] = true;
				cerr << (a&1) << ':' << (a>>1&1) << " -> ";
				a = target_state[a].first;
			}
			cerr << (a&1) << ':' << (a>>1&1) << '\n';
			int b = a;
			do {
				in_loop[b] = true;
				b = target_state[b].first;
			} while (b != a);
		}

		state_t local_start = start;
		for (int i = 0; i < 4; i++) if (!seen[i]) local_start[i].first = -1;
		std::vector<state_t> local_poss;
		local_poss.push_back(local_start);

		std::vector<std::array<std::pair<int, int>, 2>> trans;
		for (int z = 0; z < int(local_poss.size()); z++) {
			trans.emplace_back();
			for (int v = 1; v <= 2; v++) {
				int diff = 0;
				state_t s = local_poss[z];
				for (int i = 0; i < 4; i++) {
					if (s[i].first == -1) continue;
					if (s[i].first & v) {
						s[i].first = 0;
						if (in_loop[i]) {
							diff += (v == 1 ? 1 : -1);
						}
					} else {
						s[i].first |= v;
					}
				}
				auto it = std::find(local_poss.begin(), local_poss.end(), s);
				if (it == local_poss.end()) {
					local_poss.push_back(s);
					it = std::prev(local_poss.end());
				}
				trans[z][v-1] = {int(it - local_poss.begin()), diff};
			}
		}

		int tgt = int(std::find(local_poss.begin(), local_poss.end(), target_state) - local_poss.begin());
		assert(tgt < int(local_poss.size()));

		std::vector<bool> maybe_good(local_poss.size());
		maybe_good[tgt] = true;
		for (int z = 0; z < int(local_poss.size()); z++) {
			for (int i = 0; i < int(local_poss.size()); i++) {
				maybe_good[i] = maybe_good[i] || maybe_good[trans[i][0].first] || maybe_good[trans[i][1].first];
			}
		}
		assert(maybe_good[0]);
		for (int i = 0; i < int(local_poss.size()); i++) {
			for (int z = 0; z < 2; z++) {
				if (!maybe_good[trans[i][z].first]) trans[i][z].first = -1;
			}
		}

		std::vector<std::map<int, num>> dp(local_poss.size());
		dp[0][0] = num(1);
		for (char c : S) {
			std::vector<std::map<int, num>> ndp(local_poss.size());
			for (int cur = 0; cur < int(local_poss.size()); cur++) {
				for (auto [v, cnt] : dp[cur]) {
					if (c == 'a' || c == '?') {
						auto [nxt, diff] = trans[cur][0];
						if (nxt != -1) ndp[nxt][v + diff] += cnt;
					}
					if (c == 'b' || c == '?') {
						auto [nxt, diff] = trans[cur][1];
						if (nxt != -1) ndp[nxt][v + diff] += cnt;
					}
				}
			}
			dp = std::move(ndp);
		}
		for (auto [v, cnt] : dp[tgt]) {
			if (v > 0) {
				ans[0] += cnt;
			} else if (v == 0) {
				ans[1] += cnt;
			} else if (v < 0) {
				ans[2] += cnt;
			} else assert(false);
		}
	}

	cout << ans[0] << '\n' << ans[1] << '\n' << ans[2] << '\n';

	return 0;
}
