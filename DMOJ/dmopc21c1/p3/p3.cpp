#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; cin >> N >> M;
	std::vector<bool> ans(M);

	for (int i = 0; i < M; i++) {
		int u, v; cin >> u >> v; u--, v--;
		assert(u != v);
	}

	for (int z = 0; (1 << z) < N; z++) {
		for (int a = 0; a + (1 << z) < N; a += (2 << z)) {
			int m = a + (1 << z);
			int b = min(N, m + (1 << z));
			cout << '?' << ' ' << m-a << ' ' << b-m << '\n';
			for (int i = a; i < m; i++) {
				cout << i+1 << " \n"[i+1==m];
			}
			for (int i = m; i < b; i++) {
				cout << i+1 << " \n"[i+1==b];
			}
			cout << flush;
			int R; cin >> R;
			while (R--) {
				int e; cin >> e; e--;
				ans[e] = true;
			}
		}
	}
	cout << '!' << ' ';
	for (int i = 0; i < M; i++) {
		cout << int(ans[i]);
	}
	cout << '\n';

	return 0;
}
