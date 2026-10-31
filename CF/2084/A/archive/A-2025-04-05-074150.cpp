#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {

		int N; cin >> N;
		assert(N >= 2);
		int i;
		for (i = 1; i <= N; i++) {
			if ((N+1) % i == 0 && (N+1) % (i+1) == 0) {
				// N can go in position i
				break;
			}
		}
		if (i == N+1) {
			cout << -1 << '\n';
		} else {
			for (int j = 1; j <= N; j++) {
				int v;
				if (j == i) v = N;
				else if (j < i) v = j;
				else if (j > i) v = j-1;
				else assert(false);
				cout << v << " \n"[j==N];
			}
		}
	}

	return 0;
}
