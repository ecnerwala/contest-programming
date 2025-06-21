#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t X; int K, L; cin >> X >> K >> L;
	std::vector<int64_t> A(K); for (auto& a : A) cin >> a;
	assert(std::is_sorted(A.begin(), A.end()));
	std::vector<int64_t> B(L); for (auto& b : B) cin >> b;
	assert(std::is_sorted(B.begin(), B.end()));

	int cnt = 0;
	for (int i = K-1, j = 0; i >= 0 && j < L; i--) {
		if (A[i] + B[j] < X) {
			cnt++;
			while (j < L && A[i] + B[j] < X) j++;
		}
	}
	if (cnt == 0) {
		cout << 0 << '\n';
	} else {
		int ans = 1;
		while (cnt > (int64_t(1) << (ans-1))) ans++;
		cout << ans << '\n';
	}

	return 0;
}
