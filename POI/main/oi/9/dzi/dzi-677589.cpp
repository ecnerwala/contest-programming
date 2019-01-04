#include<bits/stdc++.h>

using namespace std;

const int MAXN = 3e3;
int N;
bool B[MAXN][MAXN];

int V[MAXN];
int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			char c; scanf(" %c", &c);
			B[i][j] = c == '1';
		}
		B[i][N] = true;
	}

	int res = 0;
	for(int l = 0; l < N; l++) {
		stack<pair<int, int> > st;
		st.push(make_pair(-1, -1));
		for(int i = 0; i <= N; i++) {
			if(B[l][i]) V[i] = 0;
			else V[i]++;
			int v = V[i];
			while(st.top().first >= v) {
				int h = st.top().first;
				st.pop();
				assert(!st.empty());
				res = max(res, h * (i - 1 - st.top().second));
			}
			st.push(make_pair(v, i));
		}
	}
	printf("%d\n", res);
	return 0;
}
