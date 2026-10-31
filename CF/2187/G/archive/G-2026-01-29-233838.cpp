#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	int T; std::cin >> T;
	while (T--) {
		int N; std::cin >> N;
		std::vector<std::string> S(N); for (auto& r : S) { std::cin >> r; assert(int(r.size()) == N); }
		std::vector<std::array<int, 2>> ch(N);
		for (int i = 0; i < N; i++) {
			ch[i] = {-1, -1};
			for (int j = i-1; j >= 0; j--) if (S[j][i] == '1') ch[i][0] = j;
			for (int j = i+1; j < N; j++) if (S[j][i] == '1') ch[i][1] = j;
		}
		std::vector<int> A(N, -1);
		for (int i = 0, v = 0; v < N; v++) {
			A[i] = v;
			std::array<int, 2> pa{-1, -1};
			for (int j = i-1; j >= 0; j--) if (S[i][j] == '1') pa[0] = j;
			for (int j = i+1; j < N; j++) if (S[i][j] == '1') pa[1] = j;
			bool g0 = (pa[0] != -1 && ch[pa[0]][1] == i);
			bool g1 = (pa[1] != -1 && ch[pa[1]][0] == i);
			assert(g0 + g1 == 1);
			i = g0 ? pa[0] : pa[1];
		}
		for (int i = 0; i < N; i++) {
			std::cout << A[i]+1 << " \n"[i+1==N];
		}
	}
}
