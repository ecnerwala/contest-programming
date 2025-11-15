#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, K; cin >> N >> K;
		assert(K < N);
		int M; cin >> M;
		std::vector<std::vector<int>> trams(M);
		int tot_l = 0;
		for (int m = 0; m < M; m++) {
			int l; cin >> l;
			tot_l += l;
			trams[m].resize(l);
			for (auto& v : trams[m]) { cin >> v; v--; }
		}

		std::vector<std::vector<std::pair<int, int>>> starts(N);
		for (int m = 0; m < M; m++) {
			for (int z = 0; z < int(trams[m].size()); z++) {
				starts[trams[m][z]].push_back({m, z});
			}
		}

		std::vector<int> dist(N, -1);
		std::vector<std::vector<int>> dist2(M);
		for (int m = 0; m < M; m++) dist2[m].assign(trams[m].size(), -1);
		std::vector<int> q_dist; q_dist.reserve(N);
		std::vector<std::pair<int, int>> q_tram; q_tram.reserve(tot_l);
		std::vector<std::pair<int, int>> nq_tram; nq_tram.reserve(tot_l);

		dist[0] = 0;
		q_dist.push_back(0);

		for (int d = 0; true; d++) {
			int d_ceil = (d + K+1) / (K+2);
			if (d % (K+2) == 0) {
				if (q_dist.empty()) break;
				for (int v : q_dist) {
					cerr << "at " << d << ' ' << v << '\n';
					assert(dist[v] == d_ceil);
					for (auto [m, z] : starts[v]) {
						if (dist2[m][z] == -1) {
							dist2[m][z] = d+1;
							q_tram.push_back({m, z});
						}
					}
				}
				q_dist.clear();
			} else {
				assert(d_ceil * (K+2) > d);
				for (auto [m, z] : q_tram) {
					cerr << "at " << d << ' ' << m << ' ' << z << '\n';
					assert(dist2[m][z] == d);
					{
						int v = trams[m][z];
						if (dist[v] == -1) {
							dist[v] = d_ceil;
							q_dist.push_back(v);
						}
					}
					if (z+1 < int(trams[m].size()) && d+1 < d_ceil * (K+2) && dist2[m][z+1] == -1) {
						dist2[m][z+1] = d+1;
						nq_tram.push_back({m, z+1});
					}
				}
				q_tram.clear();
				std::swap(q_tram, nq_tram);
			}
		}

		int64_t ans = 0;
		for (int i = 0; i < N; i++) {
			ans += dist[i] * int64_t(i+1);
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
