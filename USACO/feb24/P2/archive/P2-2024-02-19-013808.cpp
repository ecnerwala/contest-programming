#include <bits/stdc++.h>

template <typename T> void setmin(T& a, T b) { if (b < a) a = b; }

int solve(std::vector<int> A, std::vector<int> input_inds) {
	int N = int(A.size());
	const int INF = 1.1e6;

	int K = int(input_inds.size());
	std::vector<std::pair<int, int>> locs(K+2);
	locs[0] = {-1, INF};
	for (int i = 1; i <= K; i++) {
		int x = input_inds[i-1];
		locs[i] = {x, A[x]};
	}
	locs[K+1] = {N, INF};
	K++;

	int orig_sum = std::accumulate(A.begin(), A.end(), 0);

	std::vector<int> pool; pool.reserve(N - (K-1));
	for (int k = 0; k < K; k++) {
		for (int i = locs[k].first + 1; i <= locs[k+1].first - 1; i++) {
			pool.push_back(A[i]);
		}
	}
	assert(int(pool.size()) == N - (K-1));
	N -= (K-1);
	assert(int(pool.size()) == N);
	std::sort(pool.begin(), pool.end());

	int interval_diffs = 0;
	struct interval_t {
		int width;
		int lo;
		int hi;
	};
	std::vector<interval_t> intervals; intervals.reserve(K);
	for (int k = 0; k < K; k++) {
		int w = locs[k+1].first - locs[k].first - 1;
		assert(w >= 0);
		int a = locs[k].second, b = locs[k+1].second;
		if (a > b) std::swap(a, b);
		assert(a <= b);
		interval_diffs += b - a;
		if (w > 0) {
			intervals.push_back({w, a, b});
		} else {
			// just don't count this interval, we'll just handle it upfront by subtracting from interval diffs
			interval_diffs -= (b - a) * 2;
		}
	}
	assert(!(interval_diffs & 1));
	K = int(intervals.size());
	assert(K >= 0);
	const int INF_COST = INF * (K+2);

	//cerr << "K = " << K << '\n';
	//for (auto [w, a, b] : intervals) cerr << w << ' ' << a << ' ' << b << '\n';
	//cerr << "N = " << N << '\n';
	//for (auto v : pool) cerr << v << '\n';

	std::vector<int> subset_len(1 << K);
	subset_len[0] = 0;
	for (int i = 0; i < K; i++) {
		for (int m = 0; m < (1 << i); m++) {
			subset_len[(1 << i) + m] = subset_len[m] + intervals[i].width;
		}
	}

	std::vector<std::vector<int>> dp_exact(N+1, std::vector<int>(1 << K, INF_COST));
	dp_exact[N][0] = 0;

	for (int l = N - 1; l >= 0; l--) {
		std::vector<std::vector<int>> dp_right(N+1, std::vector<int>(1 << K, INF_COST));
		dp_exact[l][0] = 0;
		dp_right[l+1][0] = 0;
		for (int r = l + 1; r <= N; r++) {
			for (int msk = 1; msk < (1 << K); msk++) {
				if (r - l == subset_len[msk]) {
					for (int i = 0; i < K; i++) {
						if (msk & (1 << i)) {
							setmin(
								dp_exact[l][msk],
								dp_right[r][msk - (1 << i)]
								+ (std::max(intervals[i].hi, pool[r - 1])
								- std::min(intervals[i].lo, pool[l]))
							);
						}
					}
				}
			}
			if (r == N) break;
			for (int msk = 0; msk < (1 << K); msk++) {
				if (dp_right[r][msk] >= INF_COST) continue;
				setmin(dp_right[r+1][msk], dp_right[r][msk]);
				int comp_msk = (1 << K) - 1 - msk;
				for (int n_msk = comp_msk; n_msk != 0; n_msk = (n_msk - 1) & comp_msk) {
					assert(!(msk & n_msk));
					if (dp_exact[r][n_msk] >= INF_COST) continue;
					if (r + subset_len[n_msk] > N) continue;

					setmin(dp_right[r + subset_len[n_msk]][msk + n_msk], dp_right[r][msk] + dp_exact[r][n_msk]);
				}
			}
		}
	}

	int best_val = dp_exact[0][(1 << K) - 1];

	int actual_ans = orig_sum - INF - (interval_diffs / 2) + best_val;
	return actual_ans;
}

// Actual answer
// = cyclic_answer - 2 * INF
// = (sum of all givens + INF) + 1/2 cyclic_differences - 2 * INF
// = (sum of all givens) + 1/2 cyclic_differences - INF
// = (sum of all givens) - INF + sum(1/2 interval_differences)
// = (sum of all givens) - INF + sum((interval max - interval min) - 1/2 interval |left - right|)
// = (sum of all givens) - INF + sum(interval max - interval min) - 1/2 interval |left - right|

int slow(std::vector<int> A, std::vector<int> inds) {
	int res = 1e9;
	std::vector<int> B = A;
	std::sort(B.begin(), B.end());
	do {
		bool good = true;
		for (auto x : inds) { if (B[x] != A[x]) good = false; }
		if (good) {
			int cnd = 0;
			for (int i = 0; i+1 < int(A.size()); i++) {
				cnd += std::max(A[i], A[i+1]);
			}
			res = std::min(res, cnd);
		}
	} while (std::next_permutation(B.begin(), B.end()));
	return res;
}

void test() {
	using std::cerr;
	std::mt19937 rng(48);
	for (int T = 0; T < 1000; T++) {
		int N = std::uniform_int_distribution<int>(1, 6)(rng);
		cerr << "Running test " << T << " N = " << N << '\n';
		std::vector<int> A(N);
		for (auto& a : A) {
			a = std::uniform_int_distribution<int>(0, 20)(rng);
		}
		std::vector<int> X;
		for (int i = 0; i < N; i++) {
			if (rng() & 1) X.push_back(i);
		}
		auto mine = solve(A, X);
		auto slo = slow(A, X);
		if (mine != slo) {
			for (auto a : A) { cerr << a << ' '; } cerr << '\n';
			for (auto a : X) { cerr << a << ' '; } cerr << '\n';
			cerr << slo << " != " << mine << '\n';
			assert(false);
		}
	}
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	test();

	int N, K;
	cin >> N >> K;
	std::vector<int> A(N);
	for (auto& a : A) {
		cin >> a;
	}
	std::vector<int> X(K);
	for (auto& x : X) {
		cin >> x; x--;
	}
	cout << solve(A, X) << '\n';

	return 0;
}
