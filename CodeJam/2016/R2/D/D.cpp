#include<bits/stdc++.h>
using namespace std;

const int MAXN = 30;

int N;
bool op[MAXN][MAXN];

bool tmp[MAXN][MAXN];

bool check() {
	vector<int> p;
	for(int i = 0; i < N; i++) {
		p.push_back(i);
	}
	do {
		vector<int> q;
		for(int i = 0; i < N; i++) {
			q.push_back(i);
		}
		do {
			// does there exist some sequence of workers/things which blocks it?
			for(int i = 0; i < N; i++) {
				int job = 0;
				for(int j = 0; j < N; j++) job += tmp[p[i]][j];
				for(int j = 0; j < i; j++) job -= tmp[p[i]][q[j]];
				if(!job) {
					return false;
				}
				if(!tmp[p[i]][q[i]]) {
					break;
				}
			}
		} while (next_permutation(q.begin(), q.end()));
	} while (next_permutation(p.begin(), p.end()));
	return true;
}

int dsum(int i) { return i ? 1 + dsum(i & (i - 1)) : 0; }

int go() {
	int res = N * N;
	int v = 0;
	for(int i = 0; i < N * N; i++) v += (int(op[i / N][i % N])) << i;
	for(int m = 0; m < (1 << (N * N)); m++) {
		if((m & v) != v) continue;
		for(int i = 0; i < N * N; i++) tmp[i / N][i % N] = m & (1 << i);
		if(check()) {
			cerr << m << ' ' << v << ' ' << '\n';
			for(int i = 0; i < N; i++) {
				for(int j = 0; j < N; j++) {
					cerr << (tmp[i][j] ? '1' : '0');
				}
				cerr << '\n';
			}
			cerr << '\n';
			res = min(res, dsum(m ^ v));
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	int T; cin >> T;

	for(int case_num = 1; case_num <= T; case_num ++) {
		cin >> N;
		for(int i = 0; i < N; i++) {
			string s; cin >> s;
			for(int j = 0; j < N; j++) {
				op[i][j] = (s[j] == '1');
			}
		}
		
		int res = go();
		cout << "Case #" << case_num << ": " << res << '\n';
	}

	return 0;
}
