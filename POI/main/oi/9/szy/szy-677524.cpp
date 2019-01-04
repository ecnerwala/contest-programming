#include<bits/stdc++.h>

using namespace std;

const int MAXL = 22;
int N;
int L[2];
int A[2][MAXL];
int V;

pair<int, int> val[2][1200000];

int main() {
	scanf("%d", &N);
	L[0] = N / 2;
	L[1] = N - L[0];
	for(int t = 0; t < 2; t++) {
		for(int i = 0; i < L[t]; i++) {
			scanf("%d", &A[t][i]);
		}
		for(int i = 0; i < L[t]; i++) {
			for(int j = 0; j < (1 << i); j++) {
				val[t][j + (1 << i)].first = val[t][j].first + A[t][i];
			}
		}
		L[t] = 1 << L[t];
		for(int i = 0; i < L[t]; i++) {
			val[t][i].second = i;
		}
		sort(val[t], val[t] + L[t]);
	}
	scanf("%d", &V);
	for(int i = 0, j = L[1] - 1; i < L[0] && j >= 0;) {
		int v = val[0][i].first + val[1][j].first;
		if(v == V) {
			assert(val[0][i].second < (1 << (N / 2)));
			int m = val[0][i].second | (val[1][j].second << (N / 2));
			for(int a = 0; a < N; a++, m /= 2) {
				cout << (m & 1);
			}
			cout << '\n';
			return 0;
		} else if(v < V) {
			i++;
		} else {
			j--;
		}
	}
}
