#include<bits/stdc++.h>

int main() {
	using namespace std;
	ios_base::sync_with_stdio(false), cin.tie(nullptr);
	int N; cin >> N;
	struct constraint {
		bool z;
		uint64_t v;
	};
	vector<constraint> rows[2] = {vector<constraint>(N), vector<constraint>(N)};
	for (int d = 0; d < 2; d++) {
		for (int i = 0; i < N; i++) {
			cin >> rows[d][i].z;
		}
	}

	for (int d = 0; d < 2; d++) {
		for (int i = 0; i < N; i++) {
			cin >> rows[d][i].v;
		}
	}

	auto id = [](bool z) -> uint64_t { return z ? uint64_t(0) : ~uint64_t(0); };
	auto op = [](bool z, uint64_t a, uint64_t b) -> uint64_t { return z ? (a | b) : (a & b); };

	uint64_t accum[2][2] = {{id(0), id(1)}, {id(0), id(1)}};
	for (int d = 0; d < 2; d++) {
		for (int i = 0; i < N; i++) {
			for (int z = 0; z < 2; z++) {
				accum[d][z] = op(z, accum[d][z], rows[d][i].v);
			}
		}
	}

	vector<bool> parity[2] = {vector<bool>(N), vector<bool>(N)};
	for (int d = 0; d < 2; d++) {
		bool cur_parity[2] = {true, true};
		for (int i = 0; i < N; i++) {
			parity[d][i] = (cur_parity[rows[d][i].z] ^= 1);
		}
	}

	auto get_cell = [&](int i, int j) -> uint64_t {
		constraint c[2] = {rows[0][i], rows[1][j]};
		if (c[0].z == c[1].z) {
			return op(!c[0].z, c[0].v, c[1].v);
		} else {
			int d = parity[0][i] ^ parity[1][j];
			return op(!c[d].z, c[d].v, op(!c[!d].z, ~accum[!d][c[d].z], c[!d].v));
		}
	};

	vector<vector<uint64_t>> ans(N, vector<uint64_t>(N));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			ans[i][j] = get_cell(i, j);
		}
	}

	vector<uint64_t> ansVal[2] = {vector<uint64_t>(N), vector<uint64_t>(N)};
	for (int d = 0; d < 2; d++) {
		for (int i = 0; i < N; i++) {
			ansVal[d][i] = id(rows[d][i].z);
		}
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			ansVal[0][i] = op(rows[0][i].z, ansVal[0][i], ans[i][j]);
			ansVal[1][j] = op(rows[1][j].z, ansVal[1][j], ans[i][j]);
		}
	}
	for (int d = 0; d < 2; d++) {
		for (int i = 0; i < N; i++) {
			if (ansVal[d][i] != rows[d][i].v) {
				cout << -1 << '\n';
				exit(0);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			cout << ans[i][j] << " \n"[j+1==N];
		}
	}

	return 0;
}
