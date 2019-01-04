#include<iostream>
#include<cstring>
#include<vector>
#include<deque>
#include<queue>
#include<algorithm>
#include<cassert>

using namespace std;

typedef long long ll;
typedef pair<int, int> pii;

#define A first
#define B second
#define X first
#define Y second

const int MAXN = 2000000;

int N;
pii T[MAXN];

int go() {
	priority_queue<pii> pq;
	int ans = 0;

	int curDate = -1;

	for(int i = 0; i < N; i++) {
		pq.push(pii(T[i].X, i));

		while(true) {
			assert(!pq.empty());
			if(pq.top().B <= curDate) pq.pop();
			else if(pq.top().A > T[i].Y) {
				curDate = pq.top().B;
				pq.pop();
			} else break;
		}

		int val = i - curDate;
		if(val > ans) ans = val;
	}

	return ans;
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N;
	for(int i = 0; i < N; i++) cin >> T[i].X >> T[i].Y;

	cout << go() << '\n';
	return 0;
}
