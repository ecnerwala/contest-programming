#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N, M; cin >> N >> M;
	std::vector<int> A(N); for (auto& a : A) cin >> a;
	std::sort(A.begin(), A.end());
	std::vector<int> weights(N+1);
	weights[0] = A[0] - 0;
	for (int i = 1; i < N; i++) {
		weights[i] = A[i] - A[i-1];
	}
	weights[N] = M+1 - weights[N-1];

	int64_t ans = 0;
	std::vector<int> stk; stk.reserve(N+1);
	for (int i = 0; i <= N; i++) {
		int nw = weights[i];
		cerr << nw << '\n';
		while (int(stk.size()) >= 2 && stk[int(stk.size())-2] >= nw) {
			int v = stk.back();
			stk.pop_back();
			stk.back() += v;
			ans += stk.back();
		}
		stk.push_back(nw);
	}
	while (stk.size() >= 2) {
		int v = stk.back();
		stk.pop_back();
		stk.back() += v;
		ans += stk.back();
	}
	cout << ans << '\n';

	return 0;
}
