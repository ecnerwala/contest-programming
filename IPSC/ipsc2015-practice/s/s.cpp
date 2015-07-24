#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;

int N;

int pos[4][MAXN];

inline int suit(char c) {
	switch(c) {
		case 'C': return 0;
		case 'D': return 1;
		case 'H': return 2;
		case 'S': return 3;
		default:  return -1;
	}
}

int main() {
	int T; scanf("%d", &T);
	while(T--) {
		scanf("%d", &N);
		for(int i = 0; i < 4 * N; i++) {
			char c; int v;
			scanf(" %c%d", &c, &v);
			v--;
			int s = suit(c);
			pos[s][v] = i;
		}

		int res = 0;
		for(int s = 0; s < 4; s++) {
			int cnt = 1;
			for(int v = 1; v < N; v++) {
				if(pos[s][v] < pos[s][v - 1]) cnt++;
			}
			res = max(res, cnt);
		}
		printf("%d\n", res);
	}
	return 0;
}
