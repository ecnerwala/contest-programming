#include <bits/stdc++.h>

namespace std {

template<class Fun>
class y_combinator_result {
	Fun fun_;
public:
	template<class T>
	explicit y_combinator_result(T &&fun): fun_(std::forward<T>(fun)) {}

	template<class ...Args>
	decltype(auto) operator()(Args &&...args) {
		return fun_(std::ref(*this), std::forward<Args>(args)...);
	}
};

template<class Fun>
decltype(auto) y_combinator(Fun &&fun) {
	return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

} // namespace std

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int64_t MAX_VAL = int64_t(1) << 34;

	std::vector<int> primes;
	auto is_prime = [&](int p) -> bool {
		if (p < 2) return false;
		for (int i = 2; i * i <= p; i ++) {
			if (p % i == 0) return false;
		}
		return true;
	};

	for (int i = 3; i <= 100; i++) {
		if (is_prime(i)) primes.push_back(i);
	}

	// 3 -> 2
	// 5 -> 2 * 2
	// 7 -> 2 * 3
	// 11 -> 2 * 5
	// 13 -> 2 * 2 * 3
	// 17 -> 2 * 2 * 2 * 2
	// 19 -> 2 * 3 * 3
	// 23 -> 2 * 11
	// 29 -> 2 * 2 * 7
	// 31 -> 2 * 3 * 5
	// 37 -> 2 * 2 * 3 * 3
	auto check_exps = std::y_combinator([&](auto self, std::vector<int> exps, std::vector<int> cur_primes, int64_t cur_val, int64_t cur_phi, int target_d) -> void {
		//cout << "check " << cur_primes.size() << ' ' << cur_val << ' ' << cur_phi << '\n';
		assert(cur_val <= MAX_VAL);
		if (cur_primes.size() == exps.size()) {
			//cout << "check " << cur_val << ' ' << cur_phi << '\n';
			int64_t v = cur_phi;
			int r = 1;
			for (int p = 2; v > 1; p ++) {
				int e = 0;
				while (v % p == 0) { v /= p, e++; }
				r *= e+1;
			}
			if (r == target_d) {
				cout << "FOUND SOLUTION " << cur_val << '\n';
				//exit(0);
			}
			return;
		}
		int i = int(cur_primes.size());
		for (int p : primes) {
			if (cur_val % p == 0) continue;
			if (i > 0 && exps[i-1] == exps[i] && p <= cur_primes[i-1]) continue;

			int64_t nxt_val = cur_val * p;
			int64_t nxt_phi = cur_phi * (p-1);
			if (nxt_val > MAX_VAL) break;
			for (int e = 1; e < exps[i]; e++) {
				nxt_val *= p;
				nxt_phi *= p;
				if (nxt_val > MAX_VAL) break;
			}
			if (nxt_val > MAX_VAL) break;
			if (exps[i] == 1) {
				int64_t hi_val = nxt_val;
				for (int j = i+1; j < int(exps.size()); j++) {
					hi_val *= p;
					if (hi_val > MAX_VAL) break;
				}
				if (hi_val > MAX_VAL) break;
			}
			cur_primes.push_back(p);
			self(exps, cur_primes, nxt_val, nxt_phi, target_d);
			cur_primes.pop_back();
		}
	});
	std::y_combinator([&](auto self, std::vector<int> cur, int64_t cur_val, int cur_d) -> void {
		assert(cur_val <= MAX_VAL);

		do {
			int phi = 0;
			for (int i = 1; i <= cur_d; i++) if (std::gcd(i, cur_d) == 1) phi++;

			if (cur.empty()) continue;

			std::vector<int> nv = cur;
			for (auto& a : nv) a--;
			nv.push_back(int(cur.size()));
			if (*max_element(nv.begin(), nv.end()) >= phi) continue;

			int num_extra = int(cur.size())-1;
			for (int& a : nv) while (phi % (a+1) != 0) a++, num_extra--;
			if (num_extra > 0) {
				std::sort(nv.begin(), nv.end());
				nv.back() += num_extra;
			}
			int v_lo = 1;
			for (int a : nv) v_lo *= a+1;
			if (v_lo > phi) continue;

			cout << cur_d << " " << phi << ' ' << v_lo << ' ' << double(phi) / double(v_lo) << ' ' << cur_val << ": ";
			for (int e : cur) cout << e << ' ';
			cout << '\n' << flush;

			check_exps(cur, std::vector<int>(), 1, 1, phi);
		} while (false);

		cur_val *= primes[cur.size()];
		for (int e = 1; cur_val <= MAX_VAL; e++, cur_val *= primes[cur.size()]) {
			if (!cur.empty() && e > cur.back()) break;
			cur.push_back(e);
			self(cur, cur_val, cur_d * (e+1));
			cur.pop_back();
		}
	})(std::vector<int>(), 1, 1);

	return 0;
}
