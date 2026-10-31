#include<bits/stdc++.h>
using namespace std;


const int MAXN = 450;
const int MAXM = 450;
int N, M;

using bs = bitset<MAXN>;
bs solver[MAXM];

bool go() {
	cin >> N >> M;

	for (int j = 0; j < M; j++) {
		solver[j] = bs{};
	}

	for (int i = 0; i < N; i++) {
		string s; cin >> s;
		for (int j = 0; j < M; j++) {
			solver[j][i] = (s[j] == 'Y');
		}
	}

	for (int i = 1; i < N; i++) {
		bs losers;
		for (int j = 0; j < M; j++) {
			if (solver[j][0] && solver[j][i]) {
				// take it
				losers |= ~solver[j];
			}
		}

		bool found = false;
		for (int j = 0; j < M; j++) {
			if (!solver[j][0] && solver[j][i]) {
				bs v = solver[j] & (~losers);
				assert(v[i]);
				if (v.count() == 1) {
					found = true;
					break;
				}
			}
		}

		if (!found) {
			goto bad;
		}

		for (int j = 0; j < M; j++) {
			if (solver[j][0]) {
				losers |= ~solver[j];
			}
		}
		for (int k = 1; k < N; k++) {
			if (k == i) continue;
			if (!losers[k]) {
				goto bad;
			}
		}
		return true;
bad:
		;
	}
	return false;
}

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	int T; cin >> T;
	for (int t = 1; t <= T; t++) {
		bool ans = go();
		cout << "Case #" << t << ": " << "NY"[ans] << '\n';
	}

	return 0;
}
