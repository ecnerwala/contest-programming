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
	int orig_Q; cin >> orig_Q;

	struct trie_node {
		int len;
		int par;
		int fail;
		int cnt = 0;
		std::array<int, 26> ch{};
	};
	std::vector<trie_node> trie; trie.reserve(orig_Q+2);
	// trie 0 is the odd node root
	trie.push_back(trie_node{-1, -1, -1});
	const int PAL_NEG_ONE = 0;
	// trie 1 is the empty string/even node root
	trie.push_back(trie_node{0, -1, 0});
	const int PAL_EMPTY_STRING = 1;

	std::string S; S.reserve(orig_Q);
	int st = 0, en = 0;
	int ans = 0;

	// manacher state; ID of max suffix centered at each point
	std::vector<int> max_centered; max_centered.reserve(orig_Q * 2 + 1);
	int mid = 0;
	int cur_suff = PAL_EMPTY_STRING;

	std::vector<std::pair<int, int>> best_starts; best_starts.reserve(orig_Q * 2 + 1);

	for (int q = 0; q < orig_Q; q++) {
		string op; cin >> op;
		if (op == "push") {
			char c; cin >> c;
			S += c;
			en++;

			while (!(mid >= en + st && S[en-1] == S[mid - en])) {
				int nmid = mid + trie[cur_suff].len - trie[trie[cur_suff].fail].len;

				max_centered.push_back(cur_suff);
				for (int i = mid + 1; i < nmid; i++) {
					int pal = max_centered[2 * mid - i];
					assert((i + trie[pal].len) < 2 * en);
					max_centered.push_back(pal);
				}
				assert(nmid == int(max_centered.size()));

				mid = nmid;
				cur_suff = trie[cur_suff].fail;
			}

			c -= 'a';
			if (!trie[cur_suff].ch[c]) {
				int idx = int(trie.size());
				trie[cur_suff].ch[c] = idx;

				int fail = trie[cur_suff].fail;
				while (fail != -1 && !(trie[fail].ch[c] && S[en - 1] == S[en - trie[fail].len - 2])) {
					fail = trie[fail].fail;
				}
				fail = fail == -1 ? 1 : trie[fail].ch[c];

				//cerr << idx << " = " << cur_suff << '+' << char(c + 'a') << ", " << "fail = " << fail << '\n';

				trie.push_back(trie_node{trie[cur_suff].len + 2, cur_suff, fail, 0});
			}
			cur_suff = trie[cur_suff].ch[c];
		} else if (op == "pop") {
			st++;
		}
		assert(st <= en);
		cout << ans << '\n';
	}

	return 0;
}
