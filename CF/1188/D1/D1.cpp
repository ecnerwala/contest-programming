#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e5;
int N;
int deg[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N;

	for (int i = 0; i < N-1; i++) {
		int u, v; cin >> u >> v; u--, v--;
		deg[u] ++, deg[v] ++;
	}
	for (int i = 0; i < N; i++) {
		if (deg[i] == 2) {
			cout << "NO\n"; exit(0);
		}
	}
	cout << "YES\n"; exit(0);

	return 0;
}
