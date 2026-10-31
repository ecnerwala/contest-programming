#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> par(2*N, -1);
		auto get_par = [&](int a) -> int {
			while (par[a] >= 0) {
				if (par[par[a]] >= 0) par[a] = par[par[a]];
				a = par[a];
			}
			return a;
		};
		auto merge = [&](int a, int b) -> bool {
			a = get_par(a);
			b = get_par(b);
			if (a == b) return false;
			if (par[a] > par[b]) std::swap(a, b);
			par[a] += par[b];
			par[b] = a;
			return true;
		};
		int cnt = 0;
		std::string s; s.reserve(N);
		for (int i = 0; i < N; i++) {
			cin >> s;
			for (int j = 0; j < N; j++) {
				if (s[j] == '0') {
					cnt += merge(i, N+j);
				}
			}
		}
		if (cnt >= N-1) {
			cout << "Yes" << '\n';
		} else {
			cout << "No" << '\n';
		}
	}

	return 0;
}
