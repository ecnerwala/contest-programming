#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t L, R; cin >> L >> R;
		cout << (L & ((int64_t(1) << (63 - __builtin_clzll(L ^ R))) - 1)) << '\n';
	}

	return 0;
}
