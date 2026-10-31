#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		struct bin_t { int a, s; };
		std::vector<bin_t> bins(N);
		for (auto& b : bins) cin >> b.a;
		for (auto& b : bins) cin >> b.s;
		std::sort(bins.begin(), bins.end(), [&](auto b1, auto b2) { return b1.s - b1.a < b2.s - b2.a; });
		int64_t extra = 0;
		bool good = true;
		for (auto b : bins) {
			if (b.a == 0) continue;
			if (extra >= b.s - b.a) {
				extra += b.a;
			} else {
				good = false;
				break;
			}
		}
		cout << (good ? "YES" : "NO") << '\n';
	}

	return 0;
}
