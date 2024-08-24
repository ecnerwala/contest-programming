#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<std::array<int, 2>> A(N);
	std::vector<std::pair<int, std::array<int, 2>>> vals; vals.reserve(2*N);
	for (int z = 0; z < 2; z++) {
		for (int i = 0; i < N; i++) {
			cin >> A[i][z];
			vals.push_back({A[i][z], {i, z}});
		}
	}

	std::sort(vals.begin(), vals.end());
	for (int v = 0; v < 2*N; v++) {
		A[vals[v].second[0]][vals[v].second[1]] = v;
	}

	std::vector<int> pair(2*N);
	for (int i = 0; i < N; i++) {
		pair[A[i][0]] = A[i][1];
		pair[A[i][1]] = A[i][0];
	}

	std::array<std::vector<int>, 2> suff_cnt;
	std::array<std::vector<std::array<int64_t, 2>>, 2> suff_sum;
	for (int z = 0; z < 2; z++) {
		suff_cnt[z].resize(2*N+1);
		suff_sum[z].resize(2*N+1);
		suff_cnt[z][2*N] = 0;
		suff_sum[z][2*N] = {0, 0};
		for (int v = 2*N-1; v >= 0; v--) {
			suff_cnt[z][v] = suff_cnt[z][v+1];
			suff_sum[z][v] = suff_sum[z][v+1];
			if (pair[v] > v && vals[v].second[1] == z) {
				suff_sum[z][v][suff_cnt[z][v+1] & 1] += vals[v].first;
				suff_cnt[z][v] ++;
			}
		}
	}

	const int64_t INF = 1e18;
	std::vector<std::array<int64_t, 2>> dp_mid(2*N+1, std::array<int64_t, 2>{INF, INF});
	dp_mid[2*N][0] = 0;
	std::array<std::vector<std::pair<int, int>>, 2> chain;
	std::array<std::vector<std::array<int64_t, 2>>, 2> dp_chain;

	auto transition = [&](int p, int from, int to, int z) -> int64_t {
		assert(p == 0 || p == 1);
		assert(from >= to);
		assert(z == 0 || z == 1);
		return suff_sum[z][to][p] - suff_sum[z][from][p];
	};

	for (int v = 2*N-1; v >= 0; v--) {
		{
			// transition from v+1 to v
			for (int p = 0; p < 2; p++) {
				setmin(dp_mid[v][p], dp_mid[v+1][p] + transition(p, v+1, v, 0) + transition(p, v+1, v, 1));
			}
		}
		if (pair[v] > v) {
			int z = vals[v].second[1];
			if (chain[z].empty() || pair[v] < chain[z].back().second) {
				// transition the chain
				// transition to v+1, pair[v]+1
				std::array<int64_t, 2> cnd;
				for (int p = 0; p < 2; p++) {
					cnd[p] = dp_mid[pair[v]+1][p] + transition(p, pair[v] + 1, v + 1, z);
					if (!chain[z].empty()) {
						auto u = chain[z].back().first;
						setmin(cnd[p],
							dp_chain[z].back()[p]
							+ transition(p, pair[u], pair[v] + 1, !z)
							+ transition(p, u, v + 1, z)
						);
					}
				}
				cnd[suff_cnt[!z][pair[v] + 1] & 1] += vals[pair[v]].first;
				std::swap(cnd[0], cnd[1]);
				chain[z].push_back({v, pair[v]});
				dp_chain[z].push_back(cnd);
				// transition back to the main chain
				for (int p = 0; p < 2; p++) {
					setmin(dp_mid[v][p], cnd[p] + transition(p, pair[v], v, !z));
				}
			}
		}
	}
	cout << std::min(dp_mid[0][0], dp_mid[0][1]) << '\n';

	return 0;
}
