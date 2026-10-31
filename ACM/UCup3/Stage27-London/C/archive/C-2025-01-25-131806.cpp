#include <bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	const int M = 64;
	const double alpha = 0.709;
	std::mt19937_64 mt(48);
	int N, Q; cin >> N >> Q;
	std::vector<std::array<uint8_t, M>> HLL(N+Q);
	for (int i = 0; i < N; i++) {
		HLL[i].fill(0);
		uint64_t v = mt();
		int idx = v & (M-1);
		HLL[i][idx] = uint8_t(1 + __builtin_clzll(v));
	}

	for (int q = N; q < N+Q; q++) {
		int a, b; cin >> a >> b; a--, b--;
		double hsum = 0;
		int c0 = 0;
		for (int z = 0; z < M; z++) {
			HLL[q][z] = std::max(HLL[a][z], HLL[b][z]);
			hsum += pow(0.5, HLL[q][z]);
			c0 += !HLL[q][z];
		}
		double E = alpha * M * M / hsum;
		if (E < M * 2.5) {
			if (c0 != 0) {
				E = M * log(double(M) / double(c0));
			}
		}

		cout << int(E + 0.5) << '\n';

		if ((q - N) % 50 == 49) cout << flush;
	}
	cout << flush;

	return 0;
}
