#include<bits/stdc++.h>
using namespace std;

const int MAXN = 300;
int N;
int A[MAXN];

int cnt[5];

bool dp[MAXN][MAXN][MAXN];

int main() {

	scanf("%d", &N);

	memset(cnt, 0, sizeof(cnt));

	for(int i = 0; i < N; i++) {
	    char ch;
		scanf(" %c", &ch);
		if (ch == 'R') A[i] = 1;
		else if (ch == 'G') A[i] = 2;
		else if (ch == 'B') A[i] = 3;
		cnt[A[i]] ++;
	}

	dp[cnt[1]][cnt[2]][cnt[3]] = true;
	for(int l = N; l >= 2; l--) {
		for(int a = 0; a <= l; a++) {
			for(int b = 0; a + b <= l; b++) {
				int c = l - (a + b);
				if(dp[a][b][c]) {

					if(a >= 2) { dp[a - 1][b][c] = true; }
					if(b >= 2) { dp[a][b - 1][c] = true; }
					if(c >= 2) { dp[a][b][c - 1] = true; }

					if(a && b) { dp[a - 1][b - 1][c + 1] = true; }
					if(b && c) { dp[a + 1][b - 1][c - 1] = true; }
					if(a && c) { dp[a - 1][b + 1][c - 1] = true; }

				}
			}
		}
	}

	vector<char> res;
	if(dp[1][0][0]) res.push_back('R');
	if(dp[0][1][0]) res.push_back('G');
	if(dp[0][0][1]) res.push_back('B');
	sort (res.begin(), res.end());

	for(int i = 0; i < int(res.size()); i++) {
		cout << res[i];
	}
	cout << '\n';
}
