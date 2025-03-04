#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; int64_t K; cin >> N >> M >> K;

	int V = N*M;
	std::vector<int> H(V);
	for (auto& h : H) cin >> h;
	std::vector<std::pair<int, int>> cells(V);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			int v = i * M + j;
			cells[v] = {H[v], v};
		}
	}
	std::sort(cells.begin(), cells.end(), std::greater<>());

	std::vector<int> par(V, -1);
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
	std::vector<bool> alive(V);
	bool good = false;
	std::vector<int> ans(V, 0);
	for (auto [h, v] : cells) {
		// connect all cells
		alive[v] = true;
		int i = v / M, j = v % M;
		if (i-1 >= 0 && alive[v-M]) merge(v-M, v);
		if (i+1 < N && alive[v+M]) merge(v+M, v);
		if (j-1 >= 0 && alive[v-1]) merge(v-1, v);
		if (j+1 < M && alive[v+1]) merge(v+1, v);

		int sz = -par[get_par(v)];
		if (K % h == 0 && sz >= K / h) {
			good = true;
			int64_t tgt = K / h;
			[&](this auto&& self, int ci, int cj) -> void {
				if (ci < 0 || ci >= N || cj < 0 || cj >= M) return;
				if (tgt == 0) return;
				int cv = ci * M + cj;
				if (!alive[cv]) return;
				alive[cv] = false;
				ans[cv] = h;
				self(ci - 1, cj);
				self(ci + 1, cj);
				self(ci, cj - 1);
				self(ci, cj + 1);
			}(i, j);
			assert(tgt == 0);
			break;
		}
	}
	if (good) {

		cout << "YES" << '\n';
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				int v = i * M + j;
				cout << ans[v] << " \n"[j+1==M];
			}
		}
	} else {
		cout << "NO" << '\n';
	}


	return 0;
}
