#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(3e4) + 200;
int N;
int nxt[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 1; i <= N; i++) {
		scanf("%d", &nxt[i]);
	}
	int res = 0;
	for (int i = 1; i <= N; i++) {
		if (nxt[i]) {
			res++;
			int cur = nxt[i];
			while (cur != i) {
				int n = nxt[cur];
				nxt[cur] = 0;
				cur = n;
			}
			nxt[i] = 0;
		}
	}
	printf("%d\n", res);
	return 0;
}
