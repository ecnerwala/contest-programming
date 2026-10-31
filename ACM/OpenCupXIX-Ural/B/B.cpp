#include<bits/stdc++.h>
using namespace std;

void wssert(bool b) {
	if (!b) {
		cout << "PRESENTATION ERROR\n";
		exit(0);
	}
}

int autoCorrelation(uint64_t msk, int l, int s) {
	if (s >= l) return 0;
	int tot = l - s;
	int numNeg = __builtin_popcountll(((msk >> s) ^ msk) & ((1ull << tot) - 1));
	return tot - 2 * numNeg;
}

const int MAXN = 40;
int N;
string S[4];

uint64_t v[4];
vector<pair<bool, int>> ops;

bool dfs(int ind) {
	if (ind == int(ops.size())) {
		return true;
	}
	if (ops[ind].first == true) {
		int s = ops[ind].second;
		if (autoCorrelation(v[0], N, s) + autoCorrelation(v[1], N, s) + autoCorrelation(v[2], N, s) * 2 + autoCorrelation(v[3], N-1, s) * 2 != 0) {
			return false;
		}
		return dfs(ind + 1);
	} else {
		int i = ops[ind].second / N;
		int j = ops[ind].second % N;
		//assert(S[i][j] == '?');
		for (int z = 0; z < 2; z++) {
			if (dfs(ind + 1)) {
				return true;
			}
			v[i] ^= (1ull << j);
		}
		return false;
	}
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> S[0] >> S[1] >> S[2] >> S[3];
	N = int(S[0].size());
	wssert(int(S[1].size()) == N);
	wssert(int(S[2].size()) == N);
	wssert(int(S[3].size()) == N-1);

	for (int s = N-1; s >= 0; s--) {
		for (int i = 0; i < 4; i++) {
			int l = (i < 3 ? N : N-1);
			if (s < l) {
				// fix S[i][s] and S[i][l-s-1]
				if (s >= l - s - 1) {
					if (S[i][s] == '+') {
						v[i] |= (0ull << s);
					} else if (S[i][s] == '-') {
						v[i] |= (1ull << s);
					} else if (S[i][s] == '?') {
						ops.emplace_back(false, i * N + s);
					} else assert(false);
				}

				if (s > l - s - 1) {
					if (S[i][l - s - 1] == '+') {
						v[i] |= (0ull << (l - s - 1));
					} else if (S[i][l - s - 1] == '-') {
						v[i] |= (1ull << (l - s - 1));
					} else if (S[i][l - s - 1] == '?') {
						ops.emplace_back(false, i * N + (l - s - 1));
					} else assert(false);
				}
			}
		}
		if (s > 0) ops.emplace_back(true, s);
	}

	//for (int i = 0; i < 4; i++) { cerr << v[i] << '\n'; }

	//for (auto it : ops) { cerr << it.first << ' ' << it.second << '\n'; }

	bool found = dfs(0);
	//assert(found);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < (i < 3 ? N : N-1); j++) {
			cout << ((v[i] & (1ull << j)) ? '-' : '+');
		}
		cout << '\n';
	}
	return 0;
}
