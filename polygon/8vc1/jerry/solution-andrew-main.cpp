#include<bits/stdc++.h>
using namespace std;

typedef long double ld;

const int MAXN = 3e3;
const int MAXV = 2e4;

int N;
int V;
int A[MAXN];
ld diff[MAXV]; // prob of getting difference i
ld suff[MAXV]; // prob of getting difference at least i

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	V = 0;
	for(int i = 0; i < N; i++) {
		cin >> A[i];
		V = max(V, A[i]);
	}
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < i; j++) {
			assert(A[i] != A[j]);
			assert(abs(A[i] - A[j]) < V);
			diff[abs(A[i] - A[j])] ++;
		}
	}

	for(int i = 0; i <= V; i++) {
		diff[i] /= (N * (N - 1)) / 2; // this shouldn't overflow
	}

	suff[V + 1] = 0;
	for(int i = V; i >= 0; i--) {
		suff[i] = diff[i] + suff[i + 1];
	}

	ld res = 0;
	for(int i = 0; i <= V; i++) {
		for(int j = 0; j <= V; j++) {
			// A wins with margin i + j
			// B must win with margin > i + j
			int k = i + j + 1;
			if(k > V) continue;
			res += suff[k] * diff[i] * diff[j];
		}
	}
	cout << fixed << setprecision(10) << res << '\n';

	return 0;
}
