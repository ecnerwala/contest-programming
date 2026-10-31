#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		vector<array<int, 3>> A(N);
		for (auto& a : A) cin >> a[0] >> a[1] >> a[2];

		cout << ([&]() -> bool {
			for (int i = N-1; i >= 0; i--) {
			}
			return true;
		}() ? "Yes" : "No") << '\n';
	}

	return 0;
}
