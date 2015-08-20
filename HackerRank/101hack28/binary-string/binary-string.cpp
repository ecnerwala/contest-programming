#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int N;
int K, T;

string S;
queue<int> locs[MAXN]; // stores indices of 1's

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> K >> T;
	cin >> S;
	for(int i = 0; i < N; i++) {
		if(S[i] == '1') locs[i % K].push(i);
	}
	for(int i = 0; i < N; i++) {
		if(locs[i % K].empty()) {
			cout << '0';
			continue;
		}
		int v = locs[i % K].front();
		assert(v % K == i % K);
		assert(v >= i);
		int d = (v - i) / K;

		if(d <= T) {
			T -= d;
			cout << '1';
			locs[i % K].pop();
		} else {
			cout << '0';
		}
	}
	cout << '\n';
	return 0;
}
