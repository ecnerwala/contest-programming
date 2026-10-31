#include<bits/stdc++.h>
using namespace std;

using ld = double;

const int MAXN = 60;

ld go() {
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		// cerr << "Case #" << t << '\n';
		ld ans = go();
		cout << "Case #" << t << ": ";
		cout << fixed << setprecision(10) << ans << '\n';
	}

	return 0;
}
