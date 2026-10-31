#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		string S; cin >> S;
		int msk = 0;
		int cnt_2 = 0;
		for (char c : S) {
			int v = 1 << (c - 'a');
			if (msk & v) {
				cnt_2++;
			}
			msk ^= v;
		}
		int cnt_1 = __builtin_popcount(msk);
		cout << (cnt_1 <= cnt_2 ? "YES" : "NO") << '\n';
	}

	return 0;
}
