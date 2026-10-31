#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		// we want to maximize the # of correct tests
		std::vector<int> cnd(N, -1);
		for (int e = 0; e < M; e++) {
			int x, k; cin >> x >> k; x--, k--;
			assert(cnd[x] == -1);
			cnd[x] = k;
		}
		std::vector<int> check_time(N-1);
	}

	return 0;
}
