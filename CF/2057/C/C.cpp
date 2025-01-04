#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int L, R; cin >> L >> R;
		// we want the 3 values to be 2 same 1 different
		int b = 31 - __builtin_clz(L ^ R);
		int vr = (R >> b << b);
		int vl = vr - 1;
		assert(L <= vl && vr <= R);
		if (L <= vl - 1) {
			cout << vl-1 << ' ' << vl << ' ' << vr << '\n';
		} else if (vr + 1 <= R) {
			cout << vl << ' ' << vr << ' ' << vr+1 << '\n';
		} else assert(false);
	}

	return 0;
}
