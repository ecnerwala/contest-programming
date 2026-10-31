#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		for (int i = 0; i < N; i++) {
			int p; cin >> p;
			cout << (N+1-p) << " \n"[i+1==N];
		}
	}

	return 0;
}
