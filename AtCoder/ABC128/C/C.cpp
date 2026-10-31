#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);

	int N, M; cin >> N >> M;
	vector<int> switches(M);
	vector<int> p(M);
	for (int i = 0; i < M; i++) {
		int k; cin >> k; while (k--) {
			int v; cin >> v; v--;
			switches[i] |= (1<<v);
		}
	}
	for (int i = 0; i < M; i++) cin >> p[i];
	int ans = 0;
	for (int m = 0; m < (1<<N); m++) {
		bool good = true;
		for (int i = 0; i < M; i++) {
			good = good && __builtin_popcount(switches[i] & m) % 2 == p[i];
		}
		if (good) ans++;
	}
	cout << ans << '\n';
	return 0;
}
