#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	constexpr bool TESTING = true;

	std::mt19937 mt(64);
	int T;
	if constexpr (TESTING) {
		T = 1000000;
	} else {
		cin >> T;
	}
	while (T--) {
		int p_actual;
		int m_actual;
		if constexpr (TESTING) {
			p_actual = std::uniform_int_distribution<int>(27, 51)(mt);
			//m_actual = std::uniform_int_distribution<int>(p_actual+2, 200)(mt);
			m_actual = 2 * p_actual;
		}
		auto query = [&](std::string s) -> int {
			if constexpr (TESTING) {
				cerr << '?' << ' ' << s << '\n' << flush;
				int64_t r = 0;
				for (int i = int(s.size())-1; i >= 0; i--) {
					r *= p_actual; r += (s[i]-'a'+1); r %= m_actual;
				}
				return int(r);
			} else {
				cout << '?' << ' ' << s << '\n' << flush;
				int res; cin >> res; return res;
			}
		};
		// ask for p+1
		// p+1 < m, so we just get the value of p
		int p = query("aa") - 1;
		std::string s(30, 'z');
		int64_t v_init = query(s);
		int64_t sub_idx;
		int64_t sub_val;
		{
			int64_t v = v_init;
			sub_idx = 0;
			while (v >= p) {
				sub_idx++;
				v /= p;
			}
			assert(0 <= v && v < p);
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
		assert(actual_sub_val <= 2*int64_t(v_init));
		actual_sub_val -= v_init;
		assert(actual_sub_val >= 1);
		assert(actual_sub_val <= v_init);

		int res = query(s);

		int64_t m = res + actual_sub_val;

		cout << '!' << ' ' << p << ' ' << m << '\n' << flush;
	}

	return 0;
}
