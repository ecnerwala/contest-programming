#include<bits/stdc++.h>
using namespace std;

const int N = 2e5;
const int MAXC = 1e9;
vector<int> C;
const int K = 2e5;
const int MAXB = 1e9;
vector<int> B;

const int L = 15;

int main() {
	for(int i = 0, v = MAXB / 2; i < L; i++, v /= 2) {
		assert(1 <= v && v <= MAXC);
		C.push_back(v);
	}

	for(int m = 1; m < (1 << L); m++) {
		int v = C.back() - 1;
		for(int i = 0; i < L; i++) {
			if(m & (1 << i)) {
				v += C[i];
			}
		}
		assert(1 <= v && v <= MAXB);
		B.push_back(v);
	}

	for(int i = 0; i < N; i++) {
		C.push_back(1);
		C.push_back(MAXC);
	}

	assert(C.size() >= N);
	cout << N << '\n';
	for(int i = 0; i < N; i++) {
		cout << C[i] << ' ' << (N - i) << '\n';
	}
	cout << K << '\n';
	for(int i = 0; i < K; i++) {
		if(i) cout << ' ';
		cout << min(max(B[i % int(B.size())] - i / int(B.size()), 1), MAXB);
	}
	cout << '\n';
}
