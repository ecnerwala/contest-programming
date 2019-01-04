#include<bits/stdc++.h>

using namespace std;

const int MAXN = 2e6 + 20;
const int D = 26;

int N;

int P[MAXN];
vector<int> vals[D];

vector<int> buckets[MAXN];

int le[MAXN];
int ri[MAXN];
int dig[MAXN];

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
	for(int i = 0; i < N; i++) {
		scanf("%d %s", &L[i], curPos);
		S[i] = curPos;
		curPos += L[i];
		buckets[L[i]].push_back(i);
		if(L[i] > maxLen) maxLen = L[i];
	}

	//cerr << clock() - start << '\n';
	//start = clock();

	if(true) {
		int sz = 0;
		for(int i = 1; i <= maxLen; i++) {
			for(__typeof(buckets[i].begin()) j = buckets[i].begin(); j != buckets[i].end(); j++) {
				P[sz++] = *j;
			}
		}
	}
	//cerr << clock() - start << '\n';
	//start = clock();

	le[0] = 0;
	ri[0] = N;
	dig[0] = 0;
	int qsize = 1;
	for(int i = 0; i < qsize; i++) {
		int l = le[i], r = ri[i], d = dig[i];
		while(L[P[l]] == d && l < r) l++;
		if(r - l <= 1) continue;
		if(r - l == 2) {
			r--;
			char *a = S[P[l]], *b = S[P[r]];
			while(a[d] == b[d] && d < L[P[l]]) {
				d++;
			}
			if(a[d] > b[d] && d < L[P[l]]) {
				swap(P[l], P[r]);
			}
			continue;
		}
		for(int i = l; i < r; i++) {
			vals[S[P[i]][d] - 'a'].push_back(P[i]);
		}
		for(int c = 0; c < D; c++) {
			if(vals[c].empty()) continue;
			if(vals[c].size() > 1) {
				le[qsize] = l;
				ri[qsize] = l + vals[c].size();
				dig[qsize] = d + 1;
				qsize ++;
			}
			for(__typeof(vals[c].begin()) it = vals[c].begin(); it != vals[c].end(); it++) {
				P[l++] = *it;
			}
			vals[c].clear();
		}
	}
	//cerr << clock() - start << '\n';
	//start = clock();
	//cerr << qsize << '\n';


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
