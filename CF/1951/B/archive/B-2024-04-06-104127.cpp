#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K; K--;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		int V = A[K];
		int cnt = 0;
		int best = 0;
		for (int i = 0; i < N; i++) {
			if (i == K) continue;
			if (A[i] < V) cnt++;
			else cnt = 0;
			best = std::max(best, cnt);
		}
		cout << best << '\n';
	}

	return 0;
}
