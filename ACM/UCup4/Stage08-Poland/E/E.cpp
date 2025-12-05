#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	int L = int(1e5);
	std::vector<int> dist(L, 26);
	std::vector<int> freq(27, 0);
	freq[26] += L;
	int max_dist = 26;
	while (N--) {
		std::string S; cin >> S;
		assert(int(S.size()) == 5);
		int v = 0;
		for (char c : S) {
			assert('0' <= c && c <= '9');
			v *= 10;
			v += (c - '0');
		}
		[&](this auto&& self, int cur, int cur_dist) -> void {
			if (cur_dist >= dist[cur]) return;
			freq[dist[cur]]--;
			dist[cur] = cur_dist;
			freq[dist[cur]]++;
			for (int b = 0, p10 = 1, cur_pref = cur; b < 5; b++, cur_pref /= 10, p10 *= 10) {
				int dig = cur_pref % 10;
				self(cur + p10 * (dig == 9 ? -9 : +1), cur_dist + 1);
				self(cur + p10 * (dig == 0 ? +9 : -1), cur_dist + 1);
			}
		}(v, 0);
		while (!freq[max_dist]) max_dist--;
		cout << max_dist << ' ' << freq[max_dist] << '\n';
	}

	return 0;
}
