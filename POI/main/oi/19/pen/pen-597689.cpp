#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 1.5e6;

int N;
int P[MAXN];
int Z[MAXN];

bool used[MAXN];

int sz[MAXN];
list<int> children[MAXN];

int dfssz(int a) {
	sz[a] = 1;
	for(__typeof(children[a].begin()) it = children[a].begin(); it != children[a].end(); it++) {
		int ch = *it;
		sz[a] += dfssz(ch);
	}
	return sz[a];
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	int root = -1;
	for(int i = 0; i < N; i++) {
		cin >> P[i] >> Z[i];
		P[i] --;
		if(P[i] == i) {
			root = i;
		} else {
			if(!Z[i]) {
				children[P[i]].push_back(i);
			}
		}
	}

	assert(root != -1);

	for(int i = 0; i < N; i++) {
		if(Z[i]) assert(Z[P[i]]);
	}

	if(!Z[root]) {
		Z[root] = N;
	}

	for(int i = 0; i < N; i++) {
		if(Z[i]) dfssz(i);
	}

	priority_queue<pii, vector<pii>, greater<pii> > proc;
	for(int i = 0; i < N; i++) {
		if(Z[i]) {
			proc.push(pii(Z[i], i));
		}
	}

	int s = 0;

	vector<int> vals;

	int t = 0; // last one printed
	while(!proc.empty()) {
		int v = proc.top().first;
		int cur = proc.top().second;
		proc.pop();


		while(t < v) {
			t++;
			vals.push_back(t);
		}

		s += sz[cur];

		//cerr << cur << ' ' << v << '\n';

		assert(vals.back() == v);
		vals.pop_back();

		s--;

		if(sz[cur] == 1) continue;

		if(s == int(vals.size())) {
			while(!vals.empty() && int(children[cur].size()) == 1) {
				cur = children[cur].back();
				Z[cur] = vals.back();
				vals.pop_back();
				s--;
			}
		}

		s -= vals.size();
		vals.clear();
	}

	for(int i = 0; i < N; i++) {
		cout << Z[i] << '\n';
	}
	return 0;
}
