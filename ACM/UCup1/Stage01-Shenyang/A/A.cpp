#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	auto build_intervals = [&](int L) -> std::vector<std::pair<int, std::array<double, 2>>> {
		std::vector<std::array<int, 2>> A(L);
		int64_t tot = 0;
		for (auto& a : A) {
			cin >> a[0] >> a[1];
			assert(a[0] <= a[1]);
			tot += int64_t(a[1]) - a[0];
		}
		std::sort(A.begin(), A.end());
		for (int i = 0; i+1 < L; i++) {
			assert(A[i][1] < A[i+1][0]);
		}
		std::vector<std::pair<int, std::array<double, 2>>> res; res.reserve(2*L);
		if (tot == 0) {
			int cnt = 0;
			for (auto a : A) {
				assert(a[0] == a[1]);
				cnt++;
				res.push_back({a[0], std::array<double, 2>{double(cnt)/L, 0.}});
			}
		} else {
			int64_t cur_len = 0;
			for (auto a : A) {
				if (a[0] == a[1]) continue;
				assert(a[0] < a[1]);
				res.push_back({a[0], std::array<double, 2>{double(cur_len - a[0]) / tot, double(1) / tot}});
				cur_len += int64_t(a[1]) - a[0];
				res.push_back({a[1], std::array<double, 2>{double(cur_len) / tot, 0.}});
			}
		}
		return res;
	};

	int NA, NB; cin >> NA >> NB;
	auto VA = build_intervals(NA);
	auto VB = build_intervals(NB);

	std::array<double, 2> curA{0., 0.};
	std::array<double, 2> curB{0., 0.};

	int prvX = -int(1e9);

	double ans = 0;
	for (int iA = 0, iB = 0; iA < int(VA.size()) || iB < int(VB.size()); ) {
		bool useA = iA < int(VA.size()) && (iB == int(VB.size()) || VA[iA].first < VB[iB].first);

		int nxtX = useA ? VA[iA].first : VB[iB].first;

		// integrate from prvX to nxtX
		if (prvX < nxtX) {
			std::array<double, 3> poly{
				curA[0] + curB[0] - 2 * (curA[0] * curB[0]),
				curB[1] + curA[1] - 2 * (curA[0] * curB[1] + curA[1] * curB[0]),
				-2 * curA[1] * curB[1]
			};
			ans += poly[0] * (nxtX - prvX);
			ans += poly[1] * (double(nxtX) * double(nxtX) / 2 - double(prvX) * double(prvX) / 2);
			ans += poly[2] * (double(nxtX) * double(nxtX) * double(nxtX) / 3 - double(prvX) * double(prvX) * double(prvX) / 3);
		}

		prvX = nxtX;

		if (useA) {
			curA = VA[iA++].second;
		} else {
			curB = VB[iB++].second;
		}
	}
	cout << fixed << setprecision(12) << ans << '\n';

	return 0;
}
