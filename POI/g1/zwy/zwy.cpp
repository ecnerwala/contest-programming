#include<bits/stdc++.h>
using namespace std;

int N;
int V[60];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) scanf("%d", &V[i]);
	int m = V[0];
	for (int i = 0; i < N; i++) m = max(m, V[i]);
	for (int i = 0; i < N; i++) {
		if (V[i] == m) {
			printf("%c", char('A' + i));
		}
	}
	printf("\n");
	return 0;
}
