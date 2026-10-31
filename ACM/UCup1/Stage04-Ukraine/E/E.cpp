#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {

		int N; cin >> N;
		std::vector<int> A(N);
		int tot = 0;
		for (auto& a : A) {
			cin >> a;
			tot ^= a;
		}

		if (tot != 0) {
			cout << "YES" << '\n';
			cout << 2 << '\n';
			cout << 1 << ' ' << 1 << '\n';
			cout << 2 << ' ' << N << '\n';
			continue;
		} else {
			// tot == 0 so split into 2 doesn't work
			int cur = 0;
			int i = 0;
			while (i < N && cur == 0) {
				cur ^= A[i];
				i++;
			}
			int j = i;
			int cur2 = cur;
			while (j < N && (cur2 == 0 || cur2 == cur)) {
				cur2 ^= A[j];
				j++;
			}
			if (j == N) {
				cout << "NO" << '\n';
			} else {
				cout << "YES" << '\n';
				cout << 3 << '\n';
				cout << 1 << ' ' << i << '\n';
				cout << i+1 << ' ' << j << '\n';
				cout << j+1 << ' ' << N << '\n';
			}
		}
	}

	return 0;
}
