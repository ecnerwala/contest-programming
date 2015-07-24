#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int INF = 2e9;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 4e5;

int N;

int R;

pii P[MAXN];


const int MAXM = MAXN;

int M;
int pos[MAXN];
int val[MAXN];

const int S = 1 << 18;

int lazy[S * 2];
int maxval[S * 2];

void update(int a) {
	assert(a < S);
	int l = 2 * a, r = 2 * a + 1;

	maxval[a] = lazy[a] + max(maxval[l], maxval[r]);
}

int main() {
	cin >> N >> R;
	for(int i = 0; i < N; i++) {
		cin >> P[i].A >> P[i].B;
		P[i].A = max(P[i].A, 0);

		if(P[i].B < P[i].A) {
			N--;
			i--;
			continue;
		}

		P[i].B ++;
	}

	M = 0;
	val[M] = 0;
	pos[M] = -INF;
	M++;

	vector<pii> evts;
	for(int i = 0; i < N; i++) {
		evts.push_back(pii(P[i].A, P[i].B));
		evts.push_back(pii(P[i].B, P[i].A));
	}

	sort(evts.begin(), evts.end());

	int e = 0;

	priority_queue<int, vector<int>, greater<int> > pq;
	pq.push(0);

	for(int i = 0; i < N; i++) {
		pq.push(P[i].A);
		pq.push(P[i].B);
	}

	while(!pq.empty()) {
		int t = pq.top();
		while(!pq.empty() && pq.top() == t) {
			pq.pop();
		}

		while(e < int(evts.size()) && evts[e].A <= t) {
			assert(evts[e].A == t);

			if(evts[e].A < evts[e].B) {
				// insert
			} else if(evts[e].A > evts[e].B) {
				// delete
			} else assert(false);
		}

	}

	return 0;
}
