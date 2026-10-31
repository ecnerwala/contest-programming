#ifndef ECNERWALA_MACHINE
#define NDEBUG
#endif

#include <bits/stdc++.h>
using namespace std;

namespace ecnerwala {

int dp[65][257][257];
std::pair<int, int> prv[65][257][257];

class BalancedChessboard {

public:

	vector <string> addPawns(vector <string> board) {
		const int BAD = 65;
		for (int it = 0; it <= 64; it++) {
			for (int i = 0; i < 257; i++) {
				for (int j = 0; j < 257; j++) {
					dp[it][i][j] = BAD;
					prv[it][i][j] = {-1, -1};
				}
			}
		}

		dp[0][128][128] = 0;

		int it = 0;
		for (int i = 0; i <= 7; i++) {
			for (int j = 0; j <= 7; j++) {
				int wi = 2*i-7;
				int wj = 2*j-7;
				char c = board[i][j];
				assert(c == 'O' || c == '-');
				for (int x = 0; x <= 256; x++) {
					for (int y = 0; y <= 256; y++) {
						if (dp[it][x][y] == BAD) continue;
						if (c == '-' && dp[it][x][y] < dp[it+1][x][y]) {
							dp[it+1][x][y] = dp[it][x][y];
							prv[it+1][x][y] = {-1, -1};
						}

						if (0 <= x + wi && x + wi <= 256 && 0 <= y + wj && y + wj <= 256) {
							if (dp[it][x][y] + 1 < dp[it+1][x+wi][y+wj]) {
								dp[it+1][x+wi][y+wj] = dp[it][x][y] + 1;
								prv[it+1][x+wi][y+wj] = {i, j};
							}
						}
					}
				}
				it++;
			}
		}
		assert(dp[it][128][128] != BAD);
		std::vector<std::string> ans(8, std::string(8, '-'));
		int cx = 128, cy = 128;
		for (; it > 0; it--) {
			if (prv[it][cx][cy] == std::pair<int, int>{-1, -1}) {
				// do nothing
			} else {
				auto [i, j] = prv[it][cx][cy];
				ans[i][j] = 'O';
				int wi = 2*i-7;
				int wj = 2*j-7;
				cx -= wi;
				cy -= wj;
			}
		}
		return ans;
	}
};

void my_tests() {
	// FILL IN TESTS
}

}  // namespace ecnerwala

using ecnerwala::BalancedChessboard;

// vim:ft=cpp
