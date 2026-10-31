#include<bits/stdc++.h>

[[noreturn]] void FAIL() {
	std::cout << "detention" << '\n';
	exit(0);
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, P, Q; cin >> N >> P >> Q;
	vector<vector<bool>> adj(N, vector<bool>(N, false));
	vector<vector<int>> nei(N);
	for (int i = 0; i < N; i++) {
		int m; cin >> m;
		if (m >= P-1+Q) FAIL();
		nei[i].reserve(m);
		while (m--) {
			int j; cin >> j;
			assert(i != j);
			assert(!adj[i][j]);
			nei[i].push_back(j);
			adj[i][j] = true;
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (adj[i][j] != adj[j][i]) {
				FAIL();
			}
		}
	}

	return 0;
}
