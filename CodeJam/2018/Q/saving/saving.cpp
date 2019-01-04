#include<bits/stdc++.h>
using namespace std;

typedef int64_t ll;

ll D;

const int MAXN = 50;
int N;
char P[MAXN+1];

int cnt[MAXN];

int main() {
	int T; scanf("%d", &T);

	for(int case_num = 1; case_num <= T; case_num ++) {
		scanf("%" SCNd64 " %s", &D, P);

		N = int(strlen(P));

		memset(cnt, 0, sizeof(cnt));

		ll tot = 0;
		int chr = 0;
		for (int i = 0; i < N; i++) {
			if (P[i] == 'C') {
				chr++;
			} else if (P[i] == 'S') {
				cnt[chr] ++;
				tot += (1ll << chr);
			}
		}

		int res = 0;
		while (tot > D && chr > 0) {
			if (cnt[chr] == 0) {
				chr--;
				continue;
			}
			cnt[chr] --;
			cnt[chr-1] ++;
			tot -= (1ll << (chr-1));
			res++;
		}

		printf("Case #%d: ", case_num);

		if (tot > D) {
			printf("IMPOSSIBLE");
		} else {
			printf("%d", res);
		}
		printf("\n");
	}

	return 0;
}
