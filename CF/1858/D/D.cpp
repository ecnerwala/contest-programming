#include <bits/stdc++.h>

void solve() {
	int N, K; std::cin >> N >> K;
	std::string S; std::cin >> S;
	std::array<std::vector<int>, 2> locs;
	locs[0].reserve((K+1)+N+(K+1));
	locs[1].reserve((K+1)+N+(K+1));
	locs[0].assign(K+1, -1);
	locs[1].assign(K+1, -1);
	for (int i = 0; i < N; i++) {
		locs[S[i]-'0'].push_back(i);
	}
	locs[0].insert(locs[0].end(), K+1, N);
	locs[1].insert(locs[1].end(), K+1, N);

	std::vector<int> best_0(N+1, -1);
	auto process_cnd = [&](int v0, int v1) {
		best_0[v0] = std::max(best_0[v0], v1);
	};
	{
		std::vector<int> pref(N+1);
		std::vector<int> suff(N+1);
		for (int z = 0; z < 2; z++) {
			for (int k_lo = 0; k_lo <= K; k_lo++) {
				std::array<int, 2> cnts{K+1, K+1};
				for (int i = 0; i <= N; i++) {
					// prefix is strings of !z
					pref[i] = i - locs[z][cnts[z] - (k_lo+1)] - 1;
					// suffix is strings of z
					suff[i] = locs[!z][cnts[!z]-1 + ((K-k_lo)+1)] - i;
					if (i < N) cnts[S[i] - '0']++;
				}
				assert(pref[0] == 0);
				assert(suff[N] == 0);
				for (int i = N; i > 0; i--) {
					suff[i-1] = std::max(suff[i-1], suff[i]);
				}
				for (int i = 1; i <= N; i++) {
					pref[i] = std::max(pref[i], pref[i-1]);
				}
				for (int i = 0; i <= N; i++) {
					if (z) {
						process_cnd(pref[i], suff[i]);
					} else {
						process_cnd(suff[i], pref[i]);
					}
				}
			}
		}
	}

	for (int a = 1; a <= N; a++) {
		int cnd = 0;
		for (int v0 = 0; v0 <= N; v0++) {
			if (best_0[v0] == -1) continue;
			cnd = std::max(cnd, v0 * a + best_0[v0]);
		}
		std::cout << cnd << " \n"[a==N];
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		solve();
	}

	return 0;
}
