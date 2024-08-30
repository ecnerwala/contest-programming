#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, A, B; cin >> N >> A >> B;
		int G = std::gcd(A, B);
		std::vector<int> C(N);
		for (auto& c : C) { cin >> c; c %= G; }
		std::sort(C.begin(), C.end());
		int ans = C.back() - C.front();
		for (int i = 0; i+1 < int(C.size()); i++) {
			ans = std::min(ans, C[i] + G - C[i+1]);
		}
		cout << ans << '\n';
	}

	return 0;
}
