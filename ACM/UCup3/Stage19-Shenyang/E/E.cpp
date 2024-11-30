#include <bits/stdc++.h>

template <typename T> using min_pq = std::priority_queue<T, std::vector<T>, std::greater<T>>;

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	int A0, A1, A2, A3; cin >> A0 >> A1 >> A2 >> A3;

	auto flip_cell = [&](int a, int i) -> int {
		int msk;
		if (i == 0) {
			msk = 0b0101010101010101;
		} else if (i == 1) {
			msk = 0b0011001100110011;
		} else if (i == 2) {
			msk = 0b0000111100001111;
		} else if (i == 3) {
			msk = 0b0000000011111111;
		} else assert(false);
		return ((a & msk) << i) | ((a >> i) & msk);
	};

	std::vector<int64_t> dist(1 << 16, -1);
	min_pq<std::pair<int64_t, int>> pq;
	auto check = [&](int a, int64_t cnd) {
		cerr << "cnd " << a << '\n';
		if (dist[a] == -1 || cnd < dist[a]) {
			pq.push({dist[a] = cnd, a});
		}
	};
	check(0, 0);
	while (!pq.empty()) {
		auto [d, v] = pq.top(); pq.pop();
		if (d != dist[v]) continue;
		cerr << v << ' ' << d << '\n';
		for (int e = 0; e < 2; e++) {
			int nv = (e << 15) | v;
			check(flip_cell(nv, 0), d + A0);
			check(flip_cell(nv, 1), d + A0);
			check(flip_cell(nv, 2), d + A0);
			check(flip_cell(nv, 3), d + A0);
			check(flip_cell(flip_cell(nv, 0), 1), d + A1);
			check(flip_cell(flip_cell(nv, 2), 3), d + A1);
			check(flip_cell(flip_cell(nv, 0), 2), d + A2);
			check(flip_cell(flip_cell(nv, 1), 3), d + A2);
			check(flip_cell(flip_cell(flip_cell(flip_cell(nv, 0), 1), 2), 3), d + A3);
		}
	}

	for (int t = 0; t < T; t++) {
		int m; cin >> m;
		int st = 0;
		for (int z = 0; z < m; z++) {
			int v = 0;
			for (int r = 0; r < 2; r++) {
				std::string s; cin >> s;
				for (int c = 0; c < 2; c++) {
					assert(s[c] == '0' || s[c] == '1');
					v |= (s[c] - '0') << (2 * r + c);
				}
			}
			st |= (1 << v);
		}
		cout << dist[st] << '\n';
	}

	return 0;
}
