#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);


	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		int hi = 0;
		int hi2 = 0;
		int lo = int(1e9);
		while (N--) {
			int a; cin >> a;

			if (a > hi) hi2 = hi, hi = a;
			else if (a > hi2) hi2 = a;

			if (a < lo) lo = a;
		}
		cout << int64_t(hi2) * int64_t(hi - lo) << '\n';
	}

	return 0;
}
