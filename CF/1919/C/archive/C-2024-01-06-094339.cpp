#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> A(N);
		for (auto& a : A) cin >> a;
		const int INF = N + 1;
		int v0 = INF, v1 = INF;
		int res = 0;
		for (auto a : A) {
			if (a <= v0) {
				v0 = a;
				res++;
			} else if (a <= v1) {
				v1 = a;
				res++;
			} else {
				v0 = v1;
				v1 = a;
			}
		}
		cout << res << '\n';
	}

	return 0;
}
