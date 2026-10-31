#include<bits/stdc++.h>
using namespace std;

int main() {
	ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
	int T; cin >> T;

	vector<long double> logFact(int(3.1e5));
	for (int i = 1; i < int(logFact.size()); i++) {
		logFact[i] = logFact[i-1] + log((long double)(i));
	}

	for (int case_num = 1; case_num <= T; case_num ++) {
		int X, Y; cin >> X >> Y;
		int Xlo, Xhi, Ylo, Yhi;
		cin >> Xlo >> Ylo >> Xhi >> Yhi;

		auto probPath = [&](int r, int c) -> long double {
			return exp(logFact[r+c] - logFact[r] - logFact[c] - (r+c) * log(2.L));
		};
		// we'll just casework on the first entry
		vector<long double> sumAnds;
		if (Ylo > 1) {
			for (int x = Xlo; x <= Xhi; x++) {
				sumAnds.push_back(0.5 * probPath(x - 1, Ylo - 2));
			}
			if (Xhi == X) {
				for (int y = 1; y < Ylo; y++) {
					sumAnds.push_back(0.5 * probPath(X - 1, y - 1));
				}
			}
		}
		if (Xlo > 1) {
			for (int y = Ylo; y <= Yhi; y++) {
				sumAnds.push_back(0.5 * probPath(Xlo - 2, y - 1));
			}
			if (Yhi == Y) {
				for (int x = 1; x < Xlo; x++) {
					sumAnds.push_back(0.5 * probPath(x - 1, Y - 1));
				}
			}
		}
		sort(sumAnds.begin(), sumAnds.end());
		long double ans = 0;
		for (auto a : sumAnds) ans += a;
		ans = 1 - ans;

		cout << fixed << setprecision(20) << '\n';
		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
