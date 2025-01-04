#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int64_t> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int64_t> B = A;
		for (int z = 0; z < 2; z++, std::reverse(A.begin(), A.end()), std::reverse(B.begin(), B.end())) {
			std::vector<int> zero_pos; zero_pos.reserve(N+1);
			int64_t cur_stack = 0;
			for (int i = 1; i < N; i++) {
				zero_pos.push_back(i-1);
				// we send freq guys into the left side
				int64_t freq = cur_stack + A[i-1];
				int64_t nxt_stack = 0;
				while (freq > 0) {
					// every guy you send in pushes zero right by one; if zero == i, then it doesn't shoot back out
					if (zero_pos.empty()) {
						int dist = i - (-1);
						nxt_stack += (dist - 1) * (freq / dist);
						freq %= dist;
						if (freq == 0) break;
						zero_pos.push_back(-1);
					}
					int dist = i - zero_pos.back();
					if (freq >= dist) {
						freq -= dist;
						nxt_stack += dist - 1;
						zero_pos.pop_back();
					} else {
						zero_pos.back() += int(freq);
						nxt_stack += freq;
						freq = 0;
						break;
					}
				}
				cur_stack = nxt_stack;
				B[i] += cur_stack;
			}
		}
		for (int i = 0; i < N; i++) cout << B[i] << " \n"[i+1==N];
	}
	return 0;
}
