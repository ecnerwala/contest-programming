#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e7;
int N;
int main() {
	scanf("%d", &N);
	int i = 0;
	while(N >= (1 << i)) i++;
	printf("%d\n", i);
	for(int v = N; v; v--) {
		printf("%c", char('a' + __builtin_ctz(v)));
	}
	printf("\n");
}
