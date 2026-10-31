#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	vector<string> S(N); for (auto& s : S) cin >> s;

	for (int i = 0; i < N; i++) {
		S[i][i] = '1';
	}

	for (int k = 0; k < N; k++) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				if (S[i][k] == '1' && S[k][j] == '1') S[i][j] = '1';
			}
		}
	}

	double ans = 0;
	for (int i = 0; i < N; i++) {
		int cnt = 0;
		for (int j = 0; j < N; j++) {
			if (S[j][i] == '1') cnt++;
		}
		ans += 1./cnt;
	}
	cout << fixed << setprecision(20) << ans << '\n';

	return 0;
}
