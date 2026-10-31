#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int L, R; cin >> L >> R;
		for (int i = L+3; i <= R; i++) {
			for (int j = L+2; j < i; j++) {
				for (int k = L+1; k < j; k++) {
					for (int l = L; l < k; l++) {
						if (!(i ^ j ^ k ^ l)) {
							cout << l << ' ' << k << ' ' << j << ' ' << i << '\n';
							goto done;
						}
					}
				}
			}
		}
		cout << -1 << '\n';
done:;
	}

	return 0;
}
