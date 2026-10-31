#include<bits/stdc++.h>

int solve(int64_t K, std::vector<int64_t> X) {
	using namespace std;

	int N = int(X.size());

	X.reserve(2*N+1);
	for (int i = 0; i <= N; i++) {
		X.push_back(X[i]+K);
	}
	assert(int(X.size()) == 2 * N + 1);

	vector<int64_t> los; los.reserve(2*N);
	vector<int64_t> his; his.reserve(2*N);
	{
		int64_t cur = 0;
		for (int i = 0; i < 2*N; i++) {
			cur = 2 * X[i] - cur;
			if (i % 2 == 0) {
				// it's reversed
				// X[i] < cur - v < X[i+1]
				los.push_back(cur - X[i+1]);
				his.push_back(cur - X[i]);
			} else {
				// X[i] < v + cur < X[i+1]
				los.push_back(X[i] - cur);
				his.push_back(X[i+1] - cur);
			}
			if (i+1 == N) {
				int64_t lo = *max_element(los.begin(), los.end());
				int64_t hi = *min_element(his.begin(), his.end());
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
		}
	}

	// otherwise we do the dp
	vector<int> nxt(N);
	deque<int64_t> lo_q;
	deque<int64_t> hi_q;
	for (int i = 0, j = 0; i < N; i++) {
		while (j < i+N && (i==j || max(lo_q.front(), los[j+1]) < min(hi_q.front(), his[j+1]))) {
			++j;
			while (!lo_q.empty() && lo_q.back() < los[j]) {
				lo_q.pop_back();
			}
			lo_q.push_back(los[j]);
			while (!hi_q.empty() && hi_q.back() > his[j]) {
				hi_q.pop_back();
			}
			hi_q.push_back(his[j]);
		}
		assert(i < j);
		nxt[i] = j-i;
		if (lo_q.front() == los[i]) lo_q.pop_front();
		if (hi_q.front() == his[i]) hi_q.pop_front();
	}

	int st = 0;
	for (int z = 0; z < N; z++) {
		st += nxt[st%N];
	}
	int ans = N;
	int len = 0;
	while (len < N) {
		ans++;
		len += nxt[(st+len)%N];
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

		int ans = solve(K, X);

		cout << "Case #" << case_num << ": " << ans << '\n';
	}

	return 0;
}
