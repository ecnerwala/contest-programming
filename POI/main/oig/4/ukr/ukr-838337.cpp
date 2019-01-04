#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(5e6) + 200;
int N;
char C[MAXN];

int main() {
	scanf("%d\n%s", &N, C);
	long long res = 0;
	long long val = 0;
	for (int i = 0; i < N; i++) {
		if ('0' <= C[i] && C[i] <= '9') {
			val *= 10;
			val += C[i] - '0';
		} else {
			res += val;
			val = 0;
		}
	}
	res += val;
	printf("%lld\n", res);
	return 0;
}
