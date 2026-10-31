#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; cin >> N;
	std::string S; cin >> S;
	assert(int(S.size()) == N);

	// max palindrome ending at N
	std::vector<int> max_pal(N+1);
	max_pal[0] = 0;
	// longest palindrome which forms a suffix
	std::vector<int> suffix_link(N+1);
	suffix_link[0] = -1;
	// palindrome which shares the center
	std::vector<int> inner_link(N+1);
	inner_link[0] = -2;

	for (int i = 1; i <= N; i++) {
		int cur = i-1;
		int z = 0;
		while (true) {
			int l = (cur < 0 ? cur : max_pal[cur]) + 2;
			if (cur < 0 || S[i-1] == S[i-l]) {
				if (z == 0) {
					inner_link[i] = cur;
					max_pal[i] = l;
				} else {
					// TODO: how to find this child
					suffix_link[i] = child(cur, S[i-1]);
				}
				z++;
				if (z == 2) break;
			}
			cur = cur < 0 ? cur - 1 : suffix_link[cur];
		}
	}

	return 0;
}
