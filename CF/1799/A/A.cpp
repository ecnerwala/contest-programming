#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::vector<int> res(N, -1);
		int idx = N-1;
		std::vector<bool> seen(N+M+1);
		for (int j = 0; j < M; j++) {
			int v; cin >> v;
			if (!seen[v]) {
				seen[v] = true;
				if (idx >= 0) {
					res[idx--] = j+1;
				}
			}
		}
		for (int i = 0; i < N; i++) {
			cout << res[i] << " \n"[i+1==N];
		}
	}

	return 0;
}
