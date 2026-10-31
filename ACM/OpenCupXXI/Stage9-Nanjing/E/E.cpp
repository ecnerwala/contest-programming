#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int mx, my; cin >> mx >> my;
		int cup = 0, cdown = 0, cleft = 0, cright = 0;
		string S; cin >> S;
		for (char c : S) {
			if (c == 'U') cup++;
			else if (c == 'D') cdown++;
			else if (c == 'L') cleft++;
			else if (c == 'R') cright++;
			else assert(false);
		}

		auto check_string = [&](string S) -> bool {
			int cx = 0, cy = 0;
			if (mx == cx && my == cy) return false;
			for (char c : S) {
				if (c == 'U') cy++;
				else if (c == 'D') cy--;
				else if (c == 'L') cx--;
				else if (c == 'R') cx++;
				else assert(false);
				if (mx == cx && my == cy) return false;
			}
			return true;
		};

		vector<pair<char, int>> perm({{'D', cdown}, {'L', cleft}, {'R', cright}, {'U', cup}});
		assert(is_sorted(perm.begin(), perm.end()));
		string cnd; cnd.reserve(S.size());
		do {
			cnd.clear();
			for (auto& [c, v] : perm) {
				cnd.append(v, c);
			}
			if (check_string(cnd)) {
				cout << cnd << '\n';
				goto found;
			}
		} while (next_permutation(perm.begin(), perm.end()));
		assert((mx == 0 && my == 0) || (cleft + cright == 0) || (cup + cdown == 0) || (mx == cright - cleft && my == cup - cdown));
		cout << "Impossible" << '\n';
found:
		continue;
	}

	return 0;
}
