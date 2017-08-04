#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;

const int MAXN = 2000;
const int MAXC = 3;
const int MAXM = 2000;
int N, C, M;

pii P[MAXM];

pii go_small() {
	assert(C == 2);
	int n1 = 0;
	int nA = 0;
	int nB = 0;
	for (int i = 0; i < M; i++) {
		if (P[i].first == 1) n1++;

		if (P[i].second == 1) nA ++;
		else if (P[i].second == 2) nB++;
		else assert(false);
	}
	if (n1 >= nA && n1 >= nB) {
		return pii(n1, 0);
	}
	int T = max(nA, nB);
	int cur_mode = 0;
	int max_cnt = 0;
	for (int i = 1; i <= N; i++) {
		int cnt = 0;
		for (int j = 0; j < M; j++) {
			if (P[j].first == i) {
				cnt++;
			}
		}
		if (cnt >= max_cnt) {
			cur_mode = i;
			max_cnt = cnt;
		}
	}
	return pii(T, max(max_cnt - T, 0));
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> N >> C >> M;
		for (int i = 0; i < M; i++) {
			cin >> P[i].first >> P[i].second;
		}
		pii res = go_small();
		cout << "Case #" << case_num << ": " << res.first << ' ' << res.second << '\n';
	}

	return 0;
}
