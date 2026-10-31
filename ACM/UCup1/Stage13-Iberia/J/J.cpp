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

void test() {
	using namespace std;
	for (int N = 1; N <= 15; N++) {
		cout << "N = " << N << '\n';
		std::vector<std::string> best_strings;
		int best_val = N*N+1;
		for (int msk = 0; msk < (1 << N); msk++) {
			std::string S; S.reserve(N);
			for (int i = 0; i < N; i++) {
				S += char('0' + ((msk >> (N-1-i)) & 1));
			}
			auto vals = manacher(S);
			int val = std::accumulate(vals.begin(), vals.end(), 0);
			if (val < best_val) {
				best_strings.clear();
				best_val = val;
			}
			if (val == best_val) {
				best_strings.push_back(S);
			}
		}
		cout << "best value" << ' ' << best_val << '\n';
		for (string S : best_strings) {
			cout << S << '\n';
		}
		for (string S : best_strings) {
			auto vals = manacher(S);
		}
	}

}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, M; int64_t K; cin >> N >> M >> K;
	K--;

	std::vector<int> res(N);
	if (M == 1) {
		// do nothing
	} else if (M == 2) {
		std::vector<std::string> cnds = {
			"001011",
			"001101",
			"010011",
			"010110",
			"011001",
			"011010",
			"100101",
			"100110",
			"101001",
			"101100",
			"110010",
			"110100",
		};
		assert(cnds.size() == 12);
		if (N == 2) {
			cnds = {"01", "10"};
		} else if (N < 6) {
			for (auto& s : cnds) {
				s.resize(N);
			}
			cnds.resize(std::unique(cnds.begin(), cnds.end()) - cnds.begin());
		} else {
			for (auto& s : cnds) {
				s.reserve(N);
				while (int(s.size()) < N) {
					s += s.end()[-6];
				}
			}
		}
		if (K < int(cnds.size())) {
			for (int i = 0; i < N; i++) {
				res[i] = cnds[K][i] - '0';
			}
			K = 0;
		} else {
			K = 1;
		}
	} else {
		assert(M >= 3);
		for (int i = N-1; i >= 0; i--) {
			int base = i == 0 ? M : i == 1 ? M-1 : M-2;
			res[i] = int(K % base);
			K /= base;
		}
		for (int i = 0; i < N; i++) {
			if (i == 0) {
				// do nothing
			} else if (i == 1) {
				if (res[i] >= res[i-1]) {
					res[i]++;
				}
			} else {
				assert(res[i-1] != res[i-2]);
				if (res[i] >= std::min(res[i-1], res[i-2])) {
					res[i]++;
				}
				if (res[i] >= std::max(res[i-1], res[i-2])) {
					res[i]++;
				}
			}
		}
	}

	if (K != 0) {
		cout << -1 << '\n';
	} else {
		for (int i = 0; i < N; i++) {
			cout << res[i]+1 << " \n"[i+1==N];
		}
	}

	return 0;
}
