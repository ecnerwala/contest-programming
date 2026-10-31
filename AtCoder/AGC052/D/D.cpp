#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<int> A(N); for (auto& a : A) cin >> a;
		vector<int> vertex_lis(N, 1);
		for (int z = 0; z < 2; z++) {
			vector<int> stk; stk.reserve(N);
			for (int i = 0; i < N; i++) {
				int cur = z ? -A[N-1-i] : A[i];
				int mi = -1;
				int ma = int(stk.size());
				while (ma - mi > 1) {
					int md = (mi + ma) / 2;
					if (cur < stk[md]) ma = md;
					else mi = md;
				}
				vertex_lis[z ? N-1-i : i] += ma;

				if (ma == int(stk.size())) stk.push_back(cur);
				else stk[ma] = cur;
			}
		}

		int V = *max_element(vertex_lis.begin(), vertex_lis.end());
		V = (V+1) / 2;
		int cnt = 0;
		for (int i = 0; i < N; i++) {
			cnt += vertex_lis[i] >= V;
		}
		cout << (cnt >= 2 * V ? "YES" : "NO") << '\n';
	}

	return 0;
}
