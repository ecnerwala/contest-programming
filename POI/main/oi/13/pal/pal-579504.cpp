#include<bits/stdc++.h>

using namespace std;

const int MAXC = 2e6 + 20;
const int MAXN = 3e5; 
const int D = 26;

int N;

int P[MAXN];
int vsize[D];
int vals[D][MAXN];

pair<int, int> strlens[MAXN];

int le[MAXC];
int ri[MAXC];
int dig[MAXC];

int L[MAXN];
char *(S[MAXN]);

int V; // depth
bool cnt[MAXC];
char path[MAXC];

int pal[MAXC * 2];

char tmp[MAXC]; // true storage location for the strings

int main() {
	//clock_t start = clock();
	scanf("%d", &N);
	assert(N < MAXN);

	int maxLen = 0;
	char *curPos = tmp;
	for(int i = 0; i < N; i++) {
		scanf("%d %s", &L[i], curPos);
		S[i] = curPos;
		curPos += L[i];
		if(L[i] > maxLen) maxLen = L[i];
		strlens[i] = pair<int, int> (L[i], i);
	}

	//cerr << clock() - start << '\n';
	//start = clock();
	
	sort(strlens, strlens + N);
	for(int i = 0; i < N; i++) {
		P[i] = strlens[i].second;
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
		memset(vsize, 0, sizeof(vsize));
		for(int i = l; i < r; i++) {
			int c = S[P[i]][d] - 'a';
			vals[c][vsize[c]++] = P[i];
		}
		for(int c = 0; c < D; c++) {
			if(!vsize[c]) continue;
			if(vsize[c] > 1) {
				le[qsize] = l;
				ri[qsize] = l + vsize[c];
				dig[qsize] = d + 1;
				qsize ++;
			}
			for(int it = 0; it < vsize[c]; it++) {
				P[l++] = vals[c][it];
			}
		}
	}
	//cerr << clock() - start << '\n';
	//start = clock();
	//cerr << qsize << '\n';


	long long res = 0;

	V = 0;
	for(int i = 0; i < N; i++) {
		char *s = S[P[i]];
		//cerr << s << '\n';
		int len = L[P[i]];

		//Manacher's
		int c = -1;
		int r = -1;
		for(int k = 0; k < len - 1; k++) {
			if(k > r) {
				pal[k] = 0;
			} else {
				pal[k] = min(pal[2 * c - k], r - k);
			}
			if(k + pal[k] >= r) {
				c = k;
				r = k + pal[k];
				r /= 2;
				while(r < k && s[r + 1] == s[k - r - 1]) {
					r++;
				}
				if(r == k) {
					r *= 2;
					pal[k] = k;
				} else {
					r *= 2, r++;
					pal[k] = r - k;
				}
			}
		}

		if(i) {
			char *p = S[P[i - 1]];
			//memcpy(path, S[P[i - 1]], V);
			for(int j = 0; j < len; j++) {
				if(j == V || s[j] != p[j]) {
					memset(cnt + j, 0, len - j);
					break;
				}

				if(cnt[j] && pal[len - j - 2] == (len - j - 2)) {
					res++;
				}
			}
		}

		//assert(V == len);
		V = len;
		cnt[V - 1] = true;
	}

	res *= 2;
	res += N;

	//cerr << clock() - start << '\n';

	printf("%lld\n", res);
	return 0;

}
