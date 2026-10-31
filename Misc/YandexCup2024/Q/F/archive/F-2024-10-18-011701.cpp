#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	// we're outputting
	std::vector<int> code; code.reserve(1 << 11);
	std::array<int, 4> msks = {
		0b101010101010101,
		0b110011001100110,
		0b111100001111000,
		0b111111110000000,
	};
	std::vector<int> decode(1 << 15, -1);
	for (int i = 0; i < (1 << 15); i++) {
		bool good = true;
		for (int m : msks) {
			if (__builtin_popcount(m & i) & 1) good = false;
		}
		if (good) {
			int v = int(code.size());
			code.push_back(i);
			decode[i] = v;
			for (int a = 0; a < 15; a++) decode[i ^ (1 << a)] = i;
		}
	}
	cout << 15 << '\n';
	for (int i = 0; i < 15; i++) {
		bool first = true;
		for (int v = 1; v <= 2024; v++) {
			if ((code[v] >> i) & 1) {
				if (!first) cout << ' ';
				first = true;
				cout << v;
			}
		}
		cout << '\n';
	}
	cout << flush;
	while (T--) {
		int ans = 0;
		for (int i = 0; i < 15; i++) {
			int a; cin >> a;
			assert(a == 0 || a == 1);
			ans |= (a << i);
		}
		cout << decode[ans] << '\n';
	}

	return 0;
}
