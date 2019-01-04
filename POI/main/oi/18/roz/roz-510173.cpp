#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<string>
#include<cassert>

using namespace std;

const int MAXN = 2000000;

const int L = 26;
int N;
string S;

template <class T> void setmax(T &a, T b) { if( b > a ) a = b; }

int go() {
	int ans = 0;
	for(int a = 0; a < L; a++) {
		int cnt[L];
		int maxDiff[L]; //num b - num a
		int nxtDiff[L];
		memset(cnt, 0, sizeof(cnt));
		memset(maxDiff, -1, sizeof(maxDiff));
		memset(nxtDiff, 0, sizeof(nxtDiff));
		for(int i = 0; i < N; i++) {
			int v = S[i] - 'a';
			if(v == a) {
				cnt[a] ++;
			} else {
				if(maxDiff[v] >= 0) setmax(ans, cnt[a] - cnt[v] + maxDiff[v]);

				cnt[v] ++;
				setmax(maxDiff[v], nxtDiff[v]);
				nxtDiff[v] = cnt[v] - cnt[a];
			}
		}

		for(int v = 0; v < L; v++) {
			if(maxDiff[v] >= 0) setmax(ans, cnt[a] - cnt[v] + maxDiff[v]);
		}
	}
	return ans;
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N;
	cin >> S;

	cout << go() << '\n';
}
