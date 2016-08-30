#include<bits/stdc++.h>
using namespace std;

inline void setmax(int &a, int b) { a += (b > a) * (b - a); }
inline void setmin(int &a, int b) { a -= (b < a) * (a - b); }

typedef pair<int, int> pii;

const int MAXW = 1050;
int WX, WY;
int PX, PY;
int KX, KY;
int N;

bool frog[MAXW][MAXW];

const int MAXV = MAXW * MAXW * 2 + 20;

//int sq[MAXV]; // square i, with rounding error (oops)

inline int sq(int a) { return a * a; }

const int INF = MAXV * 2;

int vdist[MAXW][MAXW]; // minimum vertical distance to the next frog

int cnt[MAXW][MAXW];

int avoid[MAXW];

const int mx[4] = {-1, 0, 1, 0};
const int my[4] = {0, -1, 0, 1};

// we'll use exclusive circles
bool can_avoid(int D) {
	for(int i = 0; i <= WX; i++) {
		int v = 0;
		while(v * v < D - i * i) v++;
		assert(v * v + i * i >= D);
		avoid[i] = v;
	}
	for(int i = 1; i <= WX; i++) {
		for(int j = 1; j <= WY+1; j++) {
			cnt[i][j] = 0;
		}
		for(int j = 1; j <= WY; j++) {
			if(vdist[i][j] == INF) continue;
			int w = avoid[vdist[i][j]];
			if(w) {
				cnt[i][max(j-w+1, 1)] ++;
				cnt[i][min(j+w  , WY+1)] --;
			}
		}
		for(int j = 1; j <= WY; j++) {
			cnt[i][j+1] += cnt[i][j];
		}
		assert(cnt[i][WY+1] == 0);
	}
	for(int i = 0; i <= WX + 1; i++) {
		cnt[i][0] = true;
		cnt[i][WY+1] = true;
	}
	for(int j = 0; j <= WY + 1; j++) {
		cnt[0][j] = true;
		cnt[WX+1][j] = true;
	}
	//cerr << D << '\n';
	//for(int i = 0; i <= WX+1; i++) { for(int j = 0; j <= WY+1; j++) cerr << cnt[i][j] << ' '; cerr << '\n'; } cerr << '\n';

	if(cnt[PX][PY]) return false;
	if(cnt[KX][KY]) return false;
	stack<pii> q;
	q.push(pii(PX, PY));
	cnt[PX][PY] = true;
	while(!q.empty()) {
		pii cur = q.top(); q.pop();
		if(cur == pii(KX, KY)) return true;
		int x = cur.first, y = cur.second;
		//cerr << x << ' ' << y << '\n';
		assert(cnt[x][y]);
		for(int d = 0; d < 4; d++) {
			int nx = x + mx[d], ny = y + my[d];
			//cerr << nx << ' ' << ny << '\n';
			if(!cnt[nx][ny]) {
				q.push(pii(nx, ny));
				cnt[nx][ny] = true;
			}
		}
		//cerr << '\n';
	}
	return false;
}

int main() {
	scanf("%d %d\n%d %d %d %d\n%d", &WX, &WY, &PX, &PY, &KX, &KY, &N);
	for(int i = N; i > 0; i--) {
		int x, y; scanf("%d %d", &x, &y);
		frog[x][y] = true;
	}

	for(int j = 1; j <= WY; j++) {
		for(int i = 1, v = INF; i <= WX; i++) {
			if(frog[i][j]) v = 0;
			else if(v < INF) v++;
			vdist[i][j] = v;
		}
		for(int i = WX, v = INF; i >= 1; i--) {
			if(frog[i][j]) v = 0;
			else if(v < INF) v++;
			setmin(vdist[i][j], v);
		}
	}
	//for(int i = 1; i <= WX; i++) { for(int j = 1; j <= WY; j++) { cerr << vdist[i][j] << ' '; } cerr << '\n'; } cerr << '\n';

	int mi = 0, ma = WX * WX + WY * WY;
	while(ma - mi > 1) {
		int md = (mi + ma) / 2;
		if(can_avoid(md)) {
			mi = md;
		} else {
			ma = md;
		}
	}

	printf("%d\n", mi);

	return 0;
}
