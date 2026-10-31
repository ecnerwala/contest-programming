#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<int> F(N); for (auto& f : F) { cin >> f; f--; }

	vector<int> par(N, -1);
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
		if (par[a] > par[b]) swap(a, b);
		par[a] += par[b];
		par[b] = a;
		return true;
	};

	int num_CC = N;
	for (int i = 0; i < N; i++) {
		num_CC -= merge(i, F[i]);
	}

	const int MOD = 998244353;
	int ans = 1;
	for (int i = 0; i < num_CC; i++) {
		ans = ans * 2 % MOD;
	}
	ans--;
	cout << ans << '\n';

	return 0;
}

// Permutation iff composed of cycles
// There's 1 cycle per connected component of the functional graph
// We just need 2^(# ccs) - 1
