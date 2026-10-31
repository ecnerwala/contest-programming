#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {

		int64_t N; cin >> N;
		int64_t v = int64_t(std::sqrt(N / 13));

		auto get_pref = [&](int64_t a) {
			return a * (a+1) / 2 * 26;
		};
		while (get_pref(v) < N) v++;
		while (get_pref(v-1) >= N) v--;
		char c = char('A' + (N - get_pref(v-1) - 1) / v);

		cout << "Case #" << case_num << ": " << c << '\n';
	}

	return 0;
}
