#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;
#define A first
#define B second

class MultiplicationTable2 {

public:

	static const int MAXN = 60;
	int N;
	int v[MAXN][MAXN];
	bool used[MAXN];

	int minimalGoodSet(vector <int> table) {
		N = 0;
		while(N * N < int(table.size())) N++;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				v[i][j] = table[i * N + j];
			}
		}
		int res = N;
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				used[j] = false;
			}
			int cnt = 0;
			queue<int> q;
			q.push(i);
			used[i] = true;
			while(!q.empty()) {
				int cur = q.front(); q.pop();
				assert(used[cur]);
				cnt++;
				for(int j = 0; j < N; j++) {
					if(used[j]) {
						if(!used[v[cur][j]]) {
							used[v[cur][j]] = true, q.push(v[cur][j]);
						}
						if(!used[v[j][cur]]) {
							used[v[j][cur]] = true, q.push(v[j][cur]);
						}
					}
				}
			}
			res = min(res, cnt);
		}
		return res;
	}
};

// vim:ft=cpp
