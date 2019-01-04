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

int main() {
	ios_base::sync_with_stdio(0);
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
		/*
		cerr << p << '\n';
		for(int i = 0; i < N; i++) cerr << S[P[i]] << '\n';
		cerr << '\n';
		*/
	}

	long long res = N;

	V = 0;
	for(int i = 0; i < N; i++) {
		string s = S[P[i]];
		//cerr << s << '\n';
		for(int j = 0; j < int(s.size()); j++) {
			if(j < V && s[j] != path[j]) {
				V = j;
			}

			if(V == j) {
				cnt[j] = false;
				path[j] = s[j];
				V++;
			} else {
				if(cnt[j]) res += 2;
			}

		}
		cnt[V - 1] ++;
	}

	cout << res << '\n';
	return 0;

}
