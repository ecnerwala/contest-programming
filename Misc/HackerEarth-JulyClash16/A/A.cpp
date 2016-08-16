#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXM = 100;
const int MAXN = 2e5;

int N, M;
int C[MAXN];

int main() {
	scanf("%d %d", &N, &M);
	for(int i = 0; i < M;i++) {
		scanf("%d", &C[i]);
	}
	ll res = 0;
	for(int i = 0; i < N; i++) {
		int v; scanf("%d", &v); v--;
		int c = C[v];
		res = (res / c) * c;
		bool is_good;
		do {
			res += c;
			is_good = true;
			for(int j = v + 1; j < M; j++) {
				if(res % C[j] == 0) {
					is_good = false;
					break;
				}
			}
		} while(!is_good);
	}
	printf("%lld\n", res);

	return 0;
}
