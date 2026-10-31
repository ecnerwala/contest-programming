#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		cerr << "Case #" << case_num << ":" << '\n';
		int N, M, K; int64_t X; cin >> N >> M >> K >> X;
		//N = 1000000; M = 1000000;
		vector<int64_t> S(N);
		{
			for (int i = 0; i < K; i++) cin >> S[i];
			int64_t A, B, C, D; cin >> A >> B >> C >> D;
			for (int i = K; i < N; i++) {
				S[i] = (A * S[i-2] + B * S[i-1] + C) % D + 1;
			}
		}
		vector<int64_t> P(M);
		{
			for (int i = 0; i < K; i++) cin >> P[i];
			int64_t A, B, C, D; cin >> A >> B >> C >> D;
			for (int i = K; i < M; i++) {
				P[i] = (A * P[i-2] + B * P[i-1] + C) % D + 1;
			}
		}

		sort(S.begin(), S.end());
		sort(P.begin(), P.end());
		assert(P.front() >= 1);

		// each mailbox is stuffed with 0 or 1 packages and a ton of keys

		int64_t orig_val = 1;
		for (int i = 0; i < N; i++) {
			orig_val += max(S[i] / X - 1, int64_t(0));
		}
		auto is_good = [&](int V) -> bool {
			int64_t val = orig_val;
			assert(0 <= V && V <= M);
			for (int j = V-1; j >= 0; j--) {
				val -= P[j] / X;
			}
			if (val < 0) return false;

			multiset<int64_t> st;
			for (int j = V-1, i = N-1; j >= 0; j--) {
				for (; i >= 0 && S[i] >= P[j]; i--) {
					int64_t v = max(S[i], X) % X;
					st.insert(v);
				}

				if (st.empty()) return false;
				auto it = st.lower_bound(P[j] % X);
				if (it == st.end()) {
					val--;
					if (val < 0) return false;
					it = st.begin();
				}
				st.erase(it);
			}

			return val >= 0;
		};

		int mi = 0, ma = M+1;
		while (ma - mi > 1) {
			int md = (mi + ma) / 2;
			if (is_good(md)) {
				mi = md;
			} else {
				ma = md;
			}
		}

		cerr << "Case #" << case_num << ": " << mi << '\n';
		cout << "Case #" << case_num << ": " << mi << '\n';
	}

	return 0;
}
