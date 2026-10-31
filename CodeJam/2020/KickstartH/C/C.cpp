#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N; cin >> N;
		vector<int64_t> X(N);
		vector<int64_t> Y(N);
		for (int i = 0; i < N; i++) {
			cin >> X[i] >> Y[i];
		}
		sort(X.begin(), X.end());
		for (int i = 0; i < N; i++) X[i] -= i;
		sort(X.begin(), X.end());
		sort(Y.begin(), Y.end());
		int64_t mx = X[N/2];
		int64_t my = Y[N/2];
		int64_t ans = 0;
		for (auto& x : X) ans += abs(x - mx);
		for (auto& y : Y) ans += abs(y - my);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
