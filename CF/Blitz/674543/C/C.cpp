#include <bits/stdc++.h>

int main() {
	std::ios_base::sync_with_stdio(false); std::cin.tie(nullptr);
	std::ifstream fin("input.txt");
	std::ofstream fout("output.txt");
	int N, M; fin >> N >> M;
	std::vector<int64_t> r_cnt(N+1);
	std::vector<int64_t> c_cnt(M+1);
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			int a; fin >> a;
			r_cnt[i+1] += a;
			c_cnt[j+1] += a;
		}
	}
	int ans = 0;
	std::array<int, 3> A; fin >> A[0] >> A[1] >> A[2];
	for (int z = 0; z < 2; z++, std::swap(r_cnt, c_cnt), std::swap(N, M)) {
		for (int i = 0; i < N; i++) r_cnt[i+1] += r_cnt[i];
		std::sort(A.begin(), A.end());
		do {
			for (int i = 1; i < N; i++) {
				for (int j = i+1; j < N; j++) {
					if (A[0] + A[1] + A[2] == r_cnt[N] && r_cnt[i] == A[0] && r_cnt[j] == A[0] + A[1]) {
						ans++;
					}
				}
			}
		} while (std::next_permutation(A.begin(), A.end()));
	}
	fout << ans << '\n';
}
