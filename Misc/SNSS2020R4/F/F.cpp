#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		struct dat { int v, i; };
		vector<dat> A(N);
		for (int i = 0; i < N; i++) {
			cin >> A[i].v; A[i].i = i+1;
		}

		sort(A.begin(), A.end(), [](const dat& a, const dat& b) { return a.v > b.v; });

		for (int k = 2; k <= N; k++) {
			int z = 0;
			while (A[z].i % k == 0) z++;
			cout << A[z].v << " \n"[k==N];
		}
	}

	return 0;
}
