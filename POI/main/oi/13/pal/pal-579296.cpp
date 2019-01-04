#include<bits/stdc++.h>

using namespace std;

const int MAXN = 2e6 + 20;
const int D = 26;

int N;

int P[MAXN];
int vals[D];

int buckets[MAXN];
int nxt[MAXN];

int L[MAXN];
char *(S[MAXN]);

int V; // depth
bool cnt[MAXN];
char path[MAXN];

int pal[MAXN * 2];

char tmp[MAXN]; // true storage location for the strings

int main() {
	//clock_t start = clock();
	N = 1;
	scanf("%d", &N);

	int maxLen = 0;
	char *curPos = tmp;
	memset(buckets, -1, sizeof(buckets));
	for(int i = 0; i < N; i++) {
		scanf("%d %s", &L[i], curPos);
		S[i] = curPos;
		curPos += L[i];
		nxt[i] = buckets[L[i]];
		buckets[L[i]] = i;
		if(L[i] > maxLen) maxLen = L[i];
	}
	//cerr << clock() - start << '\n';
	//start = clock();

	if(true) {
		int sz = 0;
		for(int i = 1; i <= maxLen; i++) {
			for(int j = buckets[i]; j != -1; j = nxt[j]) {
				P[sz++] = j;
			}
		}
	}
	//cerr << clock() - start << '\n';
	//start = clock();

	for(int p = maxLen - 1; p >= 0; p--) {
		memset(vals, -1, sizeof(vals));
		int sz = N - 1;
		while(sz >= 0 && L[P[sz]] > p) {
			int v = S[P[sz]][p] - 'a';
			nxt[P[sz]] = vals[v];
			vals[v] = P[sz];

			sz--;
		}
		sz++;
		for(int c = 0; c < D; c++) {
			for(int i = vals[c]; i != -1; i = nxt[i]) {
				P[sz++] = i;
			}
		}
	}

	//cerr << clock() - start << '\n';
	//start = clock();

	long long res = N;

	V = 0;
	for(int i = 0; i < N; i++) {
		char *s = S[P[i]];
		//cerr << s << '\n';
		int len = L[P[i]];

		//Manacher's
		int c = -1;
		int r = -1;
		for(int i = 0; i <= len - 1; i++) {
			if(i > r) {
				pal[i] = 0;
			} else {
				pal[i] = min(pal[2 * c - i], r - i);
			}
			if(i + pal[i] >= r) {
				c = i;
				while(pal[i] < i &&
					((i + pal[i] + 1) % 2 == 1 || s[(i + (pal[i] + 1)) / 2] == s[(i - (pal[i] + 1)) / 2])) {
					pal[i] ++;
				}
				r = i + pal[i];
			}
			//cerr << i << ' ' << pal[i] << '\n';
		}

		for(int j = 0; j < len; j++) {
			if(j < V && s[j] != path[j]) {
				V = j;
			}

			if(V == j) {
				cnt[j] = 0;
				path[j] = s[j];
				V++;
			} else {
				if(cnt[j] && pal[len - j - 2] == (len - j - 2)) {
					res+=2;
				}
			}

		}
		assert(V == len);
		cnt[V - 1] ++;
	}

	//cerr << clock() - start << '\n';

	printf("%lld\n", res);
	return 0;

}
