#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1.5e6;
const int MAXL = 30;

const int L = 26;

int N;

char S[MAXN];
char T[MAXN];

int P[MAXN];

int bit[MAXN];

int query(int a) {
	int res = 0;
	for(int i = a; i > 0; i -= i & (-i)) {
		res += bit[i];
	}
	return res;
}

void insert(int a) {
	a++;
	for(int i = a; i <= N; i += i & (-i)) {
		bit[i] ++;
	}
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N;
	cin >> S >> T;

	for(char c = 'A'; c <= 'Z'; c++) {
		for(int i = 0, j = 0; i < N && j < N; ) {
			if(S[i] != c) i++;
			else if(T[j] != c) j++;
			else P[i] = j, i++, j++;
		}
	}
	
	long long res = 0;

	for(int i = 0; i < N; i++) {
		res += i - query(P[i]);
		insert(P[i]);
	}

	cout << res << '\n';

	return 0;
}
