#include <bits/stdc++.h>

std::vector<int> solve(std::string player, int N, int K, std::string S_all) {
	assert(N == K * 10);
	std::vector<int> res; res.reserve(K);
	for (int z = 0; z < K; z++) {
		for (int i = 0; ; i += 3) {
			if (i == 9) {
				res.push_back(10 * z + i);
				break;
			}
			assert(i < 9);
			std::string cur = S_all.substr(10 * z + i, 3);
			int a = (cur[0] == '1');
			int b = (cur[1] == '1');
			int c = (cur[2] == '1');
			if (a + b + c == 0) {
				continue;
			} else if (a + b + c == 3) {
				continue;
			} else if (a + b + c == 1) {
				int p1 = int(cur.find('1'));
				res.push_back(10 * z + i + p1);
				break;
			} else if (a + b + c == 2) {
				int p0 = int(cur.find('0'));
				if (player == "Flim") p0++;
				else p0 += 2;
				p0 %= 3;

				res.push_back(10 * z + i + p0);
				break;
			} else assert(false);
			assert(false);
		}
	}
	assert(int(res.size()) == K);
	return res;
}

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string player; cin >> player;
	int N, K; cin >> N >> K;
	std::string S_all; cin >> S_all;

	auto res = solve(player, N, K, S_all);

	for (int i = 0; i < K; i++) {
		cout << res[i]+1 << " \n"[i+1==K];
	}

	return 0;
}
