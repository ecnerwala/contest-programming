#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	uint64_t S, T; cin >> S >> T;
	int64_t A; cin >> A;
	std::vector<std::pair<uint64_t, int64_t>> Y(N);
	for (auto& [y, c] : Y) cin >> y >> c;

	const int64_t INF = 4e18;
	int64_t ans = INF;

	std::vector<uint64_t> xor_values(1<<N);
	std::vector<int64_t> costs(1<<N);
	xor_values[0] = 0;
	costs[0] = 0;
	for (int i = 0; i < N; i++) {
		for (int m = 0; m < (1<<i); m++) {
			xor_values[m+(1<<i)] = xor_values[m] ^ Y[i].first;
			costs[m+(1<<i)] = costs[m] + Y[i].second;
		}
	}

	for (int msk = 0; msk < (1 << N); msk++) {
		if ((S ^ xor_values[msk]) == T) {
			ans = std::min(ans, costs[msk]);
		}
	}

	std::vector<int64_t> s_dist = costs;
	std::vector<int64_t> t_dist = costs;
	std::vector<int64_t> m_dist = costs;

	for (int b = 0; b < 40; b++) {
		std::vector<int64_t> dist;
		min_pq<std::pair<int64_t, int>> pq;
		auto reset = [&](int num_verts) -> void {
			dist.assign(num_verts, INF);
			while (!pq.empty()) pq.pop();
		};
		auto check_vert = [&](int v, int64_t d) -> void {
			if (d < dist[v]) {
				dist[v] = d;
				pq.push({d, v});
			}
		};

		reset(1<<(N+1));
		for (int m = 0; m < (1 << N); m++) {
			check_vert((bool(((S ^ xor_values[m]) >> b) & 1) << N) + m, s_dist[m]);
		}
		while (!pq.empty()) {
			auto [d, v] = pq.top(); pq.pop();
			if (d != dist[v]) continue;
			if (v < (1 << N)) {
				// increment then go to T
				for (int n = 0; n < (1 << N); n++) {
					if (((S ^ xor_values[v]) >> (b+1)) * 2 + 1 == ((T ^ xor_values[n]) >> b)) {
						ans = std::min(ans, d + A + t_dist[n]);
					}
				}
				// costs 1 to increment, and then jump using m_dist
				for (int msk = 0; msk < (1 << N); msk++) {
					// switches to 1, and then the mask does something
					bool switches = 1 ^ ((xor_values[msk] >> b) & 1);
					check_vert((v ^ msk) + (switches << N), d + A + m_dist[msk]);
				}
			}
		}
		for (int m = 0; m < (1 << N); m++) {
			s_dist[m] = dist[(1<<N)+m];
		}

		reset(1<<(N+1));
		for (int n = 0; n < (1 << N); n++) {
			check_vert((bool(((T ^ xor_values[n]) >> b) & 1) << N) + n, t_dist[n]);
		}
		while (!pq.empty()) {
			auto [d, v] = pq.top(); pq.pop();
			if (d != dist[v]) continue;
			if (v >= (1 << N)) {
				// costs 1 to decrement, and then jump using m_dist
				for (int msk = 0; msk < (1 << N); msk++) {
					bool switches = 0 ^ ((xor_values[msk] >> b) & 1);
					check_vert(((v-(1<<N)) ^ msk) + (switches << N), d + A + m_dist[msk]);
				}
			}
		}
		for (int n = 0; n < (1 << N); n++) {
			t_dist[n] = dist[n];
		}

		reset(1<<(N+1));
		for (int m = 0; m < (1 << N); m++) {
			check_vert((bool((xor_values[m] >> b) & 1) << N) + m, m_dist[m]);
		}
		while (!pq.empty()) {
			auto [d, v] = pq.top(); pq.pop();
			if (d != dist[v]) continue;
			if (v < (1 << N)) {
				// increment
				for (int msk = 0; msk < (1 << N); msk++) {
					bool switches = 1 ^ ((xor_values[msk] >> b) & 1);
					check_vert((v ^ msk) + (switches << N), d + A + m_dist[msk]);
				}
			}
		}
		for (int m = 0; m < (1 << N); m++) {
			m_dist[m] = dist[(1<<N)+m];
		}

		//cerr << b << ' ' << ans << '\n';
	}

	if (ans == INF) {
		cout << -1 << '\n';
	} else {
		cout << ans << '\n';
	}

	return 0;
}
