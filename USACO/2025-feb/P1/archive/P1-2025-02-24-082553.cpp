#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> A(N);
	for (auto& a : A) cin >> a;

	std::vector<std::pair<int, int>> vals(N);
	for (int i = 0; i < N; i++) vals[i] = {A[i], i};
	std::sort(vals.begin(), vals.end());

	auto get_evens = [](int l1, int h1, int l2, int h2) -> int64_t {
		// first element is in (l1, h1]
		// last element is in [l2, h2)
		// last element's cut is in (l2, h2]
		// we want the same cut on both sides

		// makes sure we don't have -1 stuff
		l1 += 4, h1 += 4;
		l2 += 4, h2 += 4;

		return int64_t((h1 / 2) - (l1 / 2)) * int64_t((h2 / 2) - (l2 / 2)) + int64_t(((h1+1) / 2) - ((l1+1) / 2)) * int64_t(((h2+1) / 2) - ((l2+1) / 2));
	};

	int64_t ans = 0;
	{
		std::vector<int> nxt(N);
		std::vector<int> prv(N);
		for (int i = 0; i < N; i++) {
			nxt[i] = i+1;
			prv[i] = i-1;
		}
		for (auto [v, i] : vals) {
			int p = prv[i];
			int n = nxt[i];
			assert(n > i);

			// number of odd length interval
			int lo = (i - p);
			int hi = (n - i);
			int64_t cnt = int64_t(lo / 2) * int64_t(hi / 2) + int64_t((lo+1) / 2) * int64_t((hi+1) / 2);
			assert(cnt == get_evens(p, i, i+1, n+1));

			if (p != -1) {
				cnt += get_evens(prv[p], p, i, n);
			}
			if (n != N) {
				cnt += get_evens(p, i, n, nxt[n]);
			}

			if (p != -1) nxt[p] = n;
			if (n != N) prv[n] = p;
			prv[i] = nxt[i] = -2;
		}
	}

	for (int i = 0; i+3 <= N; i++) {
		ans -= std::max({A[i], A[i+1], A[i+2]});
		ans += std::max({A[i], A[i+2]});
	}
	for (int i = 0; i+4 <= N; i++) {
		std::array<std::pair<int, int>, 4> vs{
			std::pair<int, int>{A[i+0], 0},
			std::pair<int, int>{A[i+1], 1},
			std::pair<int, int>{A[i+2], 2},
			std::pair<int, int>{A[i+3], 3},
		};
		std::sort(vs.begin(), vs.end());
		int a = vs[2].second, b = vs[3].second;
		if (a > b) std::swap(a, b);
		if ((a == 1 && b == 2) || (a == 1 && b == 3) || (a == 0 && b == 2)) {
			ans -= vs[2].first;
			ans += vs[1].first;
		}
	}

	for (int i = 0; i+6 <= N; i++) {
		std::array<std::pair<int, int>, 6> vs{
			std::pair<int, int>{A[i+0], 0},
			std::pair<int, int>{A[i+1], 1},
			std::pair<int, int>{A[i+2], 2},
			std::pair<int, int>{A[i+3], 3},
			std::pair<int, int>{A[i+4], 4},
			std::pair<int, int>{A[i+5], 5},
		};
		std::sort(vs.begin(), vs.end());
		int a = vs[4].second, b = vs[5].second;
		if (a > b) std::swap(a, b);
		if ((a == 2 && b == 4) || (a == 1 && b == 4) || (a == 1 && b == 3)) {
			ans -= vs[4].first;
			ans += vs[3].first;
		}
	}

	cout << ans << '\n';

	return 0;
}
