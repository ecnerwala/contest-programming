#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		vector<int> invA(N);
		for (int i = 0; i < N; i++) {
			int a; cin >> a; a--;
			invA[a] = i;
		}
		vector<int> remTime(N, -1);
		for (int k = 0; k < K; k++) {
			int b; cin >> b; b--; b = invA[b];
			remTime[b] = k;
		}

		const int MOD = 998244353;
		int res = 1;
		for (int i = 0; i < N; i++) {
			if (remTime[i] == -1) continue;
			bool a = (i-1 >= 0 && remTime[i-1] < remTime[i]);
			bool b = (i+1 < N && remTime[i+1] < remTime[i]);
			res *= (a+b);
			res = (res >= MOD ? res - MOD : res);
		}
		cout << res << '\n';
	}

	return 0;
}
