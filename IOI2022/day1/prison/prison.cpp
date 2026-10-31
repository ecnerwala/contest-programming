#include "prison.h"

#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

std::vector<std::vector<int>> devise_strategy(int N) {
	const std::vector<int> bases({3, 3, 3, 3, 3, 3, 2});
	// Editorial lists this array, which is better
	// const std::vector<int> bases({3, 3, 3, 3, 3, 2, 2, 1});
	const int B = int(bases.size());

	std::vector<int> st(B + 1);
	st[0] = 1;
	for (int i = 0; i < B; i++) {
		st[i+1] = st[i] + bases[i];
	}

	std::vector<int> divs(B + 1);
	divs[B] = 2;
	for (int i = B - 1; i >= 0; i--) {
		divs[i] = divs[i+1] * bases[i] + 2;
	}
	assert(divs[0] >= N);

	std::vector<std::vector<int>> res(st.back(), std::vector<int>(divs[0] + 1, -3));

	// inclusive range (for once!)
	std::y_combinator([&](auto self, int b, int i, int l, int r) {
		assert(r - l + 1 == divs[b]);

		// if b is 0 mod 2, we're querying bag B
		res[i][l] = (b & 1) ? -1 : -2;
		res[i][r] = (b & 1) ? -2 : -1;

		if (b == B) return;
		for (int d = 0; d < bases[b]; d++) {
			int ni = st[b] + d;
			int nl = l + 1 + d * divs[b+1];
			int nr = l + 1 + (d + 1) * divs[b+1] - 1;
			for (int x = nl; x <= nr; x++) {
				res[i][x] = ni;
			}
			for (int x = l; x < nl; x++) {
				res[ni][x] = (b & 1) ? -2 : -1;
			}
			for (int x = nr + 1; x <= r; x++) {
				res[ni][x] = (b & 1) ? -1 : -2;
			}
			self(b+1, ni, nl, nr);
		}
	})(0, 0, 1, divs[0]);

	assert(res[0][0] == -3);
	res[0][0] = 1;
	for (int i = 0; i < B; i++) {
		for (int j = st[i]; j < st[i+1]; j++) {
			assert(res[j][0] == -3);
			res[j][0] = i & 1;
		}
	}

	for (auto& a : res) {
		a.resize(N+1);
		for (int j = 1; j <= N; j++) {
			// set it to whatever
			if (a[j] == -3) a[j] = st.back()-1;
		}
	}

	return res;
}
