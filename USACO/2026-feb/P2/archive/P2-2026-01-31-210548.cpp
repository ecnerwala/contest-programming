#include <bits/stdc++.h>

int modinv(int a, int m) { return a == 1 ? 1 : m - int(int64_t(modinv(m % a, a)) * m / a); }

template <int MOD_> struct modnum {
	static constexpr int MOD = MOD_;
	int v;
	modnum() : v(0) {}
	modnum(int v_) : v(v_ % MOD) { if (v < 0) v += MOD; }
	explicit operator int () const { return v; }
	friend std::ostream& operator << (std::ostream& o, modnum n) { return o << int(n); }

	modnum& operator += (modnum o) {
		v += o.v;
		if (v >= MOD) v -= MOD;
		return *this;
	}
	modnum& operator -= (modnum o) {
		v -= o.v;
		if (v < 0) v += MOD;
		return *this;
	}
	modnum& operator *= (modnum o) {
		v = int(int64_t(v) * int64_t(o.v) % MOD);
		return *this;
	}
	friend modnum operator + (modnum a, modnum b) { return a += b; }
	friend modnum operator - (modnum a, modnum b) { return a -= b; }
	friend modnum operator * (modnum a, modnum b) { return a *= b; }

	friend modnum inv(modnum a) {
		modnum r;
		r.v = modinv(a.v, MOD);
		return r;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	using num = modnum<int(1e9)+7>;

	auto solve = [&](int N, int M, int64_t K, std::vector<num> P_pos, std::vector<int> X) -> std::vector<num> {
		auto div_mod = [&](int64_t a, int m) -> std::pair<int64_t, int> {
			int64_t v = a / m;
			int64_t r = int(a % m);
			if (r < 0) v--, r += m;
			return {v, int(r)};
		};
		auto mod = [&](int64_t a, int m) -> int { return div_mod(a, m).second; };

		// Let's find the initial value
		std::vector<std::pair<int, std::pair<int, bool>>> evts;
		evts.reserve(2*N);
		std::vector<std::array<int, 2>> cnt_vals(N);
		for (int i = 0; i < N; i++) {
			for (int z = 0; z < 2; z++) {
				auto [cnt, pos] = div_mod(z ? X[i] - K : X[i] + K, M);
				cnt = mod(cnt, N);
				evts.push_back({pos, {i, z}});
				cnt_vals[i][z] = int(cnt);
			}
		}
		std::sort(evts.begin(), evts.end());
		// Every time we cross an event, we recalculate the thing
		std::vector<num> ans(M);
		for (auto [pos, upd] : evts) {
			// product the things
			std::vector<num> dp(N);
			dp[0] = 1;
			for (int i = 0; i < N; i++) {
				std::vector<num> ndp(N);
				for (int a = 0; a < N; a++) {
					if (i != upd.first || upd.second == 0) ndp[(a+cnt_vals[i][0])%N] += P_pos[i] * dp[a];
					if (i != upd.first || upd.second == 1) ndp[(a+cnt_vals[i][1])%N] += (1-P_pos[i]) * dp[a];
				}
				dp = ndp;
			}
			ans[pos] += dp[0];
			{
				auto& r = cnt_vals[upd.first][upd.second];
				if (r == 0) r = N;
				r--;
			}
		}
		return ans;
	};

	auto slow = [&](int N, int M, int64_t K, std::vector<num> P_pos, std::vector<int> X) -> std::vector<num> {
		std::vector<num> ans(M);
		M *= 2;
		for (auto& x : X) x *= 2;
		K *= 2;
		for (int msk = 0; msk < (1 << N); msk++) {
			num prob = 1;
			std::vector<std::pair<int, int>> people(N);
			for (int i = 0; i < N; i++) {
				bool dir = (msk >> i) & 1;
				prob *= dir ? 1 - P_pos[i] : P_pos[i];
				people[i] = {X[i], dir};
			}
			for (int z = 0; z < K; z++) {
				for (int i = 0; i < N; i++) {
					auto& v = people[i].first;
					if (people[i].second) {
						if (v == 0) v = M;
						v--;
					} else {
						v++;
						if (v == M) v = 0;
					}
				}
				for (int i = 0; i < N; i++) {
					if (people[i].first == people[(i+1)%N].first && !people[i].second && people[(i+1)%N].second) {
						// flip them
						people[i].second = true;
						people[(i+1)%N].second = false;
					}
				}
			}
			assert(!(people[0].first & 1));
			ans[people[0].first/2] += prob;
		}
		return ans;
	};

	int T; cin >> T; while (T--) {
		int N, M; int64_t K; cin >> N >> M >> K;
		std::vector<num> P_pos(N);
		for (auto& p : P_pos) { int v; cin >> v; p = v; }
		std::vector<int> X(N);
		for (auto& x : X) cin >> x;

		//auto ans = solve(N, M, K, P_pos, X);
		auto ans = slow(N, M, K, P_pos, X);
		for (int m = 0; m < M; m++) cout << ans[m] << " \n"[m+1==M];
	}

	return 0;
}
