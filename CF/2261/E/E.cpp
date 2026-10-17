#include <bits/stdc++.h>
#include <cassert>

int main() {
	std::ios_base::sync_with_stdio(false), std::cin.tie(nullptr);

	int T; std::cin >> T;
	while (T--) {
		int N, K; std::cin >> N >> K;
		std::string S; S.reserve(N); std::cin >> S;
		assert(int(S.size()) == N);
		assert(!(N & 1));
		std::vector<int> match(N);
		int ans = 0;
		{
			std::vector<int> stk; stk.reserve(N+1);
			stk.push_back(-1);
			for (int i = 0; i < N; i++) {
				if (S[i] == '(') {
					stk.push_back(i);
					{
						int mi = -1, ma = int(stk.size());
						while (ma - mi > 1) {
							int md = (mi + ma) / 2;
							int dist = i+1 - stk[md];
							int cnt = int(stk.size()) - md;
							assert(dist >= cnt);
							if (dist - cnt <= 2 * K) {
								ma = md;
							} else {
								mi = md;
							}
						}
						assert(ma < int(stk.size()));
						if (ma == 0) {
							ans = std::max(ans, K + (int(stk.size()) - 1));
						} else {
							ans = std::max(ans, int(stk.size()) - ma);
						}
					}
				} else if (S[i] == ')') {
					assert(!stk.empty());
					int j = stk.back(); stk.pop_back();
					match[i] = j;
					match[j] = i;
				} else assert(false);
			}
		}
		ans = std::min(ans, N/2);
		std::cout << ans << '\n';
	}

	return 0;
}
