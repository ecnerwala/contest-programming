#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int64_t N; cin >> N;

	int M; cin >> M;
	std::vector<std::array<int, 2>> splits(M);
	for (auto& [a, b] : splits) cin >> a >> b;

	int Q; cin >> Q;
	for (int q = 0; q < Q; q++) {
		int64_t c, d; cin >> c >> d;

		int64_t num_verts = d-c+1;
		int verts_c = 0;
		int depth_c = 0;
		{
			int64_t l = 1, r = N;
			for (int z = 0; true; z++) {
				auto [a, b] = splits[z % M];
				int64_t m = (l * a + r * b) / (a + b);
				if (m == c) break;

				if (m < c) {
					l = m+1;
					verts_c++;
				} else {
					r = m-1;
				}

				depth_c++;
			}
		}
		int verts_d = 0;
		int depth_d = 0;
		{
			int64_t l = 1, r = N;
			for (int z = 0; true; z++) {
				auto [a, b] = splits[z % M];
				int64_t m = (l * a + r * b) / (a + b);
				if (m == d) break;

				if (m > d) {
					r = m-1;
					verts_d++;
				} else {
					l = m+1;
				}

				depth_d++;
			}
		}

		int64_t ans = 2 * (num_verts + verts_c + verts_d - 1) - depth_c - depth_d;
		cout << ans << '\n';
	}

	return 0;
}
