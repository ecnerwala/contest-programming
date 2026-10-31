#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string S; cin >> S;
	std::array<int, 26> freq; freq.fill(0);
	for (char c : S) freq[c - 'a']++;

	int64_t ans = int64_t(N) * int64_t(N+1) / 2;
	for (auto v : freq) {
		ans -= int64_t(v) * int64_t(v-1) / 2;
	}
	cout << ans << '\n';

	return 0;
}
