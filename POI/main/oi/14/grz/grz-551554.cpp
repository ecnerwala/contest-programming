#include<iostream>
#include<cstring>
#include<queue>

using namespace std;

const int MAXN = 2e3;

int N;
int H[MAXN][MAXN];

bool vis[MAXN][MAXN];

inline bool isGood(int v) {
	return v >= 0 && v < N;
}

typedef pair<int, int> pii;

bool bfsMax(int x, int y) {
	queue<pii> q;
	q.push(pii(x, y));
	bool good = true;
	while(!q.empty() ) {
		x = q.front().first;
		y = q.front().second;
		q.pop();
		if(vis[x][y]) continue;
		vis[x][y] = true;
		for(int i = -1; i <= 1; i++) {
			for(int j = -1; j <= 1; j++) {
				if(i || j) {
					if(isGood(x + i) && isGood(y + j)) {
						if(H[x + i][y + j] > H[x][y]) good = false;
						else if(H[x + i][y + j] == H[x][y]) {
							q.push(pii(x + i, y + j));
						} else {
							// do nothing
						}
					}
				}
			}
		}
	}
	return good;
}

bool dfsMax(int x, int y) {
	if(vis[x][y]) return true;
	vis[x][y] = true;
	bool good = true;
	for(int i = -1; i <= 1; i++) {
		for(int j = -1; j <= 1; j++) {
			if(i || j) {
				if(isGood(x + i) && isGood(y + j)) {
					if(H[x + i][y + j] > H[x][y]) good = false;
					else if(H[x + i][y + j] == H[x][y]) {
						if(!dfsMax(x + i, y + j)) good = false;
					} else {
						// do nothing
					}
				}
			}
		}
	}
	return good;
}

int goMax() {
	int res = 0;
	memset(vis, 0, sizeof(vis));
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			if(!vis[i][j]) {
				if(bfsMax(i, j)) res++;
			}
		}
	}
	return res;
}

int goMin() {
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) {
			H[i][j] *= -1;
		}
	}
	return goMax();
}
	

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < N; j++) cin >> H[i][j];
	}

	cout << goMax() << ' ';
	cout << goMin() << '\n';
}
