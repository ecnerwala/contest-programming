#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) {
			cin >> a;
		}
		bool found = false;
		for (int i = 0; i+1 < N; i++) {
			if (A[i] > A[i+1]) {
				cout << "YES" << '\n';
				cout << 2 << '\n';
				cout << A[i] << ' ' << A[i+1] << '\n';
				found = true;
				break;
			}
		}
		if (!found) {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
