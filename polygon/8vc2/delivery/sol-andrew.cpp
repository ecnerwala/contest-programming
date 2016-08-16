#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXD = 1e9 + 200;
const int MAXN = MAXD;
const int MAXM = 3e5;

int D, N;
int M;

int main() {
	scanf("%d %d %d", &D, &N, &M);
	
	priority_queue<pair<int, int> > pq;
	pq.emplace(-D, 1);
	if(N < D) pq.emplace(-N, 0);

	for(int i = 0; i < M; i++) {
		int x, p;
		scanf("%d %d", &x, &p);
		pq.emplace(-x, p);
		if(x + N < D) pq.emplace(-(x + N), -p);
	}
	
	ll res = 0;

	priority_queue<int, vector<int>, greater<int> > vals;
	vals.push(0);
	priority_queue<int, vector<int>, greater<int> > rem;
	for(int x = 0; !pq.empty();) {
		int nx = -pq.top().first, np = pq.top().second;
		pq.pop();
		if(nx > x) {
			if(vals.empty()) {
				cout << -1 << '\n';
				return 0;
			}
			res += ll(vals.top()) * (nx - x);
			x = nx;
		}

		if(np > 0) vals.push(np);
		else rem.push(-np);
		while(!rem.empty()) {
			assert(!vals.empty());
			assert(vals.top() <= rem.top());
			if(vals.top() == rem.top()) vals.pop(), rem.pop();
			else break;
		}
	}

	cout << res << '\n';
	return 0;
}
