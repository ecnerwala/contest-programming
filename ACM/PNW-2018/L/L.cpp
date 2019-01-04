#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4000;
int N;
int A[MAXN];
int B[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for (int i = 0; i < N; i++) cin >> A[i] >> B[i];

	for (int v = N; v >= 0; v--) {
		int numTrue = 0;
		for (int i = 0; i < N; i++) {
			numTrue += (A[i] <= v && v <= B[i]);
		}
		if (numTrue == v) {
			cout << v << '\n';
			return 0;
		}
	}
	cout << -1 << '\n';
	return 0;
}
