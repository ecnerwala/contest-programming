#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	for (int M = 1; M <= 100; M++) {
		const int N = 1020;
		std::vector<std::vector<int>> winning_moves(N);
		std::string res(N, '?');
		for (int i = 0; i < N; i++) {
			if (winning_moves[i].empty()) {
				for (int j = i+1; j < N && j <= i+M; j++) winning_moves[j].push_back(i);
				res[i] = 'X';
			} else if (winning_moves[i].size() == 1) {
				int k = 2 * i - winning_moves[i][0];
				if (k < N) winning_moves[k].push_back(i);
				res[i] = ' ';
			} else {
				res[i] = ' ';
			}
		}
		if (__builtin_ctz(M+1) % 2 == 0) {
			for (int i = 0; i < N; i++) {
				assert((res[i] == 'X') == ((i % (M+1)) == 0));
			}
			continue;
		}
		std::vector<int> losing;
		for (int i = 0; i < N; i++) {
			if (res[i] == 'X') {
				losing.push_back(i);
			}
		}
		cerr << M << ' ';
		//cerr << res << ' ';
		for (int z = 1; z < int(losing.size()); z++) cerr << losing[z] - losing[z-1] - (M+1) << ' ';
		cerr << '\n';
	}

	return 0;
}
