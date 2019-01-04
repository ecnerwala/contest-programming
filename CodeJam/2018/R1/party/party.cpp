#include<bits/stdc++.h>
using namespace std;

typedef int64_t ll;

const int MAXC = 2000;
const int MAXR = 2000;
int R, C;
ll B;

ll M[MAXC];
ll S[MAXC];
ll P[MAXC];

ll cnts[MAXC];
bool is_good(ll T) {
	for (int i = 0; i < C; i++) {
		if (P[i] <= T) {
			cnts[i] = min(M[i], (T-P[i]) / S[i]);
		} else {
			cnts[i] = 0;
		}
	}
	sort(cnts, cnts + C);
	reverse(cnts, cnts + C);
	ll cur = 0;
	for (int i = 0; i < R; i++) {
		cur += cnts[i];
	}
	return cur >= B;
}

ll go() {
	ll mi = -1;
	ll ma = ll(2e18);
	while (ma - mi > 1) {
		ll md = (mi + ma) / 2;
		if (!is_good(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}
	return ma;
}

int main() {
	int T; scanf("%d", &T);

	for(int case_num = 1; case_num <= T; case_num ++) {

		scanf("%d %" SCNd64 "%d", &R, &B, &C);
		assert(R <= C);
		for (int i = 0; i < C; i++) {
			scanf(" %" SCNd64 " %" SCNd64 " %" SCNd64, &M[i], &S[i], &P[i]);
		}

		ll res = go();
		printf("Case #%d: %" PRId64 "\n", case_num, res);
	}

	return 0;
}
