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
		std::string S; cin >> S;
		[&]() -> void {
			int N = int(S.size());
			bool all_same = true;
			for (int i = 0; i < N; i++) {
				if (S[i] != S[0]) {
					all_same = false;
					break;
				}
			}
			if (all_same) {
				cout << "NO" << '\n';
				return;
			}

			bool all_pal = true;
			for (int i = 0; i < N-1-i; i++) {
				if (S[i] != S[N-1-i]) {
					all_pal = false;
					break;
				}
			}
			if (!all_pal) {
				cout << "YES" << '\n';
				cout << 1 << '\n';
				cout << S << '\n';
				return;
			}

			assert(all_pal && !all_same);
			assert(N >= 3);

			auto man = manacher(S);
			auto is_pal = [&](int l, int r) -> bool {
				assert(0 <= l && l <= r && r <= N);
				return r - l <= man[l+r];
			};

			for (int i = 1; i < N; i++) {
				if (!is_pal(0, i) && !is_pal(i, N)) {
					cout << "YES" << '\n';
					cout << 2 << '\n';
					cout << S.substr(0, i) << ' ' << S.substr(i) << '\n';
					return;
				}
			}

			if (S[1] == S[0]) {
				// We must have aaaaabaaaaa
				cout << "NO" << '\n';
				return;
			} else {
				// We must be alternating ababababa
				cout << "NO" << '\n';
				return;
			}
		}();
	}

	return 0;
}
