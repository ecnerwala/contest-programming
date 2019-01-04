#include<iostream>
#include<algorithm>
#include<cstring>
#include<map>

using namespace std;

typedef pair<int, int> pii;

#define A first
#define B second

const int MAXN = 2e5;

int N;
int A[MAXN];

int prv[MAXN];

pii bit[MAXN];

void insert(int i, pii v) {
	i++;
	for(int x = i; x <= N; x += x & (-x)) {
		bit[x] = max(bit[x], v);
	}
}

pii query(int i) {
	i++;
	pii res = pii(0, -1);
	for(int x = i; x > 0; x -= x & (-x)) {
		res = max(res, bit[x]);
	}
	return res;
}

pii pts[MAXN];

int res;
int rem[MAXN];
void go() {
	for(int i = 1; i <= N; i++) {
		bit[i] = pii(0, -1);
	}
	for(int i = 0; i < N; i++) {
		pts[i] = pii(i - A[i], A[i]);
	}
	sort(pts, pts + N);

	for(int i = 0; i < N; i++) {
		//cerr << pts[i].A + pts[i].B << ' ' << pts[i].A << ' ' << pts[i].B << '\n';
		if(pts[i].A >= 0) {
			pii val = query(pts[i].B - 1);
			prv[pts[i].A + pts[i].B] = val.second;
			insert(pts[i].B, pii(val.first + 1, pts[i].A + pts[i].B));
		}
	}

	res = 0;

	int cur = query(N - 1).second;
	while(cur > 0) {
		int numRem = cur - A[cur];
		if(prv[cur] != -1) numRem -= prv[cur] - A[prv[cur]];
		for(int i = 1; i <= numRem; i++) {
			rem[res++] = cur - i;
		}
		cur = prv[cur];
	}
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 0; i < N; i++) cin >> A[i], A[i]--;

	go();

	cout << res << '\n';
	for(int i = res - 1; i >= 0; i--) {
		cout << (rem[i]+1) << (i ? ' ' : '\n');
	}
	return 0;
}
