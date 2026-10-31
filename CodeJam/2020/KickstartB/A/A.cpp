#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		vector<int> H(N);
		for (int& a : H) cin >> a;
		int ans = 0;
		for (int i = 1; i+1 < N; i++) {
			ans += (H[i] > H[i-1]) && H[i] > H[i+1];
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
