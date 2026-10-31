#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int N, M, K; int64_t S; cin >> N >> M >> K >> S;
		vector<int64_t> P(N);
		{
			for (int i = 0; i < K; i++) cin >> P[i];
			int64_t A, B, C, D; cin >> A >> B >> C >> D;
			for (int i = K; i < N; i++) {
				P[i] = int((A * P[i-2] + B * P[i-1] + C) % D) + 1;
			}
		}
		vector<int64_t> Q(M);
		{
			for (int i = 0; i < K; i++) cin >> Q[i];
			int64_t A, B, C, D; cin >> A >> B >> C >> D;
			for (int i = K; i < M; i++) {
				Q[i] = int((A * Q[i-2] + B * Q[i-1] + C) % D) + 1;
			}
		}

		sort(P.begin(), P.end());
		sort(Q.begin(), Q.end());

		auto is_good = [&](int64_t t) -> bool {
			int j = 0;
			for (int i = 0; i < N && j < M; i++) {
				// eat as many people as possible
				int j0 = j;
				while (j < M && S * int64_t(j - j0 + 1) + min(abs(P[i] - Q[j]), abs(P[i] - Q[j0])) + (Q[j] - Q[j0]) <= t) {
					j++;
				}
			}
			return j == M;
		};


		int64_t mi = 0;
		int64_t ma = 1e18;
		assert(is_good(ma));
		while (ma - mi > 1) {
			int64_t md = mi + (ma - mi) / 2;
			if (is_good(md)) {
				ma = md;
			} else {
				mi = md;
			}
		}

		cout << "Case #" << case_num << ": " << ma << '\n';
	}

	return 0;
}
