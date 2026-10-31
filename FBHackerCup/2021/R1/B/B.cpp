#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int T; cin >> T;
	for (int case_num = 1; case_num <= T; case_num ++) {
		int N, M; cin >> N >> M;
		int A, B; cin >> A >> B;
		assert(A <= 1000 && B <= 1000);

		cout << "Case #" << case_num << ": ";
		if (std::min(A, B) < N + M - 1) {
			cout << "Impossible" << '\n';
		} else {
			cout << "Possible" << '\n';
			assert(N >= 2 && M >= 2);
			for (int i = 0; i < N; i++) {
				for (int j = 0; j < M; j++) {
					int v;
					if (i == 0 && j == 0) {
						v = A-(N-1)-(M-1);
					} else if (i == 0 && j == M-1) {
						v = B-(N-1)-(M-1);
					} else {
						v = 1;
					}
					assert(1 <= v && v <= 1000);
					cout << v << " \n"[j+1==M];
				}
			}
		}
	}

	return 0;
}
