#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int N, W, D; std::cin >> N >> W >> D;

	std::vector<int> F(N);
	for (auto& f : F) std::cin >> f;

	int W_loops = (W-1) / D;
	int64_t ans = int64_t(W_loops) * int64_t(D) * int64_t(N - 1);
	W -= D * W_loops;

	std::vector<int> vals; vals.reserve(2*N+2);
	vals.push_back(0);
	for (auto f : F) {
		vals.push_back(f);
		vals.push_back((f + W) % D);
	}
	std::sort(vals.begin(), vals.end());
	vals.resize(std::unique(vals.begin(), vals.end()) - vals.begin());
	int V = int(vals.size());
	vals.push_back(D);

	int ccs = V+1;
	std::vector<int> par(V+1, -1);
	auto get_par = [&](int a) -> int {
		while (par[a] >= 0) {
			if (par[par[a]] >= 0) par[a] = par[par[a]];
			a = par[a];
		}
		return a;
	};
	auto merge = [&](int a, int b) -> bool {
		a = get_par(a), b = get_par(b);
		if (a == b) return false;
		if (par[a] > par[b]) std::swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};
	ccs -= merge(0, V);

	std::vector<int> pref(V+1);
	for (auto f : F) {
		int st = int(std::lower_bound(vals.begin(), vals.end(), f) - vals.begin());
		int en = int(std::lower_bound(vals.begin(), vals.end(), (f + W) % D) - vals.begin());
		pref[en]--;
		pref[st]++;
		if (en <= st) {
			pref[0]++;
			pref[V]--;
		}
		ccs -= merge(st, en);
	}
	for (int v = 0; v < V; v++) { pref[v+1] += pref[v]; }
	assert(pref[V] == 0);
	int mx = *std::max_element(pref.begin(), pref.end());
	assert(mx >= 1);
	// find the last mx
	std::vector<int> need(V, mx);
	int last = V-1;
	while (pref[last] != mx) last--;
	for (int v = last+1; v < V; v++) need[v] = mx-1;

	for (int v = 0; v < V; v++) {
		assert(need[v] >= pref[v]);
		if (need[v] > pref[v]) {
			ccs -= merge(v, v+1);
		}
	}

	for (int v = (last+1) % V; ccs > 1; v++, v %= V) {
		need[v]++;
		assert(need[v] > pref[v]);
		ccs -= merge(v, v+1);
	}
	for (int v = 0; v < V; v++) {
		ans += int64_t(need[v]) * int64_t(vals[v+1] - vals[v]);
	}
	ans -= W;
	std::cout << ans << '\n';

	return 0;
}
