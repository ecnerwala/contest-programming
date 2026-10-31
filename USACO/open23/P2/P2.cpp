#include <bits/stdc++.h>

std::string gen_string(int64_t A, int64_t B) {
	std::string s(A+B, '?');
	int64_t v = 0;
	for (int64_t i = 0; i < A+B; i++) {
		if (v >= 0) {
			v -= B;
			s[i] = '0';
		} else {
			v += A;
			s[i] = '1';
		}
	}
	return s;
}

bool is_good(std::string s) {
	int64_t A = 0, B = 0;
	for (char c : s) {
		if (c == '0') A++;
		else if (c == '1') B++;
		else assert(false);
	}
	return s == gen_string(A, B);
}

int64_t cnt_slow(int64_t A, int64_t B) {
	std::cerr << "slow " << A << ' ' << B << '\n';
	std::string s = gen_string(A, B);
	int64_t ans = 0;
	for (int64_t i = 2; i <= int(s.size()); i++) {
		if (is_good(s.substr(0, i))) {
			std::cerr << "g " << i << '\n';
			ans ++;
		}
	}
	return ans;
}

int64_t solve(int64_t A, int64_t B) {
	//std::cerr << "solve " << A << ' ' << B << '\n';
	assert(A >= 1 && B >= 1);

	// Bx - Ay > 0
	int64_t lo_x = 1, lo_y = 0, lo_v = B;
	// Ay - Bx >= 0
	int64_t hi_x = 0, hi_y = 1, hi_v = A;

	int64_t ans = 0;

	while (hi_v != 0) {
		assert(hi_x + hi_y >= lo_x + lo_y);
		if (hi_v >= lo_v) {
			// the next one is still bigger
			int64_t num_steps = hi_v / lo_v;
			//for (int i = 1; i <= num_steps; i++) { std::cerr << "g " << (hi_x + hi_y) + (lo_x + lo_y) * i << '\n'; }
			ans += num_steps;
			hi_x += num_steps * lo_x;
			hi_y += num_steps * lo_y;
			hi_v -= num_steps * lo_v;
		} else {
			assert(lo_v > hi_v);
			int64_t num_steps = (lo_v - 1) / hi_v;
			assert(num_steps >= 1);
			//for (int i = 1; i <= num_steps; i++) { std::cerr << "g " << (hi_x + hi_y) * i + (lo_x + lo_y) << '\n'; }
			ans += num_steps;
			lo_x += num_steps * hi_x;
			lo_y += num_steps * hi_y;
			lo_v -= num_steps * hi_v;

			assert(hi_v >= lo_v);
			//for (int i = 1; i <= 1; i++) { std::cerr << "g " << (hi_x + hi_y) + (lo_x + lo_y) * i << '\n'; }
			ans++;
			// How many multiple >= 2x also work?
			// This is almost num_steps, but there are some edge cases if lo_x == hi_x or lo_y == hi_y
			if (hi_x > 0 && hi_y > 0) {
				int64_t c = std::max((hi_x + lo_x - 1) / hi_x, (hi_y + lo_y - 1) / hi_y) - 1;
				//for (int i = 1; i <= c; i++) { std::cerr << "g " << (hi_x + hi_y) * (i+1) << '\n'; }
				ans += c;
			}
			hi_x += lo_x;
			hi_y += lo_y;
			hi_v -= lo_v;
		}
	}

	// This is like case 2, but we just hardcode it
	int64_t c = A / hi_x - 1;
	//for (int i = 1; i <= c; i++) { std::cerr << "g " << (hi_x + hi_y) * (i+1) << '\n'; }
	ans += c;
	ans += c;
	return ans;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int64_t A, B; cin >> A >> B;
		/*
		int64_t slo = cnt_slow(A, B);
		cerr << "ans " << slo << '\n';
		*/
		int64_t res = solve(A, B);
		//cerr << "ours " << res << '\n';
		//std::cerr << '\n';
		cout << res << '\n';
	}

	return 0;
}
