#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		bool good = false;
		for (int i = 0; i+1 < N; i++) {
			int x = A[i], y = A[i+1];
			if (x + x > y && y + y > x) {
				good = true;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
