#include <bits/stdc++.h>

std::pair<int, std::vector<int>> solve_real(const std::vector<int> S, int A, int B) {
	int N = int(S.size());
	std::vector<int> ans(N);
	auto solve = [&](int V, bool do_gen) -> bool {
		std::vector<std::array<int, 2>> dp(N+1);
		dp[N] = {0, 0};
		for (int i = N-1; i >= 0; i--) {
			if (S[i] == 0) {
				dp[i][1] = V+1;
			} else {
				dp[i][1] = dp[i+1][0] <= V ? 1 : dp[i+1][1] + 1;
				dp[i][1] = std::min(dp[i][1], V+1);
			}
			if (S[i] == 1) {
				dp[i][0] = V+1;
			} else {
				dp[i][0] = dp[i+1][1] <= V ? 1 : dp[i+1][0] + 1;
				dp[i][0] = std::min(dp[i][0], V+1);
			}
			if (dp[i][0] > V && dp[i][1] > V) return false;
		}

		auto run_sim = [&](int amt_0) {
			int num_0 = 0;

			int cur_run = 0;
			int cur_char = 0;
			for (int i = 0; i < N; i++) {
				auto cnd_good = [&](int cnd) -> bool {
					if (S[i] == 1-cnd) return false;
					int nxt_char = cnd;
					int nxt_run = cnd == cur_char ? cur_run + 1 : 1;
					if (nxt_run > V) return false;
					if (dp[i+1][!nxt_char] <= V) return true;
					if (nxt_run + dp[i+1][nxt_char] <= V) return true;
					return false;
				};
				int cnd = (i < amt_0) ? 0 : 1;
				if (cnd_good(cnd)) {
				} else {
					cnd = 1-cnd;
					assert(cnd_good(cnd));
				}
				ans[i] = cnd;
				num_0 += (S[i] == -1) && (cnd == 0);
				cur_run = (cnd == cur_char) ? cur_run + 1 : 1;
				cur_char = cnd;
			}
			return num_0;
		};

		if (run_sim(0) > A || run_sim(N) < A) {
			return false;
		}

		if (!do_gen) return true;

		int mi = -1;
		int ma = N+1;
		while (true) {
			int md = (mi + ma) / 2;
			int n0 = run_sim(md);
			if (n0 < A) {
				mi = md;
			} else if (n0 > A) {
				ma = md;
			} else {
				return true;
			}
		}
	};

	int mi = 0;
	int ma = N+1;
	while (ma - mi > 1) {
		int md = (mi + ma) / 2;
		if (solve(md, false)) {
			ma = md;
		} else {
			mi = md;
		}
	}
	auto res = solve(ma, true);
	assert(res);
	return {ma, ans};
}

void check_real(std::vector<int> V, int A, int B, int val, std::vector<int> ans) {
	int N = int(ans.size());
	for (int i = 0; i < N; i++) {
		if (V[i] != -1) assert(ans[i] == V[i]);
		else if (ans[i] == 0) A--;
		else if (ans[i] == 1) B--;
		else assert(false);
	}
	assert(A == 0 && B == 0);
	int max_run = 0;
	int cur_run = 0;
	int cur_char = 0;
	for (int c : ans) {
		if (c != cur_char) cur_run = 0;
		cur_run++;
		cur_char = c;
		max_run = std::max(max_run, cur_run);
	}
	assert(max_run == val);
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N, A, B; cin >> N >> A >> B;
		string S; cin >> S;
		assert(int(S.size()) == N);

		if (N == 1) {
			if (A == 1) S[0] = '0';
			else if (B == 1) S[0] = '1';
			cout << 1 << '\n';
			cout << S << '\n';
			continue;
		}
		assert(N >= 2);

		std::vector<int> V(N);
		for (int i = 0; i < N; i++) {
			V[i] = (S[i] == '?' ? -1 : (S[i] - '0'));
		}
		auto [ma, res] = solve_real(V, A, B);
		cout << ma << '\n';
		for (int v : res) { cout << v; } cout << '\n';
		//check_real(V, A, B, ma, res);
	}

	return 0;
}
