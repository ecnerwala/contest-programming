#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

class WordleBack {

public:

	string deduce(string guess, string colors) {
		int N = int(guess.size());
		string res(N, '\0');

		std::array<int, 26> gcnt; gcnt.fill(0);
		std::array<int, 26> hcnt; hcnt.fill(0);
		std::array<bool, 26> no_more; no_more.fill(false);

		for (int i = 0; i < N; i++) {
			if (colors[i] == 'g') {
				res[i] = guess[i];
			} else if (colors[i] == 'y') {
				gcnt[guess[i] - 'A']++;
				if (no_more[guess[i] - 'A']) return "";
				hcnt[guess[i] - 'A']++;
			} else if (colors[i] == '-') {
				gcnt[guess[i] - 'A']++;
				no_more[guess[i] - 'A'] = true;
			} else assert(false);
		}
		// now, we can increase any cnt where no_more is not true
		int tot_gcnt = std::accumulate(gcnt.begin(), gcnt.end(), 0);
		int tot_hcnt = std::accumulate(hcnt.begin(), hcnt.end(), 0);
		for (int i = 0; i < 26; i++) {
			if (gcnt[i] + hcnt[i] > tot_gcnt) return "";
		}
		for (int i = 0; i < 26; i++) {
			if (no_more[i]) continue;
			int v = std::min(tot_gcnt - tot_hcnt, tot_gcnt - hcnt[i] - gcnt[i]);
			hcnt[i] += v;
			tot_hcnt += v;
		}
		if (tot_hcnt < tot_gcnt) return "";

		assert(tot_hcnt == tot_gcnt);

		while (tot_hcnt > 0) {
			auto assign = [&](int g_char, int h_char) {
				assert(g_char != h_char);
				assert(gcnt[g_char] > 0);
				assert(hcnt[h_char] > 0);
				gcnt[g_char]--;
				hcnt[h_char]--;
				tot_gcnt--;
				tot_hcnt--;
				for (int i = 0; i < N; i++) {
					if (res[i] == '\0' && guess[i] - 'A' == g_char) {
						res[i] = char('A' + h_char);
						return;
					}
				}
				assert(false);
			};
			for (int i = 0; i < 26; i++) {
				if (gcnt[i] + hcnt[i] == tot_gcnt) {
					// match this i with any other j
					for (int j = 0; j < 26; j++) {
						if (j == i) continue;
						if (gcnt[i] > 0 && hcnt[j] > 0) {
							assign(i, j);
							goto found;
						} else if (gcnt[j] > 0 && hcnt[i] > 0) {
							assign(j, i);
							goto found;
						}
					}
					assert(false);
				}
			}
			for (int i = 0; i < 26; i++) {
				for (int j = 0; j < 26; j++) {
					if (i == j) continue;
					if (gcnt[i] > 0 && hcnt[j] > 0) {
						assign(i, j);
						goto found;
					}
				}
			}
			assert(false);
found:;
		}

		return res;
	}
};

void my_tests() {
	auto check_result = [&](string guess, string colors) -> string {
		string res = WordleBack().deduce(guess, colors);
		if (res.empty()) {
			// TODO: Maybe check this?
			cerr << "no solution ? " << guess << ' ' << colors << '\n';
		} else {
			int L = int(guess.size());
			assert(int(colors.size()) == L);
			assert(int(res.size()) == L);
			string ncolors(L, '\0');
			std::map<char, int> cnt;
			for (int i = 0; i < L; i++) {
				if (guess[i] == res[i]) ncolors[i] = 'g';
				else cnt[res[i]]++;
			}
			for (int i = 0; i < L; i++) {
				if (guess[i] != res[i]) {
					if (cnt[guess[i]] > 0) {
						cnt[guess[i]] --;
						ncolors[i] = 'y';
					} else {
						ncolors[i] = '-';
					}
				}
			}
			if (colors != ncolors) {
				cerr << "Bad " << guess << ' ' << colors << ' ' << res << ' ' << ncolors << '\n';
				assert(false);
			}
		}
		return res;
	};
	// FILL IN TESTS
	for (int L = 1; L <= 5; L++) {
		for (int cmsk = 0; cmsk < (1 << (2*L)); cmsk++) {
			string colors;
			for (int i = 0; i < L; i++) {
				int v = ((cmsk >> (2*i)) & 3);
				if (v == 3) {
					colors.clear();
					break;
				}
				colors += "-yg"[v];
			}
			if (colors.empty()) continue;
			for (int gmsk = 0; gmsk < (1 << (2*L)); gmsk++) {
				string guess = "";
				for (int i = 0; i < L; i++) {
					int v = (gmsk >> (2*i)) & 3;
					guess += "ABCD"[v];
				}
				check_result(guess, colors);
			}
		}
	}
	cerr << "should be good " << check_result(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		"------------------------yy"
	) << '\n';
	cerr << "alphabet without Z " << check_result(
		"ABCDEFGHIJKLMNOPQRSTUVWXY",
		"-------------------------"
	) << '\n';
	cerr << "alphabet " << check_result(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		"-------------------------y"
	) << '\n';
	cerr << "alphabet " << check_result(
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
		"--------------------------"
	) << '\n';
}

}  // namespace ecnerwala

using ecnerwala::WordleBack;

// vim:ft=cpp
