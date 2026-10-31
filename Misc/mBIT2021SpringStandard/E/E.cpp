#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	string S; cin >> S;
	array<int, 26> cnt; cnt.fill(0);
	for (char c : S) cnt[c - 'a']++;
	int g = 0;
	for (int v : cnt) g = std::gcd(g, v);
	if (g == 1) {
		cout << "IMPOSSIBLE" << '\n';
	} else {
		for (int z = 0; z < g; z++) {
			for (int c = 0; c < 26; c++) {
				for (int v = 0; v < cnt[c] / g; v++) {
					cout << char('a'+c);
				}
			}
		}
		cout << '\n';
	}

	return 0;
}
