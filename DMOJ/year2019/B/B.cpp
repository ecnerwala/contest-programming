#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.1e6;
int N;
int M[MAXN];
int A[MAXN][2];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> N >> M[1];
	for (int i = 1; i <= N; i++) {
		cin >> A[i][0] >> A[i][1];
	}
	for (int i = 1; i <= N; i++) {
		cout << M[i]%2;
		M[A[i][0]] += (M[i] + 1) / 2;
		M[A[i][1]] += M[i] / 2;
	}
	cout << '\n';


	return 0;
}
