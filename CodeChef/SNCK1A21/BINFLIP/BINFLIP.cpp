#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int K; cin >> K >> K;
		if (K == 0) {
			cout << "YES" << '\n';
			cout << 0 << '\n';
		} else if (K & 1) {
			cout << "YES" << '\n';
			int l = 0;
			while ((1 << l) <= K) l++;
			cout << l << '\n';
			K += (((1 << l) - 1 - K) >> 1);
			std::vector<int> res(l);
			int cur = 0;
			res[l-1] = cur;
			cur += (1 << (l-1));
			for (int i = 0; i < l-1; i++) {
				if (!(K & (1 << i))) {
					cur -= (1 << i);
					res[i] = cur;
				}
			}
			for (int i = 0; i < l-1; i++) {
				if (K & (1 << i)) {
					res[i] = cur;
					cur += (1 << i);
				}
			}
			for (int i = 0; i < l; i++) {
				cout << res[i]+1 << '\n';
			}
		} else {
			cout << "NO" << '\n';
		}
	}

	return 0;
}
