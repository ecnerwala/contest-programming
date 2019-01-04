#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(2e6) + 200;
int N;
int C[MAXN * 2];
char S[MAXN];

int main() {
	scanf("%d\n%s", &N, S);
	long long res = 0;
	int v = MAXN;
	C[v] ++;
	for (int i = 0; i < N; i++) {
		if (S[i] == '(') {
			v++;
			C[v] = 1;
		} else if (S[i] == ')'){
			v--;
			res += C[v];
			C[v] ++;
		} else assert(false);
	}
	cout << res << '\n';
	return 0;
}
