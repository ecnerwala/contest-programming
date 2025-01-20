#include <bits/stdc++.h>

int solve(std::string S, std::string T) {
	T = char(S.front() ^ 1) + T + char(S.back() ^ 1);
	S = char(S.front() ^ 1) + S + char(S.back() ^ 1);
	int N = int(S.size());
	int diff = 0;
	std::vector<int> matches; matches.reserve(N);
	for (int i = 0; i < N; i++) {
		if (diff == 0 && S[i] == T[i]) {
			matches.push_back(i);
		}
		if (S[i] == '1') diff--;
		else diff++;

		if (T[i] == '1') diff++;
		else diff--;
	}
	if (diff != 0) return -1;
	assert(matches.front() == 0);
	assert(matches.back() == N-1);
	int ops = 0;
	for (int i = 0; i+1 < N; i++) {
		ops += (S[i] != S[i+1]);
		ops -= (T[i] != T[i+1]);
	}
	assert(ops & 1);
	for (int z = 0; z+1 < int(matches.size()); z++) {
		int l = matches[z], r = matches[z+1];
		int changes = 0;
		for (int i = l; i+1 < r; i++) {
			changes = T[i] != T[i+1];
		}
		if (changes > 1) return -1;
	}
	return ops / 2;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int CASES; cin >> CASES;
	while (CASES--) {
		std::string S, T; cin >> S >> T;
		cout << solve(S, T) << '\n';
	}

	return 0;
}
