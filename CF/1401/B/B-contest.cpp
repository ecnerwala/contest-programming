#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		array<int, 3> A;
		for (int& c : A) cin >> c;

		array<int, 3> B;
		for (int& c : B) cin >> c;

		int ans = 0;

		int v = min(A[2], B[1]);
		A[2] -= v, B[1] -= v;
		ans += 2 * v;

		v = min(A[1], B[0] + B[1]);
		A[1] -= v;
		ans -= 2 * A[1];

		cout << ans << '\n';
	}
	return 0;
}

// 21 is +2, 12 is -2
