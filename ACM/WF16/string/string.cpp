#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200;

int N;
int A[MAXN];
int tot;

bool is_good(int k) {
	if(k == 1) {
		return tot == 2;
	}
	if(tot < k * (k + 1)) return false;
	if(tot % 2) return false;
	for(int z = 0; z < 2; z++, reverse(A, A + N)) {
		int ind = 0;
		int cur = A[0];
		for(int v = k; v; v--) {
			if(cur < v) return false;
			cur -= v;
			if(cur == 0) cur = A[++ind];
			assert(cur);
		}
	}
	return true;
}

int go() {
	for(int k = 150; k >= 1; k--) {
		if(is_good(k)) return k;
	}
	return 0;
}

int main() {
	cin >> N;
	for(int i = 0; i < N; i++) cin >> A[i], tot += A[i];
	int res = go();
	if(res == 0) cout << "no quotation" << '\n';
	else cout << res << '\n';
}
