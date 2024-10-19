#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	// at most 9 digits
	struct mtn_t {
		int64_t v;
		int64_t p10;
		int head, tail;
	};
	std::vector<mtn_t> mtns;
	for (int d = 1; d <= 9; d++) {
		mtns.push_back({d, 10, d-1, d-1});
	}
	for (int z = 0; z < int(mtns.size()); z++) {
		if (mtns[z].p10 > int64_t(1e18) / 10 / 10) break;
		if (z > 0 && mtns[z].p10 > mtns[z-1].p10) cerr << z << ' ' << mtns[z].p10 << '\n';
		for (int a = 1; a <= mtns[z].head; a++) {
			for (int b = 1; b <= mtns[z].tail; b++) {
				mtns.push_back({
					(a * mtns[z].p10 + mtns[z].v) * 10 + b,
					10 * mtns[z].p10 * 10,
					a, b
				});
			}
		}
	}
	cerr << int(mtns.size()) << '\n';

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t A, B, M; cin >> A >> B >> M;
		cerr << "Case #" << case_num << " " << A << ' ' << B << ' ' << M << '\n';
		int ans = 0;
		for (auto v : mtns) {
			if (A <= v.v && v.v <= B && v.v % M == 0) ans++;
		}

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
