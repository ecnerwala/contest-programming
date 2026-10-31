#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const std::string INVALID = "INVALID";

	std::string S; cin >> S;
	for (char c : S) {
		if (c != 'E' && c != 'O') {
			cout << INVALID << '\n';
			exit(0);
		}
	}
	if (S.back() != 'O') {
		cout << INVALID << '\n';
		exit(0);
	}
	for (int i = 0; i+1 < int(S.size()); i++) {
		if (S[i] == 'O' && S[i+1] == 'O') {
			cout << INVALID << '\n';
			exit(0);
		}
	}

	using i128 = __int128_t;
	i128 coeff = 1, add = 0;
	i128 denom = 1;

	for (char c : S) {
		if (c == 'E') {
			denom *= 2;
		} else if (c == 'O') {
			coeff *= 3;
			add *= 3;
			add += denom;
		} else assert(false);
	}

	for (i128 tgt = 1; true; tgt *= 2) {
		i128 n = (denom * tgt - add);
		if (n > (coeff << 47)) break;
		if (n <= 0) continue;
		if (n % coeff != 0) continue;
		n /= coeff;
		bool good = true;
		i128 cur = n;
		for (char c : S) {
			assert(cur > 0);
			if (c == 'O') {
				if (!(cur & 1)) {
					good = false;
					break;
				}
				cur = 3 * cur + 1;
			} else if (c == 'E') {
				if ((cur & (cur-1)) == 0) {
					good = false;
					break;
				}
				if (cur & 1) {
					good = false;
					break;
				}
				cur = cur / 2;
			}
		}
		if (!good) continue;
		assert(cur == tgt);
		std::string ans;
		while (n) {
			ans += char('0' + (n % 10));
			n /= 10;
		}
		std::reverse(ans.begin(), ans.end());
		cout << ans << '\n';
		exit(0);
	}
	cout << INVALID << '\n';
	exit(0);

	return 0;
}
