#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3e5;
int N;
int A[MAXN];
int B[MAXN];
int loc[MAXN][2];

int main() {
	cin >> N;
	for(int i = 0; i < N; i++) { cin >> A[i]; }
	for(int i = 0; i < N; i++) { cin >> B[i]; }
	for(int i = 0; i + 1 < N; i++) {
		if(A[i] == 0) swap(A[i], A[i + 1]);
		if(B[i] == 0) swap(B[i], B[i + 1]);
	}
	N--;
	for(int i = 0; i < N; i++) A[i] --, B[i] --;
	for(int i = 0; i < N; i++) loc[A[i]][0] = loc[B[i]][1] = i;
	for(int i = 0; i + 1 < N; i++) {
		if(
				(loc[i + 1][0] - loc[i][0] + N) % N != 
				(loc[i + 1][1] - loc[i][1] + N) % N
		  ) {
			cout << "NO\n";
			return 0;
		}
	}
	cout << "YES\n";
	return 0;
}
