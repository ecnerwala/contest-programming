#define NDEBUG
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef pair<int, int> pii;

const int MAXN = 3e5;
int N;
pii C[MAXN]; // second matters
ll S[MAXN];
const int MAXL = 20;
const int L = 19;
int nxt[MAXN][MAXL];

const int MAXK = 3e5;
int K;
pii B[MAXN]; // first matters
int res[MAXN];

void dfs(pii* l, pii* r, int v, int i) {
	//cerr << l->first << ' ' << (r-1)->first << ' ' << v << ' ' << i << '\n';
	assert(l < r);
	assert(i);
	assert(l->first >= v);
	ll u = v + C[i].second;
	pii* m = ((r-1)->first < u) ? r : lower_bound(l, r, pii(u, -1));
	if(l < m) {
		int t = i;
		//while((r-1)->first < v + C[nxt[i][j]].second) j++;
		for(int j = L; j-->0; ) {
			if((m-1)->first < v + C[nxt[t][j]].second) t = nxt[t][j];
		}
		if(nxt[t][0]) dfs(l, r, v, nxt[t][0]);
	}
	res[m - B] ++;
	res[r - B] --;
	if(m < r && i-1) {
		while(m < r && m->first == u) m++;
		int t = i-1;
		u += S[t];
		assert(m->first >= u - S[t]);
		if(m->first >= u) { // S[0] == 0
			res[m - B] += t;
			res[r - B] -= t;
			return;
		}
		for(int j = L; j-->0; ) {
			if(t > (1 << j) && m->first >= u - S[t - (1 << j)]) {
				t -= (1 << j);
				res[m - B] += (1 << j);
				res[r - B] -= (1 << j);
			}
		}
		assert(t);
		dfs(m, r, u - S[t], t);
	}
}

int main() {
	scanf("%d", &N);
	for(int i = 0; i < N; i++) {
		scanf("%d %d", &C[i].second, &C[i].first);
		C[i].first *= -1;
	}
	sort(C, C + N);
	reverse(C, C + N + 1);

	stack<int> st;
	st.push(0);
	for(int i = 1; i <= N; i++) {
		while(C[st.top()].second >= C[i].second) st.pop();
		nxt[i][0] = st.top();
		for(int j = 0; nxt[i][j]; j++) {
			nxt[i][j + 1] = nxt[nxt[i][j]][j];
		}
		st.push(i);
	}
	for(int i = 1; i <= N; i++) {
		S[i] = S[i - 1] + C[i].second;
	}

	//for(int i = N; i > 0; i--) { cerr << C[i].second << ' '; } cerr << '\n';
	//for(int i = N; i > 0; i--) { cerr << nxt[i][0] << ' '; } cerr << '\n';

	scanf("%d", &K);
	for(int i = 0; i < K; i++) {
		scanf("%d", &B[i].first);
		B[i].second = i;
	}
	sort(B, B + K);

	dfs(B, B + K, 0, N);

	for(int i = 0; i < K; i++) {
		res[i + 1] += res[i];
	}
	assert(res[K] == 0);
	for(int i = 0; i < K; i++) {
		B[B[i].second].first = res[i];
	}

	for(int i = 0; i < K; i++) {
		printf("%d\n", B[i].first);
	}
	return 0;
}
