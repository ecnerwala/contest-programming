#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> freq(N+1);
		for (int z = 0; z < N; z++) { int c; cin >> c; freq[c]++; }
		std::nth_element(freq.begin(), freq.begin()+1, freq.end(), std::greater<int>());
		int a = freq[0];
		int b = freq[1];
		int r = a | b;
		if (a & b) {
			int lg2 = 31 - __builtin_clz(a & b);
			assert((a & b & (1 << lg2)));
			r |= (1 << lg2) - 1;
		}
		cout << r << '\n';
	}

	return 0;
}
