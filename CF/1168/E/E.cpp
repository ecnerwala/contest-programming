#include<bits/stdc++.h>
using namespace std;

const int MAXK = 13;
const int MAXN = 1 << MAXK;
int K, N;
int A[MAXN];

int main() {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> K; N = (1 << K);

	for (int i = 0; i < N; i++) {
		cin >> A[i];
	}

	{
		int x = 0;
		for (int i = 0; i < N; i++) {
			x ^= A[i];
		}
		if (x != 0) {
			cout << "Fou" << '\n';
			exit(0);
		}
	}

	return 0;
}
