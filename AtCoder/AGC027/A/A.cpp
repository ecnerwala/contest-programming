#include<bits/stdc++.h>
using namespace std;

using ll = long long;

const int MAXN = 200;
int N;
ll X;
ll A[MAXN];

int main() {
	cin >> N >> X;
	for (int i = 0; i < N; i++) {
		cin >> A[i+1];
	}
	sort(A + 1, A + N + 1);
	A[0] = 0;
	for (int i = 0; i < N; i++) {
		A[i+1] += A[i];
	}
	if (A[N] == X) {
		cout << N << '\n';
	} else {
		int res = 0;
		while (res < N-1 && A[res+1] <= X) {
			res++;
		}
		cout << res << '\n';
	}

	return 0;
}
