#include<bits/stdc++.h>

using namespace std;

const int MAXN = 2e6 + 20;
const int D = 26;

int N;

int P[MAXN];
vector<int> vals[D];

pair<int, int> L[MAXN];
string S[MAXN];

int V; // depth
bool cnt[MAXN];
char path[MAXN];

int pal[MAXN * 2];

int main() {
	ios_base::sync_with_stdio(0);
	N = 1;
	cin >> N;
	for(int i = 0; i < N; i++) {
		cin >> L[i].first >> S[i];
		L[i].second = i;
	}
	sort(L, L + N);
	for(int i = 0; i < N; i++) {
		P[i] = L[i].second;
	}
	for(int p = int(S[P[N - 1]].size()) - 1; p >= 0; p--) {
		//for(int i = 0; i < N; i++) cerr << S[P[i]] << '\n';
		//cerr << p << '\n';
		int sz = N - 1;
		while(sz >= 0 && int(S[P[sz]].size()) > p) {
			vals[S[P[sz]][p] - 'a'].push_back(P[sz]);
			sz--;
		}
		sz++;
		for(int c = 0; c < D; c++) {
			while(!vals[c].empty()) {
				P[sz++] = vals[c].back();
				vals[c].pop_back();
			}
		}
	}

	long long res = N;

	V = 0;
	for(int i = 0; i < N; i++) {
		string s = S[P[i]];
		//cerr << s << '\n';
		int len = s.size();

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

	cout << res << '\n';
	return 0;

}
