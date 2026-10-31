#include<bits/stdc++.h>

struct dsu {
	std::vector<int> par;
	dsu() {}
	explicit dsu(int n) : par(n, -1) {}
	int get_par(int a) {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	}
	bool merge(int a, int b) {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	int L = 32 - __builtin_clz(N);
	assert((1 << (L-1)) <= N && N < (1 << L));

	vector<dsu> dsus(L, dsu(N));
	for (int e = 0; e < M; e++) {
		int a, b, l; cin >> a >> b >> l; a--, b--;
		int d = 31 - __builtin_clz(l);

		dsus[d].merge(a, b);

		if (l > (1 << d)) {
			dsus[d].merge(a + l - (1 << d), b + l - (1 << d));
		}
	}

	for (int d = L-1; d > 0; d--) {
		for (int i = 0; i + (1 << d) <= N; i++) {
			int j = dsus[d].get_par(i);
			if (j == i) continue;
			dsus[d-1].merge(i, j);
			dsus[d-1].merge(i + (1 << (d-1)), j + (1 << (d-1)));
		}
	}

	int ans = 0;
	for (int i = 0; i < N; i++) {
		ans += dsus[0].get_par(i) == i;
	}
	cout << ans << '\n';

	return 0;
}
