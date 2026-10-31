#include<bits/stdc++.h>
using namespace std;

using i64 = int64_t;

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		i64 x[5], y[5];
		for (int i = 0; i < 4; i++) {
			cin >> x[i] >> y[i];
		}
		x[4]=x[0]; y[4]=y[0];
		i64 ans = 0;
		for (int i = 0; i < 4; i++) {
			ans += x[i] * y[i+1] - x[i+1] * y[i];
		}
		ans = abs(ans);
		i64 d; cin >> d; d *= 2;
		cout << "Case #" << t << ": " << fixed << setprecision(2) << double(d) / ans << '\n';
	}

	return 0;
}
