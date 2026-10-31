#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		std::array<int, 4> v;
		for (auto& a : v) cin >> a;
		for (auto& a : v) { int b; cin >> b; if (b < a) a = b; }
		for (auto& a : v) { int b; cin >> b; if (b < a) a = b; }

		cout << "Case #" << case_num << ": ";
		if (std::accumulate(v.begin(), v.end(), 0) < int(1e6)) {
			cout << "IMPOSSIBLE" << '\n';
		} else {
			int rem = int(1e6);
			for (int z = 0; z < 4; z++) {
				int a = v[z];
				a = std::min(a, rem);
				cout << a << " \n"[z+1==4];
				rem -= a;
			}
			assert(rem == 0);
		}
	}

	return 0;
}
