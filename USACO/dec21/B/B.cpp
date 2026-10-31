#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }
template <typename T> void setmax(T& a, T b) { if (b > a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;

	int N, K; cin >> N >> K;
	struct cow_t {
		int x, w;
	};
	std::array<std::vector<cow_t>, 2> cows;
	cows[0].reserve(N);
	cows[1].reserve(N);
	for (int i = 0; i < N; i++) {
		char b; int x, w; cin >> b >> x >> w;
		cows[b == 'H'].push_back({x, w});
	}
	int A = int(cows[0].size());
	int B = int(cows[1].size());
	assert(A + B == N);

	if (T == 1) {
		// minimize unpaired/maximize pairing
		//
		const int INF = int(1e9) + 100;

		std::vector<int> dp((A+1)*(B+1), INF);
		dp[0] = 0;
		for (int i = 0; i <= A; i++) {
			for (int j = 0; j <= B; j++) {
				if (dp[i*(B+1)+j] == INF) continue;
				if (i < A && j < B && std::abs(cows[0][i].x - cows[1][j].x) <= K) {
					setmin(dp[(i+1)*(B+1)+(j+1)], dp[i*(B+1)+j]);
				}
				if (i < A) {
					setmin(dp[(i+1)*(B+1)+j], dp[i*(B+1)+j] + cows[0][i].w);
				}
				if (j < B) {
					setmin(dp[i*(B+1)+(j+1)], dp[i*(B+1)+j] + cows[1][j].w);
				}
			}
		}
		cout << dp.back() << '\n';
	} else if (T == 2) {
		struct dp_t {
			int all_best = -1;
			int far_best = -1;
			//std::vector<int> by_star;
			int idx = 0;
			int offset = 0;
			int en = 0;
		};
		std::vector<std::array<dp_t, 2>> dp((A+1)*(B+1));

		dp[0][0].all_best = dp[0][0].far_best = 0;
		//dp[0][1].all_best = dp[0][1].far_best = 0;

		int buf_size = 0;
		for (int j = 0; j <= B; j++) {
			int num_steps = min(B-j, A)+1;
			assert(num_steps <= N);
			for (int z = 0; z < 2; z++) {
				dp[j][z].idx = dp[j][z].en = buf_size;
				buf_size += num_steps;
			}
		}
		for (int i = 1; i <= A; i++) {
			int num_steps = min(A-i, B)+1;
			assert(num_steps <= N);
			for (int z = 0; z < 2; z++) {
				dp[i*(B+1)][z].idx = dp[i*(B+1)][z].en = buf_size;
				buf_size += num_steps;
			}
		}

		std::vector<int> buf(buf_size);

		for (int i = 0; i <= A; i++) {
			for (int j = 0; j <= B; j++) {
				auto& cur_dp = dp[i*(B+1) + j];
				auto bump_dp = [&](int z, int loc) {
					auto& a = cur_dp[z];
					while (a.idx < a.en && cows[z][a.idx + a.offset].x < loc - K) {
						setmax(a.far_best, buf[a.idx]);
						a.idx++;
					}
				};
				if (i < A) {
					// try skipping i
					int v = -1;
					setmax(v, cur_dp[0].all_best);
					bump_dp(1, cows[0][i].x);
					setmax(v, cur_dp[1].far_best);

					if (v != -1) {
						v += cows[0][i].w;
					}
					auto& nxt_dp = dp[(i+1)*(B+1) + j];
					setmax(nxt_dp[0].all_best, v);
					//assert(int(nxt_dp[0].by_star.size()) > nxt_dp[0].en);
					nxt_dp[0].offset = i - nxt_dp[0].en;
					buf[nxt_dp[0].en] = v;
					nxt_dp[0].en++;
				}
				if (j < B) {
					// try skipping j
					int v = -1;
					setmax(v, cur_dp[1].all_best);
					bump_dp(0, cows[1][j].x);
					setmax(v, cur_dp[0].far_best);

					if (v != -1) {
						v += cows[1][j].w;
					}
					auto& nxt_dp = dp[i*(B+1) + (j+1)];
					setmax(nxt_dp[1].all_best, v);
					//assert(int(nxt_dp[1].by_star.size()) > nxt_dp[1].en);
					nxt_dp[1].offset = j - nxt_dp[1].en;
					buf[nxt_dp[1].en] = v;
					nxt_dp[1].en++;
				}

				if (i < A && j < B) {
					auto& nxt_dp = dp[(i+1)*(B+1) + (j+1)];
					nxt_dp = std::move(cur_dp);
					if (std::abs(cows[0][i].x - cows[1][j].x) <= K) {
						// we just move it forwards
					} else {
						nxt_dp[0].all_best = nxt_dp[0].far_best = -1;
						nxt_dp[1].all_best = nxt_dp[1].far_best = -1;
						nxt_dp[0].idx = nxt_dp[0].en;
						nxt_dp[1].idx = nxt_dp[1].en;
					}
				}
			}
		}

		cout << std::max(dp.back()[0].all_best, dp.back()[1].all_best) << '\n';
	} else assert(false);

	return 0;
}
