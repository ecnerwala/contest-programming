#include <bits/stdc++.h>

/**
 * manacher(S): return the maximum palindromic substring of S centered at each point
 *
 * Input: string (or vector) of length N (no restrictions on character-set)
 * Output: vector res of length 2*N+1
 *   For any 0 <= i <= 2*N:
 *   * i % 2 == res[i] % 2
 *   * the half-open substring S[(i-res[i])/2, (i+res[i])/2) is a palindrome of length res[i]
 *   * For odd palindromes, take odd i, and vice versa
 */
template <typename V> std::vector<int> manacher(const V& S) {
	int N = int(S.size());
	std::vector<int> res(2*N+1, 0);
	for (int i = 1, j = -1, r = 0; i < 2*N; i++, j--) {
		if (i > r) {
			r = i+1, res[i] = 1;
		} else {
			res[i] = res[j];
		}
		if (i+res[i] >= r) {
			int b = r>>1, a = i-b;
			while (a > 0 && b < N && S[a-1] == S[b]) {
				a--, b++;
			}
			res[i] = b-a, j = i, r = b<<1;
		}
	}
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, Q; cin >> N >> Q;
		std::string S;
		cin >> S;
		std::vector<int> max_pal = manacher(S);

		std::vector<int> run_len(N);
		for (int i = 0; i < N; i++) {
			run_len[i] = 1 + (i >= 2 && S[i] == S[i-2] ? run_len[i-2] : 0);
		}

		// inclusive
		auto solve = [&](int l, int r) -> int64_t {
			assert(0 <= l && l <= r && r < N);
			int len = r - l + 1;
			if (len == 1) return 0;

			assert(len >= 2);
			bool alternating = (run_len[r] >= (len + 1) / 2) && (run_len[r-1] >= len / 2);
			if (alternating && S[r] == S[r-1]) {
				// all equal, so all bad
				return 0;
			} else if (alternating) {
				// alternating, so only evens are good
				int v = len / 2;
				return int64_t(v) * int64_t(v + 1);
			} else {
				int64_t ans = int64_t(len) * int64_t(len + 1) / 2;
				// subtract out all bad ones
				// 1 is always bad
				ans -= 1;
				if (max_pal[l + r + 1] >= len) {
					ans -= len;
				}
				return ans;
			}
		};

		for (int q = 0; q < Q; q++) {
			int l, r; cin >> l >> r; l--, r--;
			cout << solve(l, r) << '\n';
		}
	}

	return 0;
}

// A string is k-bad if all substrings of length k are palindromes
// If the string is shorter than the whole thing, then it must be alternating or all the same
//
// k = 1 and k = N are special cases
