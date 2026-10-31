#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N, C, V; cin >> N >> C >> V;
	string S; cin >> S;
	cout << ([&]() -> bool {
		int cons = 0, vow = 0;
		for (char c : S) {
			if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'y') {
				if (++vow > V) return false;
			} else {
				vow = 0;
			}
			if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
				cons = 0;
			} else {
				if (++cons > C) return false;
			}
		}
		return true;
	}() ? "YES" : "NO") << '\n';

	return 0;
}
