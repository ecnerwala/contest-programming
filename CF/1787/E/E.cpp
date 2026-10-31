#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K, X; cin >> N >> K >> X;
		int tot_val = 0;
		for (int i = 1; i <= N; i++) tot_val ^= i;
		if (tot_val != ((K & 1) ? X : 0)) {
			cout << "NO" << '\n';
			continue;
		}
		assert(X != 0);
		int num_good = 0;
		for (int i = 1; i <= N; i++) {
			if ((i ^ X) < i) {
				num_good++;
			}
		}
		if (K > num_good) {
			cout << "NO" << '\n';
			continue;
		}
		cout << "YES" << '\n';
		std::vector<bool> used(N+1);
		int num_rem = N;
		for (int i = 1; K > 1 && i <= N; i++) {
			if ((i ^ X) < i) {
				K--;
				if ((i^X) == 0) {
					cout << 1 << ' ' << i << '\n';
					used[i] = true;
					num_rem --;
				} else {
					cout << 2 << ' ' << (i ^ X) << ' ' << i << '\n';
					used[i^X] = used[i] = true;
					num_rem -= 2;
				}
			}
		}
		cout << num_rem;
		for (int i = 1; i <= N; i++) {
			if (!used[i]) {
				cout << ' ' << i;
			}
		}
		cout << '\n';
	}

	return 0;
}
