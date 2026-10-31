#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;

	std::vector<std::vector<int>> smallers(N);
	std::vector<std::unordered_set<int>> biggers(N);
	for (int e = 0; e < M; e++) {
		int x, y; cin >> x >> y; x--, y--;
		if (x > y) std::swap(x, y);
		smallers[y].push_back(x);
		biggers[x].insert(y);
	}

	std::vector<int> par(N, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};

	int64_t ans = 0;
	for (int i = 0; i < N; i++) {
		for (int j : smallers[i]) {
			j = get_par(j);
			if (j == i) continue;
			par[i] += par[j];
			par[j] = i;

			biggers[j].erase(i);

			if (biggers[j].size() > biggers[i].size()) {
				std::swap(biggers[i], biggers[j]);
			}
			for (int v : biggers[j]) {
				biggers[i].insert(v);
			}
		}
		ans += int(biggers[i].size());
	}

	cout << ans - M << '\n';

	return 0;
}
