#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;
template <typename T> using max_pq = std::priority_queue<T, std::vector<T>, std::less<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M, P; cin >> N >> M >> P; P--;
	std::vector<int64_t> D(N);
	for (auto& d : D) cin >> d;

	std::vector<int> in_deg(N);
	std::vector<std::vector<int>> out_edges(N);
	for (int e = 0; e < M; e++) {
		int a, b; cin >> a >> b; a--, b--;
		if (a == P) {
			// don't care about these
			continue;
		}
		in_deg[a]++;
		out_edges[b].push_back(a);
	}

	min_pq<std::pair<int64_t, int>> his;
	max_pq<std::pair<int64_t, int>> los;

	auto add_vert = [&](int v) {
		if (D[v] >= D[P]) {
			his.push({D[v], v});
		} else {
			los.push({D[v], v});
		}
	};

	int64_t cur_lo = D[P], cur_hi = D[P];
	for (int i = 0; i < N; i++) {
		if (in_deg[i] == 0) {
			add_vert(i);
		}
	}

	int64_t ans = 0;
	for (int z = 0; z < N; z++) {
		assert(!his.empty() || !los.empty());
		int cur;
		if (!his.empty() && (los.empty() || his.top().first - cur_hi <= cur_lo - los.top().first)) {
			ans = std::max(ans, his.top().first - cur_hi);
			cur_hi = std::max(cur_hi, his.top().first);
			cur = his.top().second;
			his.pop();
		} else {
			ans = std::max(ans, cur_lo - los.top().first);
			cur_lo = std::min(cur_lo, los.top().first);
			cur = los.top().second;
			los.pop();
		}

		for (int nxt : out_edges[cur]) {
			if (!(--in_deg[nxt])) {
				add_vert(nxt);
			}
		}
	}
	cout << ans << '\n';

	return 0;
}
