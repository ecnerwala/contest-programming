#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const bool TEST = false;
	int N; int64_t K;
	if (TEST) {
		N = 4, K = 2;
	} else {
		cin >> N >> K;
	}

	vector<int64_t> A(N, K);
	vector<int64_t> NA(N);
	auto take_step = [&]() {
		fill(NA.begin(), NA.end(), 0);
		NA[1] += A[0];
		for (int i = 1; i < N; i++) {
			NA[i-1] += A[i]/2;
			NA[(i+1==N?0:i+1)] += A[i] - A[i]/2;
		}
		swap(A, NA);
	};

	int loc = 1;

	auto query = [&](int i) -> int64_t {
		cout << '?' << ' ' << i+1 << '\n' << flush;
		if (TEST) {
			cerr << A[(i+N-loc) % N] << '\n';
			return A[(i+N-loc) % N];
		} else {
			int64_t v; cin >> v;
			return v;
		}
	};
	{
		auto v = query(0);
		assert(v == K);
	}


	int mi = 0;
	int ma = N;
	while (ma - mi > 1) {
		take_step();
		int num_big = 0;
		for (int i = 1; i < N && A[i] > K; i++) {
			num_big++;
		}
		int num_small = 0;
		for (int i = N-1; i >= 0 && A[i] < K; i--) {
			num_small++;
		}
		assert(num_big == num_small);
		if (num_big + num_small + 1 >= ma - mi) {
			// binary search
			int md = (mi + ma) / 2;
			int64_t v = query(md);
			if (v == K) {
				mi = md, ma = md+1;
			} else if (v > K) {
				ma = md;
			} else {
				mi = md + 1;
			}
		} else {
			assert(mi + num_big < ma);
			int cnd = mi + num_big;
			int64_t v = query(cnd);
			if (v > K) {
				ma = cnd;
			} else {
				mi = cnd;
			}
		}
	}
	assert(ma - mi == 1);

	cout << '!' << ' ' << mi+1 << '\n' << flush;
	if (TEST) {
		assert(mi == loc);
	}

	return 0;
}

// 2 2 2* 2
// 2 1 2* 3
// 2 1 2* 3

// 2 2 2* 2 2
// 2 1 2* 3 2
// 1 1 2* 3 3
// 2 1 2* 3 2
