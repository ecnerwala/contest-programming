#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t R; cin >> R;

		int64_t R2 = R * R;

		int64_t xa, ya, xb, yb; cin >> xa >> ya >> xb >> yb;

		int64_t r2 = xa * xa + ya * ya;
		assert(r2 == xb * xb + yb * yb);

		int64_t l2 = (xa - xb) * (xa - xb) + (ya - yb) * (ya - yb);

		double ans;
		if (R2 * (4 * r2 - l2) >= 4 * r2 * r2) {
			ans = 2 * sqrt(double(R2 + r2) - sqrt(double(R2 * (4 * r2 - l2))));
		} else {
			ans = sqrt(double(R2 * l2) / double(r2));
		}
		cout << fixed << setprecision(10) << ans << '\n';
	}

	return 0;
}
