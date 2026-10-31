#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::vector<int> stk; stk.reserve(N);
	for (int i = 0; i < N; i++) {
		std::string s; cin >> s;
		__int128_t v = 0;
		for (char c : s) v = v * 10 + (c - '0');
		int a = 0;
		while (v > 1) {
			assert(!(v & 1));
			v >>= 1;
			a++;
		}

		while (!stk.empty() && stk.back() < a) {
			stk.pop_back();
		}
		while (!stk.empty() && stk.back() == a) {
			stk.pop_back();
			a++;
		}
		stk.push_back(a);
	}
	assert(N > 0);
	assert(!stk.empty());
	__int128_t ans = __int128_t(1) << stk[0];
	std::string s;
	while (ans) {
		s += char('0' + (ans % 10));
		ans /= 10;
	}
	std::reverse(s.begin(), s.end());

	cout << s << '\n';

	return 0;
}
