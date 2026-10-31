#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t D; cin >> D;
		if (D % 9 != 0) {
			cout << "NONE" << '\n';
		} else {
			std::vector<int> digs;
			for (int64_t v = D / 9; v; v /= 10) { digs.push_back(v % 10); }
			std::reverse(digs.begin(), digs.end());
			digs.push_back(0);
			int64_t a = 0, b = 0;
			int64_t cur = 0;
			for (auto d : digs) {
				a *= 10;
				b *= 10;
				if (d > cur) {
					a += (d - cur);
				} else if (d < cur) {
					b += (cur - d);
				} else {
					// do nothing
				}
				cur = d;
			}

			assert(a - b == D);

			cout << a << ' ' << b << '\n';
		}
	}

	return 0;
}
