#include<iostream>

using namespace std;

const int MAXK = 1e6 + 100;
int K;
char S[MAXK];
int F[MAXK];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> K >> S;
	F[0] = -1;
	F[1] = 0;
	for(int pos = 2, cnd = 0; pos <= K; ) {
		if(cnd < 0 || S[cnd] == S[pos - 1]) {
			F[pos] = cnd + 1;
			cnd++, pos++;
		} else {
			cnd = F[cnd];
		} 
	}
	for(int i = 1; i <= K; i++) {
		if(F[F[i]] > 0) F[i] = F[F[i]];
	}
	long long res = 0;
	for(int i = 1; i <= K; i++) {
		if(F[i] > 0) {
			res += i - F[i];
		}
	}
	cout << res << '\n';
}
