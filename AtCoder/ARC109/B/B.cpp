#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int64_t N; cin >> N;

	int64_t mi = 1;
	int64_t ma = 2e9;
	while (ma - mi > 1) {
		int64_t md = (mi + ma) / 2;
		if (md * (md+1) / 2 <= N+1) {
			mi = md;
		} else {
			ma = md;
		}
	}

	cout << N - (mi - 1) << '\n';

	return 0;
}
