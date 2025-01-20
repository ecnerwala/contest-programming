#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, M; cin >> N >> M;
		std::map<int, int64_t> A;
		for (int i = 0; i < N; i++) {
			int a; cin >> a; A[a]++;
		}
		std::map<int, int64_t> B;
		for (int i = 0; i < M; i++) {
			int b; cin >> b; B[b]++;
		}
		bool good;
		while (true) {
			if (A.empty() && B.empty()) {
				good = true;
				break;
			}
			if (A.empty() || B.empty()) {
				good = false;
				break;
			}
			auto at = A.rbegin();
			auto bt = B.rbegin();
			if (at->first > bt->first) {
				good = false;
				break;
			}
			if (at->first == bt->first && at->second > bt->second) {
				good = false;
				break;
			}
			int v = bt->first;
			int64_t cnt = bt->second;
			B.erase(bt->first);
			if (at->first == bt->first) {
				cnt -= at->second;
				A.erase(at->first);
			}
			assert(cnt >= 0);
			if (cnt > 0) {
				if (v == 1) {
					good = false;
					break;
				}
				B[v / 2] += cnt;
				B[(v+1) / 2] += cnt;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
