#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	struct person_t {
		int id;
		int a, b;
	};
	std::vector<person_t> P(N);
	for (int i = 0; i < N; i++) {
		int a, b; cin >> a >> b;
		P[i] = {i, a, b};
	}
	std::sort(P.begin(), P.end(), [&](const person_t &a, const person_t &b) {
		return a.a < b.a;
	});

	std::vector<int> pref_b(N+1, -1);
	for (int i = 0; i <= N; i++) {
		pref_b[i+1] = std::max(pref_b[i], P[i].b);
	}
	std::vector<int> suff_b(N+1, 1e9);
	for (int i = N-1; i >= 0; i--) {
		suff_b[i] = std::min(suff_b[i+1], P[i].b);
	}

	std::vector<int> ans(N, -1);
	int j = 0;
	for (int i = 1; i <= N; i++) {
		if (pref_b[i] < suff_b[i]) {
			// i is a cutoff
			int res = i-1;
			for (; j < i; j++) {
				ans[P[j].id] = res;
			}
		}
	}
	for (int i = 0; i < N; i++) {
		cout << ans[i] << '\n';
	}

	return 0;
}
