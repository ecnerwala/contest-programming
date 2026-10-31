#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	std::string player; cin >> player;
	int N, K; cin >> N >> K;
	assert(N == K * 10);
	std::string S_all; cin >> S_all;
	std::vector<int> res; res.reserve(K);
	for (int z = 0; z < K; z++) {
		for (int i = 0; i < 10; i += 3) {
			if (i == 9) {
				res.push_back(10 * z + i);
				break;
			}
			std::string cur = S_all.substr(10 * z + i, 3);
			int a = (cur[i] == '1');
			int b = (cur[i+1] == '1');
			int c = (cur[i+2] == '1');
			if (a + b + c == 0) continue;
			else if (a + b + c == 3) continue;
			else if (a + b + c == 1) {
				int p1 = int(cur.find('1'));
				res.push_back(10 * z + i + p1);
			} else if (a + b + c == 2) {
				int p0 = int(cur.find('0'));
				if (player == "Flim") p0++;
				else p0 += 2;
				p0 %= 3;

				res.push_back(10 * z + i + p0);
			} else assert(false);
			break;
		}
	}

	for (int i = 0; i < K; i++) {
		cout << res[i] << " \n"[i+1==K];
	}

	return 0;
}
