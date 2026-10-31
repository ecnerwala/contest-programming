#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;

	struct ticket_t {
		int a, b;
		int c;
		int64_t p;
	};
	std::vector<ticket_t> tickets(K);
	for (auto& t : tickets) {
		cin >> t.c >> t.p >> t.a >> t.b; t.c--, t.a--;
	}

	std::vector<std::vector<int>> seg_tickets(2*N);
	for (int k = 0; k < K; k++) {
		for (int a = N + tickets[k].a, b = N + tickets[k].b; a < b; a >>= 1, b >>= 1) {
			if (a & 1) seg_tickets[a++].push_back(k);
			if (b & 1) seg_tickets[--b].push_back(k);
		}
	}

	auto do_dijk = [&](std::vector<int64_t>& dist) -> std::vector<int64_t> {
		min_pq<std::pair<int64_t, int>> pq;

		for (int i = 0; i < N; i++) {
			if (dist[i] != -1) {
				pq.push({dist[i], i});
			}
		}

		std::vector<bool> vis(2*N);

		std::vector<int64_t> ticket_dist(K, -1);

		while (!pq.empty()) {
			auto [d, cur] = pq.top(); pq.pop();
			assert(dist[cur] != -1 && d >= dist[cur]);
			if (d != dist[cur]) continue;
			for (int a = cur+N; a > 0 && !vis[a]; a >>= 1) {
				vis[a] = true;
				for (int k : seg_tickets[a]) {
					assert(tickets[k].a <= cur && cur < tickets[k].b);
					if (ticket_dist[k] != -1) continue;
					ticket_dist[k] = d;
					int c = tickets[k].c;
					int64_t nd = d + tickets[k].p;
					if (dist[c] == -1 || nd < dist[c]) {
						pq.push({dist[c] = nd, c});
					}
				}
			}
		}

		return ticket_dist;
	};

	std::vector<int64_t> ticket_dist(K, 0);
	std::vector<int64_t> base_dist(N, 0);
	for (int z = 0; z < 2; z++) {
		std::vector<int64_t> dist(N, -1);
		dist[z ? N-1 : 0] = 0;
		auto resp = do_dijk(dist);
		for (int k = 0; k < K; k++) {
			if (ticket_dist[k] == -1 || resp[k] == -1) {
				ticket_dist[k] = -1;
			} else {
				ticket_dist[k] += resp[k];
			}
		}
		for (int i = 0; i < N; i++) {
			if (dist[i] == -1 || base_dist[i] == -1) {
				base_dist[i] = -1;
			} else {
				base_dist[i] += dist[i];
			}
		}
	}

	for (int k = 0; k < K; k++) {
		if (ticket_dist[k] == -1) continue;
		int c = tickets[k].c;
		int64_t d = ticket_dist[k] + tickets[k].p;
		if (base_dist[c] == -1 || d < base_dist[c]) {
			base_dist[c] = d;
		}
	}
	do_dijk(base_dist);
	for (int i = 0; i < N; i++) {
		cout << base_dist[i] << '\n';
	}

	return 0;
}
