#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) {
			cin >> a;
		}
		bool good = true;
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				if ((j % i == 0) != (A[j-1] % A[i-1] == 0)) {
					good = false;
				}
			}
		}
		cout << (good ? "Yes" : "No") << '\n';
	}

	return 0;
}
