#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);

	int N; array<int, 3> W; cin >> N >> W[0] >> W[2] >> W[1];
	W[2] = W[0] - W[1] - W[2];

	for (int z = 0; z < 3; z++) W[z]++;

	vector<int> A(N); for (auto& a : A) cin >> a, a++;

	array<vector<int>, 3> nxt_loc;
	for (int z = 0; z < 3; z++) {
		nxt_loc[z].resize(N+1);
		int64_t sum = 0;
		for (int i = 0, j = 0; i <= N; i++) {
			while (j < N && A[j] + sum <= W[z]) {
				sum += A[j];
				j++;
			}

			nxt_loc[z][i] = j;

			if (i < N) sum -= A[i];
		}
	}


	vector<int> jump_start(N+1);
	jump_start[0] = 0;
	for (int i = 0; i < N; i++) {
		jump_start[i+1] = nxt_loc[0][jump_start[i]];
	}

	vector<array<int, 20>> jump_1(N+1);
	jump_1[N].fill(N);
	for (int i = N-1; i >= 0; i--) {
		jump_1[i][0] = nxt_loc[2][nxt_loc[1][i]];
		for (int z = 0; z+1 < 20; z++) {
			jump_1[i][z+1] = jump_1[jump_1[i][z]][z];
		}
	}

	vector<int> jump_end(N+1);
	jump_end[N] = 0;
	for (int i = N-1; i >= 0; i--) {
		jump_end[i] = jump_end[nxt_loc[0][i]] + 1;
	}

	int Q; cin >> Q;
	while (Q--) {
		int S, H; cin >> S >> H;
		S--;
		if (jump_end[0] <= S) {
			cout << jump_end[0] + H << '\n';
		} else {
			int cur = jump_start[min(N, S)];
			for (int i = 0; i < 20; i++) {
				if (H & (1 << i)) cur = jump_1[cur][i];
			}

			cout << S + H + jump_end[cur] << '\n';
		}
	}

	return 0;
}
