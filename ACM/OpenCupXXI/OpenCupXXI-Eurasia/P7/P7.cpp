#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int ans = 0;
		for (int i = 0; i < N; i++) {
			bool g = false;
			for (int j = 0; j < i; j++) {
				int v; cin >> v;
				if (v) g = true;
			}
			ans += g;
			for (int j = i; j < N; j++) {
				int v; cin >> v;
			}
		}
		cout << ans << '\n';
	}

	return 0;
}
