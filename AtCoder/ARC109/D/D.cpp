#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		auto get_center = [](array<array<int64_t, 2>, 3> pts) {
			array<int64_t, 2> tot{0,0};
			array<int64_t, 2> other{0,0};
			for (const auto& p : pts) {
				for (int z : {0,1}) {
					tot[z] += p[z];
					other[z] ^= p[z];
				}
			}
			array<int64_t, 2> pt;
			for (int z : {0,1}) {
				pt[z] = (tot[z] * 3 - other[z] - 2) / 4;
			}
			return pt;
		};

		auto source = get_center(array<array<int64_t, 2>, 3>{array<int64_t, 2>{0,0},{0,1},{1,0}});

		array<array<int64_t, 2>, 3> pts;
		for (auto& p : pts) {
			cin >> p[0] >> p[1];
		}
		auto dest = get_center(pts);

		int64_t d0 = abs(dest[0] - source[0]);
		int64_t d1 = abs(dest[1] - source[1]);

		int64_t ans = max(d0, d1);
		assert(source[0] == source[1]);
		if (dest[0] == dest[1] && dest != source && dest[0] != (source[0] ^ 1)) {
			ans++;
		}

		cout << ans << '\n';

		// a * tot/3 + (1-a) * other = a * 1/3 + (1-a) * 1 = 1/4
		// 3 * tot / 8 - 1/8 * other
		//
		// (3 * x0 + 1) and (x0 + 1) gives 4 * x0 + 1
		// 3/2 * (3 * x0 + 1) - 1/2 (x0 + 1)
	}

	return 0;
}
