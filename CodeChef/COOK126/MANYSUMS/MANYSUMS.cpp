#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int L, R; cin >> L >> R;
		cout << ((R-L)*2+1) << '\n';
	}

	return 0;
}
