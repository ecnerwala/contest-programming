#include <bits/stdc++.h>

using std::cerr;

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

void slow(int N, int K) {
	using num = double;
	std::vector<num> ev(1<<N);
	for (int msk = (1 << N) - 1; msk >= 0; msk--) {
		if (__builtin_popcount(msk) != K) continue;
		int invs = 0;
		num tot_time = 0;
		for (int i = 0; i < N; i++) {
			for (int j = i+1; j < N; j++) {
				if (((msk >> i) & 1) == 1 && ((msk >> j) & 1) == 0) {
					// inversion
					int nmsk = msk ^ (1 << i) ^ (1 << j);
					assert(nmsk > msk);
					invs++;
					tot_time += ev[nmsk];
				}
			}
		}
		if (invs == 0) {
			ev[msk] = 0;
		} else {
			ev[msk] = 1 + tot_time / invs;
		}
	}
	cerr << std::fixed << std::setprecision(20);
	for (int msk = 0; msk < (1 << N); msk++) {
		if (__builtin_popcount(msk) != K) continue;
		for (int i = 0; i < N; i++) cerr << ((msk >> i) & 1);
		cerr << ' ' << ev[msk] << '\n';
	}
}

void test() {
	for (int N = 1; N <= 5; N++) {
		for (int K = 0; K <= N; K++) {
			slow(N, K);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	test();

	return 0;
}
