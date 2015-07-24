#include<bits/stdc++.h>

using namespace std;

int main() {
	srand(time(NULL));
	int N; cin >> N;
	int Q = 1000;
	cout << N << ' ' << Q << '\n';
	for(int i = 1; i < N; i++) {
		int l = rand() % i;
		int r = i;
		if(rand() % 2) {
			swap(l, r);
		}
		cout << l << ' ' << r << '\n';
	}

	for(int i = 0; i < Q; i++) {
		cout << rand() % N << ' ' << rand() % N << '\n';
	}

	return 0;
}
