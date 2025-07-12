#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	while (T--) {
		int N; cin >> N;
		std::vector<int> S(N);
		{
			std::string S_; S_.reserve(N); cin >> S_;
			assert(int(S_.size()) == N);
			for (int i = 0; i < N; i++) {
				char c = S_[i];
				if ('a' <= c && c <= 'z') S[i] = c - 'a';
				else if ('A' <= c && c <= 'Z') S[i] = 26 + (c - 'A');
				else assert(false);
			}
		}
		std::array<int, 52> freq; freq.fill(0);
		for (int c : S) freq[c]++;
	}

	return 0;
}
