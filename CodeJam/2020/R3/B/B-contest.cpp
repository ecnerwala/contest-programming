#include<bits/stdc++.h>

int solve(std::vector<int64_t> X) {
	using namespace std;

	int N = int(X.size())-1;
	int64_t K = X.back();

	assert(X[0] == 0);

	{
		int64_t cur = 0;
		int64_t lo = X[N-1]-K, hi = 0;
		for (int i = 0; i < N; i++) {
			cur = 2 * X[i] - cur;
			if (i % 2 == 0) {
				// it's reversed
				// X[i] < cur - v < X[i+1]
				lo = max(lo, cur - X[i+1]);
				hi = min(hi, cur - X[i]);
			} else {
				// X[i] < v + cur < X[i+1]
				lo = max(lo, X[i] - cur);
				hi = min(hi, X[i+1] - cur);
			}
		}
		if ((N-1) % 2 == 0) {
			// reversed
			// cur - v == v + K
			// v = (cur - K) / 2
			if (2 * lo < (cur - K) && (cur - K) < 2 * hi) {
				return N;
			}
		} else {
			// cur + v == v + K
			if (lo < hi && cur == K) {
				return N;
			}
		}
	}

	X.reserve(2*N+1);
	for (int i = 1; i <= N; i++) {
		X.push_back(X[i]+K);
	}
	assert(int(X.size()) == 2 * N + 1);

	// otherwise we do the dp
	vector<int> nxt(N);
	for (int i = 0; i < N; i++) {
		int64_t cur = 0;
		int64_t lo = X[i], hi = X[i+1];
		nxt[i] = 0;
		for (int j = 0; j < N; j++) {
			int k = i+1+j;
			cur = 2 * X[k] - cur;
			if (j % 2 == 0) {
				lo = max(lo, cur - X[k+1]);
				hi = min(hi, cur - X[k]);
			} else {
				lo = max(lo, X[k] - cur);
				hi = min(hi, X[k+1] - cur);
			}
			if (lo < hi) {
				nxt[i] = j+1;
			} else {
				break;
			}
		}
	}
	int ans = 2*N;
	for (int i = 0; i < N; i++) {
		int cnd = N;
		int len = 0;
		while (len < N) {
			cnd++;
			len += nxt[(i+len)%N];
		}
		ans = min(ans, cnd);
	}
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		//cerr << "Case #" << case_num << '\n';
		int64_t K; int N; cin >> K >> N;
		vector<int64_t> X(N);
		for (int i = 0; i < N; i++) cin >> X[i];
		vector<int> V(N);
		for (int i = 0; i < N; i++) cin >> V[i];
		for (int i = 0; i < N; i++) {
			assert(V[i] != V[(i+1)%N]);
		}

		for (int i = 1; i < N; i++) {
			X[i] -= X[0];
		}
		X[0] = 0;
		X.push_back(K);
		assert(X.back() == K);

		int ans = solve(X);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
