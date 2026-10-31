#ifndef ECNERWALA_MACHINE
#define NDEBUG
#include <chrono>
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class DifferenceSequence {
public:

	vector<int> get_masks_bad(int n) {
		int num_extra = 1 << __builtin_ctz(n);

		// there's nothing else
		if (num_extra == n) return {};

		auto g = [&](int m) -> int {
			if (m & 1) m ^= (m ^ (1 << n) ^ 1) >> 1;
			else m ^= m >> 1;
			return m;
		};

		auto expand_mask = [&](int m) -> int {
			int st = m;
			for (int i = num_extra; i < 32; i *= 2) st ^= st >> i;
			st &= (1 << num_extra) - 1;
			st |= m << num_extra;
			return st;
		};

		auto is_good = [&](int st) -> bool {
			for (int cur = g(st); cur != st; cur = g(cur)) {
				if (cur > st) return false;
			}
			return true;
		};

		vector<int> msks;
		for (int m = 1; m < (1 << (n - num_extra)); m++) {
			int st = expand_mask(m);
			if (is_good(st)) {
				msks.push_back(st);
			}
		}
		return msks;
	}

	vector<int> get_masks(int n) {
		int num_extra = 1 << __builtin_ctz(n);

		// there's nothing else
		if (num_extra == n) return {};

		auto g = [&](int m) -> int {
			/*
			if (m & 1) m ^= (m ^ (1 << n) ^ 1) >> 1;
			else m ^= m >> 1;
			return m;
			*/
			m ^= m << 1;
			return min(m, m ^ (1 << n) ^ 1);
		};

		auto expand_mask = [&](int m) -> int {
			int st = m;
			for (int i = num_extra; i < 32; i *= 2) st ^= st >> i;
			st &= (1 << num_extra) - 1;
			st |= m << num_extra;
			return st;
		};

		auto is_good = [&](int st) -> bool {
			for (int cur = g(st); cur != st; cur = g(cur)) {
				if (cur > st) return false;
			}
			return true;
		};

		vector<int> msks;
		for (int m = 1; m < (1 << (n - num_extra)); m++) {
			int st = expand_mask(m);
			if (is_good(st)) {
				msks.push_back(st);
			}
		}
		return msks;
	}

	vector<long long> kthSmallestRepresentative(int n, long long k) {
		if (k == 1) {
			return vector<long long>(n, 0);
		}
		k -= 2;

		int num_extra = 1 << __builtin_ctz(n);

		// there's nothing else
		if (num_extra == n) return {};

		auto g = [&](int m) -> int {
			m ^= m << 1;
			return min(m, m ^ (1 << n) ^ 1);
		};

		auto expand_mask = [&](int m) -> int {
			int st = m;
			for (int i = num_extra; i < 32; i *= 2) st ^= st >> i;
			st &= (1 << num_extra) - 1;
			st |= m << num_extra;
			return st;
		};

		auto is_good = [&](int st) -> bool {
			for (int cur = g(st); cur != st; cur = g(cur)) {
				if (cur > st) return false;
			}
			return true;
		};

		int cnt = 0;
		for (int m = 1; m < (1 << (n - num_extra)); m++) {
			int st = expand_mask(m);
			if (is_good(st)) {
				cnt++;
			}
		}

		assert(cnt > 0);

		cerr << "cnt = " << cnt << '\n';

		long long fac = (k / cnt) + 1;
		k %= cnt;

		cnt = 0;
		for (int m = 1; m < (1 << (n - num_extra)); m++) {
			int st = expand_mask(m);
			if (is_good(st)) {
				if (cnt++ == k) {
					vector<long long> res(n);
					for (int i = 0; i < n; i++) {
						res[i] = bool(st & (1 << i)) * fac;
					}
					reverse(res.begin(), res.end());
					return res;
				}
			}
		}
		assert(false);
	}
};

void my_tests() {
	// FILL IN TESTS
	for (int n = 2; n <= 26; n++) {
		cerr << "n = " << n << '\n';
		auto good_masks = DifferenceSequence().get_masks(n);
		auto bad_masks = DifferenceSequence().get_masks_bad(n);
		if (good_masks != bad_masks) {
			assert(good_masks.size() == bad_masks.size());
			int num_diff = 0;
			for (int i = 0; i < int(good_masks.size()); i++) {
				if (good_masks[i] != bad_masks[i]) num_diff++;
			}
			cerr << num_diff << "/" << int(good_masks.size()) << " differences" << "(" << double(num_diff) / int(good_masks.size()) << " fraction)" << '\n';
			//long long k = (long long)(1e18) - 1;
			long long k = 107249613;
			k -= 2;
			k %= int(good_masks.size());
			cerr << "With k = 1e18-1: " << good_masks[k] << ' ' << bad_masks[k] << '\n';
		}
	}
	for (int n = 2; n <= 30; n++) {
		cerr << "n = " << n << '\n';
		auto start = chrono::system_clock().now();
		auto v = DifferenceSequence().kthSmallestRepresentative(n, 2);
		auto end = chrono::system_clock().now();
		cerr << std::chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << '\n';
	}
}

}  // namespace ecnerwala

using ecnerwala::DifferenceSequence;

// vim:ft=cpp
