#include<bits/stdc++.h>
using namespace std;

const int MAXN = 3000;
int N;
int W[MAXN];
int M;
int T;
int main() {
	scanf("%d", &N);
	char s[100];
	for (int i = 0; i < N; i++) {
		scanf(" %s", s);
		W[i] = int(strlen(s));
		assert(W[i]);
	}
	M = W[0];
	T = W[0];
	for (int i = 1; i < N; i++) {
		M = max(M, W[i]);
		T += W[i];
	}
	int res = 0, best = -1;
	for (int l = M; l <= T+(N-1); l++) {
		//cerr << l << '\n';
		vector<pair<int, int>> rivers;
		int i = 0;
		while (i < N) {
			auto it = rivers.begin();
			vector<pair<int, int>> nrivers;
			int cl = W[i];
			assert(cl <= l);
			//cerr << W[i] << ' ';
			i++;
			while (i < N && cl + 1+W[i] <= l) {
				//cerr << W[i] << ' ';
				while (it != rivers.end() && it->first < cl - 1) {
					it++;
				}
				int nv = 0;
				if (it != rivers.end() && it->first <= cl + 1) {
					nv = it->second;
					if (it->first == cl - 1) {
						it++;
						assert(it == rivers.end() || it->first >= cl + 1);
						if (it != rivers.end() && it->first <= cl + 1) {
							nv = max(nv, it->second);
						}
					}
				}
				nv ++;
				if (nv > res) {
					res = nv, best = l;
				}
				nrivers.emplace_back(cl, nv);
				cl += 1+W[i];
				i++;
			}
			//cerr << '\n';
			swap(rivers, nrivers);
			nrivers.clear();
		}
		//cerr << '\n';
	}
	assert(res);
	printf("%d %d\n", best, res);

	return 0;
}
