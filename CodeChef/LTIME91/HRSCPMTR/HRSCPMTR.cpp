#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		vector<unordered_map<int, int>> diags(N+M-1);
		vector<vector<int>> A(N, vector<int>(M));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				cin >> A[i][j];
				diags[i+(M-1-j)][A[i][j]]++;
			}
		}
		int num_bad_diags = 0;
		for (const auto& d : diags) num_bad_diags += (d.size() > 1);

		int Q; cin >> Q;
		while (Q--) {
			int x, y, v; cin >> x >> y >> v; x--, y--;
			auto& d = diags[x+(M-1-y)];
			num_bad_diags -= (d.size() > 1);

			if ((--d[A[x][y]]) == 0) {
				d.erase(A[x][y]);
			}

			A[x][y] = v;
			d[v]++;

			num_bad_diags += (d.size() > 1);

			cout << (num_bad_diags == 0 ? "Yes" : "No") << '\n';
		}
	}

	return 0;
}
