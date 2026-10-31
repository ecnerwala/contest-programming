#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> B(N-2);
		for (auto& b : B) cin >> b;
		bool good = true;
		for (int i = 0; i+2 < N-2; i++) {
			if (B[i] == 1 && B[i+1] == 0 && B[i+2] == 1) {
				good = false;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
