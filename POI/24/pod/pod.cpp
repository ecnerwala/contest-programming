#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int B, Q; cin >> B >> Q;
	vector<int64_t> A(B);
	for (auto& a : A) cin >> a;
	int tot = 0;
	for (int i = B-1, s = 0; i >= 1; i--) {
		s += int(A[i]);
		s %= B-1;
		tot += s; tot = (tot >= B-1 ? tot - (B-1) : tot);
	}

	if (tot) {
		A[tot]--;
		if (B == 2 && !A[tot]) A[0] = 1;
	}
	for (int i = 0; i+1 < B; i++) {
		A[i+1] += A[i];
	}
	while (Q--) {
		int64_t K; cin >> K;
		int d = int(upper_bound(A.begin(), A.end(), K) - A.begin());
		if (d == B) cout << -1 << '\n';
		else cout << d << '\n';
	}

	return 0;
}
