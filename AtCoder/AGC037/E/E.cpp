#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int N, K; cin >> N >> K;
	string S; cin >> S;
	string ans = S;

	string T(S.rbegin(), S.rend());
	string U = S + T;
	char a = *min_element(S.begin(), S.end());

	if (K == 1) {
		for (int i = 0; i <= N; i++) {
			ans = min(ans, string(U.begin() + i, U.begin() + i + N));
		}
		cout << ans << '\n';
		return 0;
	} else {
		for (int i = 0; i <= N; i++) {
			string cur = string(U.begin() + i, U.begin() + i + N);
			int cnt = 0;
			while (!cur.empty() && cur.back() == a) {
				cur.pop_back();
				cnt++;
			}
			if (cnt == 0) continue;
			for (int z = 0; cnt < N && z < K-1; z++) {
				cnt = min(cnt * 2, N);
			}
			string cnd(cnt, a);
			cnd += string(cur.rbegin(), cur.rend());
			ans = min(ans, string(cnd.begin(), cnd.begin() + N));
		}
		cout << ans << '\n';
	}

	return 0;
}
