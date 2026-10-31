#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

struct evt_t {
	int64_t val;
	// -2 for edge join, -1 for dijkstra, >=0 for query
	int type;
	int st;
	int en;
	friend bool operator < (const evt_t &a, const evt_t &b) {
		return std::tie(a.val, a.type) < std::tie(b.val, b.type);
	}
	friend bool operator > (const evt_t &a, const evt_t &b) {
		return std::tie(a.val, a.type) > std::tie(b.val, b.type);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, K; cin >> N >> M >> K;
	std::vector<std::vector<std::pair<int, int64_t>>> adj(N);
	for (int e = 0; e < M; e++) {
		int a, b; cin >> a >> b; a--, b--;
		int64_t w; cin >> w;
		adj[a].push_back({b, w});
		adj[b].push_back({a, w});
	}

	int Q; cin >> Q;
	std::vector<bool> ans(Q);

	min_pq<evt_t> pq;
	for (int q = 0; q < Q; q++) {
		int x, y; int64_t t; cin >> x >> y >> t; x--, y--;
		pq.push(evt_t{t, q, x, y});
	}
	std::vector<int64_t> dist(N, -1);
	std::vector<bool> processed(N);
	for (int k = 0; k < K; k++) {
		dist[k] = 0;
		pq.push(evt_t{0, -1, -1, k});
	}

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a);
		b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	while (!pq.empty()) {
		auto evt = pq.top(); pq.pop();
		if (evt.type == -2) {
			// join these two
			merge(evt.st, evt.en);
		} else if (evt.type == -1) {
			if (processed[evt.en]) continue;
			processed[evt.en] = true;
			if (evt.st != -1) {
				merge(evt.en, evt.st);
			}

			for (auto [nxt, l] : adj[evt.en]) {
				if (processed[nxt]) {
					pq.push(evt_t{dist[evt.en] + dist[nxt] + l, -2, evt.en, nxt});
					continue;
				} else if (dist[nxt] == -1 || dist[evt.en] + l < dist[nxt]) {
					dist[nxt] = dist[evt.en] + l;
					pq.push(evt_t{dist[nxt] * 2, -1, evt.en, nxt});
				}
			}
		} else {
			assert(0 <= evt.type && evt.type < Q);
			ans[evt.type] = get_par(evt.st) == get_par(evt.en);
		}
	}

	for (int q = 0; q < Q; q++) {
		cout << (ans[q] ? "Yes" : "No") << '\n';
	}

	return 0;
}
