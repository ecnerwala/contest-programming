#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (int i = 0; i < N; i++) {
			cin >> A[i];
		}
		bool good = false;
		for (int i = 0; i < N; i++) {
			if (A[i] <= i+1) {
				good = true;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
