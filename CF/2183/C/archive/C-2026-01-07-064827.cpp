#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M, K; cin >> N >> M >> K;
		M++;
		// we get M+1 soldiers
		int L = K-1;
		int R = N-K;
		if (L > R) std::swap(L, R);
		assert(L <= R);
		if (L + R + R <= M) {
			cout << L+R+1 << '\n';
		} else if (3 * L <= M) {
			cout << L + (M-L)/2 + 1 << '\n';
		} else {
			cout << (2*M/3) + 1 << '\n';
		}
	}

	return 0;
}
