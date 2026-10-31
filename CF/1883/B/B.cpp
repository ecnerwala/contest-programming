#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N, K; cin >> N >> K;
		std::array<int, 26> cnt; cnt.fill(0);
		std::string s; cin >> s;
		for (char c : s) cnt[c - 'a'] ^= 1;
		int num_odd = 0;
		for (int a : cnt) num_odd += a;
		cout << (K < num_odd - 1 ? "NO" : "YES") << '\n';
	}

	return 0;
}
