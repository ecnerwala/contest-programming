#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	vector<array<int, 3>> A(N-1);
	for (int z = 0; z < 3; z++) {
		for (auto& a : A) cin >> a[z];
	}

	vector<array<int, 2>> B(N);
	for (int z = 0; z < 2; z++) {
		for (auto& b : B) cin >> b[z];
	}

	vector<array<bool, 3>> filled(N, array<bool, 3>{false, false, false});
	for (int z = 0; z < M; z++) {
		int r, c; cin >> r >> c; r--, c--;
		filled[c][r] = true;
	}
	
	map<pair<array<int, 3>, int>, int64_t> dp;
	dp[{{0,1,2},0}] = 0;

	for (int i = 0; i < N; i++) {
		{
			// Add people and break horizontal walls
			map<pair<array<int, 3>, int>, int64_t> ndp;
			auto relax = [&](pair<array<int, 3>, int> state, int64_t v) {
				auto it = ndp.find(state);
				if (it == ndp.end()) ndp.insert({state, v});
				else it->second = min(it->second, v);
			};
			for (auto it : dp) {
				const auto conn = it.first.first;
				auto msk = it.first.second;
				for (int z = 0; z < 3; z++) {
					if (filled[i][z]) msk |= (1 << conn[z]);
				}

				// We can break none
				relax({conn, msk}, it.second);

				{
					// We can break all 3
					relax({{0,0,0}, (msk & 8) | bool(msk & 7)}, it.second + B[i][0] + B[i][1]);
				}
				if (conn[0] != conn[1]) {
					// We can break the top 2
					int a = min(conn[0], conn[1]);
					int b = max(conn[0], conn[1]);
					auto nconn = conn;
					if (nconn[2] == b) nconn[2] = a;
					nconn[0] = nconn[1] = a;

					int nmsk = msk;
					nmsk |= ((nmsk >> b) & 1) << a;
					nmsk ^= ((nmsk >> b) & 1) << b;
					relax({nconn, nmsk}, it.second + B[i][0]);
				}
				if (conn[1] != conn[2]) {
					// We can break the bottom 2
					int a = min(conn[1], conn[2]);
					int b = max(conn[1], conn[2]);
					auto nconn = conn;
					if (nconn[0] == b) nconn[0] = a;
					nconn[1] = nconn[2] = a;

					int nmsk = msk;
					nmsk |= ((nmsk >> b) & 1) << a;
					nmsk ^= ((nmsk >> b) & 1) << b;
					relax({nconn, nmsk}, it.second + B[i][1]);
				}
			}
			dp = std::move(ndp);
		}

		if (i < N-1) {
			map<pair<array<int, 3>, int>, int64_t> ndp;
			auto relax = [&](pair<array<int, 3>, int> state, int64_t v) {
				auto it = ndp.find(state);
				if (it == ndp.end()) ndp.insert({state, v});
				else it->second = min(it->second, v);
			};
			for (auto it : dp) {
				const auto conn = it.first.first;
				const auto msk = it.first.second;
				for (int msk2 = 0; msk2 < 8; msk2++) {
					int64_t cost = it.second;
					for (int z = 0; z < 3; z++) {
						if (msk2 & (1 << z)) {
							cost += A[i][z];
						}
					}

					bool is_good = true;
					array<int, 3> nconn{0,1,2};
					int nmsk = msk & 8;
					for (int z = 0; z < 3; z++) {
						if (conn[z] == z) {
							int nidx = 3;
							for (int x = 0; x < 3; x++) {
								if (conn[x] == z) {
									if (msk2 & (1 << x)) {
										nidx = min(nidx, x);
									}
								}
							}
							for (int x = 0; x < 3; x++) {
								if (conn[x] == z) {
									if (msk2 & (1 << x)) {
										nconn[x] = nidx;
									}
								}
							}
							if (msk & (1 << z)) {
								if (nmsk & (1 << nidx)) {
									is_good = false;
									break;
								}
								nmsk |= (1 << nidx);
							}
						}
					}
					if (!is_good) continue;
					relax({nconn, nmsk}, cost);
				}
			}
			dp = std::move(ndp);
		}
	}

	int64_t ans = 1e18;
	for (auto it : dp) {
		int msk = it.first.second;
		if (msk & (msk-1)) continue;
		ans = min(ans, it.second);
	}
	cout << ans << '\n';

	return 0;
}
