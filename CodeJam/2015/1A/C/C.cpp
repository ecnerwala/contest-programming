#include<bits/stdc++.h>
using namespace std;

int X[4000], Y[4000];

const double EPS = 1e-9;

const double PI = atan2(1, 0) * 2;

int main() {
	int T;
	scanf("%d", &T);
	for(int t = 1; t <= T; t++) {
		int N;
		scanf("%d", &N);
		for(int i = 0; i < N; i++) {
			scanf("%d%d", &X[i], &Y[i]);
		}

		printf("Case #%d:\n", t);

		for(int c = 0; c < N; c++) {

			vector<double> ang;
			for(int i = 0; i < N; i++) {
				if(i == c) continue;
				ang.push_back(atan2(Y[i] - Y[c], X[i] - X[c]));
				ang.push_back(atan2(Y[i] - Y[c], X[i] - X[c]) + PI + PI);
			}
			sort(ang.begin(), ang.end());

			int res = 0;

			for(int l = 0, r = 0; l < N - 1; l++) {
				while(ang[r] < ang[l] + PI + EPS) {
					r++;
				}
				res = max(res, r - l);
			}

			res = N - 1 - res;

			printf("%d\n", res);
		}
	}
	return 0;
}
