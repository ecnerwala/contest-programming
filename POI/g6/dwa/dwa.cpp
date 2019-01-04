#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e6;
int N;
char L[MAXN], R[MAXN];
bool inQ[MAXN];

int T;

int main() {
	scanf("%d %s %s %d", &N, L, R, &T);
	priority_queue<int> pq;
	for(int i = 0; i < N; i++) {
		if (L[i] != R[i]) {
			pq.push(-i);
			inQ[i] = true;
		}
	}
	while(T--) {
		int a, b; scanf("%d %d", &a, &b);
		swap(L[a], R[b]);
		if (L[a] != R[a] && !inQ[a]) {
			pq.push(-a);
			inQ[a] = true;
		}
		if (L[b] != R[b] && !inQ[b]) {
			pq.push(-b);
			inQ[b] = true;
		}
		while(!pq.empty()) {
			int i = -pq.top();
			if(L[i] == R[i]) {
				pq.pop();
				inQ[i] = false;
			} else {
				if(L[i] > R[i]) {
					printf("1\n");
				} else {
					printf("2\n");
				}
				break;
			}
		}
		if(pq.empty()) {
			printf("0\n");
		}
	}
	return 0;
}
