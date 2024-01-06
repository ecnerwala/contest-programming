#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		std::vector<int> prv_smaller(N);
		{
			std::vector<int> stk; stk.reserve(N);
			for (int i = 0; i < N; i++) {
				while (!stk.empty() && stk.back() >= A[i]) {
					stk.pop_back();
				}
				prv_smaller[i] = (stk.empty() ? -1 : stk.back());
				stk.push_back(A[i]);
			}
		}
		std::vector<int> nxt_smaller(N);
		{
			std::vector<int> stk; stk.reserve(N);
			for (int i = N-1; i >= 0; i--) {
				while (!stk.empty() && stk.back() >= A[i]) {
					stk.pop_back();
				}
				nxt_smaller[i] = (stk.empty() ? -1 : stk.back());
				stk.push_back(A[i]);
			}
		}
		bool good = true;
		int cnt_0 = 0;
		for (int i = 0; i < N; i++) {
			if (A[i] == 0) {
				cnt_0++;
				if (cnt_0 >= 2) {
					good = false;
					break;
				}
			} else {
				if (std::max(prv_smaller[i], nxt_smaller[i]) == A[i] - 1) {
					// we're good
				} else {
					good = false;
					break;
				}
			}
		}
		if (cnt_0 != 1) good = false;
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
