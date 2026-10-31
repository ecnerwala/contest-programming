#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	if ((N & 1) || (K == N)) {
		cout << 0 << '\n';
		exit(0);
	}

	int G = std::gcd(N, K);
	int NG = N / G;

	vector<bool> prod(N);
	if (NG & 1) {
		if (N & 2) {
			cout << 0 << '\n';
			exit(0);
		}

		int need = N/4;
		for (int g = 0; g < G; g++) {
			int cnt = min(need, NG/2);
			need -= cnt;
			cnt *= 2;
			for (int z = 0, k = g; z < cnt; z++, k += G) {
				prod[k] = true;
			}
		}
	} else {
		fill(prod.begin(), prod.begin()+(N/2), true);
	}

	assert(std::accumulate(prod.begin(), prod.end(), 0)*2 == N);

	vector<bool> diff(N);
	for (int i = 0; i < N; i++) {
		diff[i] = prod[i] ^ prod[(i+1)%N];
	}

	vector<bool> ans(N);
	for (int g = 0; g < G; g++) {
		ans[g] = false;
		for (int z = 0, k = g; z < NG; z++) {
			ans[(k+K)%N] = ans[k] ^ diff[k];
			k = (k+K)%N;
		}
		assert(ans[g] == false);
	}

	for (int i = 0; i < N; i++) {
		cout << (ans[i] ? -1 : 1) << " \n"[i+1==N];
	}

	return 0;
}
