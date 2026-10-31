#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		// ask for p+1
		cout << '?' << ' ' << "aa" << '\n' << flush;
		// p+1 < m, so we just get the value of p
		int p; cin >> p; p--;
		std::string s(30, 'z');
		cout << '?' << ' ' << s << '\n' << flush;
		int v_init; cin >> v_init;
		int sub_idx;
		int sub_val;
		if (v_init == 0) {
			sub_idx = 0;
			sub_val = 1;
		} else {
			int v = v_init;
			sub_idx = 0;
			while (v >= p) {
				sub_idx++;
				v /= p;
			}
			assert(1 <= v && v < p);
			if (v <= 24) {
				// subtract v+1, which is <= 25
				sub_val = v+1;
			} else {
				assert(2*v >= p);
				sub_idx++;
				sub_val = 1;
			}
		}
		assert(1 <= sub_val && sub_val <= 25);
		s[sub_idx] -= sub_val;
		assert('a' <= s[sub_idx] && s[sub_idx] <= 'z');

		int64_t actual_sub_val = sub_val;
		for (int z = 0; z < sub_idx; z++) actual_sub_val *= p;
		assert(actual_sub_val >= v_init+1);
		assert(actual_sub_val <= 2*v_init);
		actual_sub_val -= v_init;
		assert(actual_sub_val >= 1);
		assert(actual_sub_val <= v_init);

		cout << '?' << ' ' << s << '\n' << flush;
		int res; cin >> res;

		cout << '!' << ' ' << p << ' ' << res + actual_sub_val << '\n' << flush;
	}

	return 0;
}
