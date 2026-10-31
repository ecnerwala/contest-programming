#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::sort(A.begin(), A.end());
		int v = A[0];
		for (auto a : A) {
			v += a;
			v >>= 1;
		}
		cout << v << '\n';
	}

	return 0;
}
