#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e6;
int N;
char L[MAXN], R[MAXN];

int T;

int main() {
	scanf("%d %s %s %d", &N, L, R, &T);
	priority_queue<int> pq;
	for(int i = 0; i < N; i++) {
		pq.push(-i);
	}
	while(T--) {
		int a, b; scanf("%d %d", &a, &b);
		pq.push(-a);
		pq.push(-b);
		swap(L[a], R[b]);
		while(!pq.empty()) {
			int i = -pq.top();
			if(L[i] == R[i]) {
				pq.pop();
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
