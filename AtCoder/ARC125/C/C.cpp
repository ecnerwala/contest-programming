#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, K; cin >> N >> K;
	vector<int> A(K); for (auto& a : A) { cin >> a; a--; }

	vector<bool> used(N, false);

	vector<int> ans; ans.reserve(N);
	int lo = 0;
	for (int i = 0; i < K-1; i++) {
		used[A[i]] = true;
		ans.push_back(A[i]);
		while (lo < A[i] && used[lo]) lo++;
		if (lo < A[i]) {
			used[lo] = true;
			ans.push_back(lo);
		}
	}
	for (int i = N-1; i >= 0; i--) {
		if (!used[i]) ans.push_back(i);
	}

	for (int i = 0; i < N; i++) {
		cout << ans[i]+1 << " \n"[i+1==N];
	}

	return 0;
}
