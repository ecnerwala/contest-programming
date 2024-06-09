#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		assert(N >= 3);
		std::string res(N, 'R');
		if (A[1] < A[N-1]) {
			res[0] = 'B';
			cout << "YES" << '\n';
			cout << res << '\n';
		} else if (A[0] < A[N-2]) {
			res[N-1] = 'B';
			cout << "YES" << '\n';
			cout << res << '\n';
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
