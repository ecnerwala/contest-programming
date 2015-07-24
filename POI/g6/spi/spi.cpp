#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.2e6;
int N;
char S[MAXN];

int main() {
	scanf("%d %s", &N, S);
	int res = 0;
	for(int i = 0; i < N; i++) {
		if(S[i] == 'Z') {
			res++;
			if(S[++i] == 'Z') {
				i++;
			}
		}
	}
	printf("%d\n", res);
	return 0;
}
