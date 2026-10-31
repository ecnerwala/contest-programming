#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		array<int, 10> A;
		for (auto& a : A) cin >> a;
		int K; cin >> K;
		int res = 9;
		while (true) {
			if (A[res] > K) {
				break;
			} else {
				K -= A[res];
				res--;
			}
		}
		cout << res+1 << '\n';
	}

	return 0;
}
