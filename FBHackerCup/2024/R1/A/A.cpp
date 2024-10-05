#include<bits/stdc++.h>

struct fraction_t {
	int n, d;
	friend auto operator <=> (const fraction_t& a, const fraction_t& b) {
		return int64_t(a.n) * int64_t(b.d) <=> int64_t(b.n) * int64_t(a.d);
	}
};

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N; cin >> N;
		fraction_t lo{0, 1};
		fraction_t hi{1, 0};
		for (int i = 1; i <= N; i++) {
			int a, b; cin >> a >> b;
			// sonic must run at least i/b and at most i/a
			assert(a < b);
			lo = std::max(lo, fraction_t{i, b});
			hi = std::min(hi, fraction_t{i, a});
		}
		cout << "Case #" << case_num << ": ";
		if (lo < hi) {
			cout << double(lo.n) / double(lo.d) << '\n';
		} else {
			cout << -1 << '\n';
		}
	}

	return 0;
}
