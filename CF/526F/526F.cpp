#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4e5;

int N;
int P[MAXN];

int main() {
	cin >> N;
	for(int i = 0; i < N; i++) {
		int x, y;
		cin >> x >> y;
		x--, y--;
		P[x] = y;
	}

	for(int i = 0; i < N; i++) {
	}

	return 0;
}
