#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e6;
int N, K;

char S[MAXN];

int trans[MAXN][30];

int main() {
	cin >> N >> K;

	if(K == 1) {
		for(int i = 0; i < N; i++) {
			cout << '1';
		}
		cout << '\n';
	}

	memset(trans, 0, sizeof(trans));

	for(int i = 0; i < N; i++){
	}
	return 0;
}
