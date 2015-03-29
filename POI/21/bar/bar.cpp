#include<bits/stdc++.h>
using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e6;

int N;

char A[MAXN];

vector<pii> stq;
vector<pii> enq;

int main() {
	scanf("%d", &N);
	scanf("%s", A);

	int pref = 0;

	stq.push_back(pii(0, -1));

	enq.push_back(pii(MAXN, -2));
	enq.push_back(pii(0, -1));

	int res = 0;

	for(int i = 0; i < N; i++) {
		if(A[i] == 'j') pref --;
		else pref ++;

		while(!enq.empty() && pref >= enq.back().A) {
			enq.pop_back();
		}

		while(!stq.empty() && pref < stq.back().A) {
			stq.pop_back();
		}

		assert(!enq.empty());

		int mi = -1, ma = int(stq.size());
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			assert(stq[md].B != enq.back().B);
			if(stq[md].B > enq.back().B) {
				ma = md;
			} else {
				mi = md;
			}
		}

		stq.push_back(pii(pref, i));
		enq.push_back(pii(pref, i));

		assert(ma < int(stq.size()));
		res = max(res, i - stq[ma].B);
	}

	cout << res << '\n';
	return 0;
}
