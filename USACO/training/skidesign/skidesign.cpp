/*
ID: he.andr1
LANG: C++
TASK: skidesign
*/

#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e3;
int N;
int H[MAXN];

int main() {
	if(fopen("skidesign.in", "r")) {
		freopen("skidesign.in", "r", stdin);
		freopen("skidesign.out", "w", stdout);
	}

	scanf("%d", &N);
	int res = 0;
	for (int i = 0; i < N; i++) {
		scanf("%d", &H[i]);
		res += H[i] * H[i];
	}

	for (int h = 0; h <= 100; h++) {
		int val = 0;

		for (int i = 0; i < N; i++) {
			int g = min(h + 17, max(H[i], h));
			val += (g - H[i]) * (g - H[i]);
		}

		res = min(res, val);
	}
	printf("%d\n", res);


	return 0;
}
