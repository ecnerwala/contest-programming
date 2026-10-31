#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; int64_t L, F; cin >> N >> M >> L >> F;
		assert(N == M);
		cout << (L + F) * (N-1) + std::max(L, F) << '\n';
	}

	return 0;
}
