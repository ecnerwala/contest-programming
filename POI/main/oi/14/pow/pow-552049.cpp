#include<iostream>
#include<cstring>
#include<algorithm>
#include<queue>
#include<cassert>

using namespace std;

typedef pair<int, int> pii;
typedef pair<int, pii> pip;

#define A first
#define B second

const int MAXN = 2e3;
const int MAXM = 2e3;

int N, M;
int H[MAXN][MAXM];
bool isLand[MAXN][MAXM];

bool drained[MAXN][MAXM];

inline bool isGood(int x, int y) {
	return x >= 0 && x < N && y >= 0 && y < M;
}

const int mx[] = {-1, 0, 1, 0}, my[] = {0, -1, 0, 1};

int go() {
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			if(H[i][j] > 0) {
				isLand[i][j] = true;
			} else {
				H[i][j] *= -1;
				isLand[i][j] = false;
			}
		}
	}

	priority_queue<pip> pq;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			if(isLand[i][j]) {
				pq.push(pip(-H[i][j], pii(-i - 1, -j - 1)));
			}
		}
	}
	
	memset(drained, 0, sizeof(drained));
	int res = 0;

	while(!pq.empty()) {
		int x = pq.top().B.A, y = pq.top().B.B;
		pq.pop();
		//cerr << "start\n";
		//cerr << x << ' ' << y << '\n';
		if(x < 0) {
			x = -(x + 1);
			y = -(y + 1);
			if(!drained[x][y]) {
				res++;
				drained[x][y] = true;
			} else {
				continue;
			}
		}
		for(int v = 0; v < 4; v++) {
			int nx = x + mx[v], ny = y + my[v];
			if(isGood(nx, ny)){
				//cerr << nx << ' ' << ny << '\n';
				if(!drained[nx][ny]) {
					drained[nx][ny] = true;
					if(H[nx][ny] < H[x][y]) H[nx][ny] = H[x][y];
					pq.push(pip(-H[nx][ny], pii(nx, ny)));
				}
			}
		}
	}
	return res;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < N; i++ ) {
		for(int j = 0; j < M; j++) {
			cin >> H[i][j];
		}
	}
	cout << go() << '\n';
	return 0;
}
