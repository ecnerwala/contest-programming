#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<int> P(N);
	int cur = 0;
	for (int i = 0; i < N; ) {
		cur++;
		if (A[i] != cur) {
			cout << -1 << '\n';
			exit(0);
		}
		int st = i;
		while (i < N && A[i] == cur) i++;
		int en = i;
		for (int j = st; j < en; j++) {
			P[j] = st + en - j;
		}
	}
	for (int i = 0; i < N; i++) {
		cout << P[i] << " \n"[i+1==N];
	}

	return 0;
}
