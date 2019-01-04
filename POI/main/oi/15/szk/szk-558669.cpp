#include<iostream>
#include<vector>
#include<cstring>
#include<algorithm>

using namespace std;

typedef pair<int, int> pii;
#define A first
#define B second

const int MAXN = 2e5;
const int MAXX = 6e5;

const int MAXL = 6e5;

int N;
int X[MAXN];
int Y[MAXN];

int L;

pii pts[MAXL];

int par[MAXL];
int rank[MAXL];

void init() {
	memset(par, -1, sizeof(par));
	memset(rank, 0, sizeof(rank));
}

int getPar(int v) {
	if(par[v] == -1) return v;
	return par[v] = getPar(par[v]);
}

bool merge(int a, int b) {
	a = getPar(a), b = getPar(b);
	if(a == b) return false;
	if(rank[a] > rank[b]) swap(a, b);
	par[a] = b;
	if(rank[a] == rank[b]) rank[b] ++;
	return true;
}

int SX;
pair<pii, int> xpts[MAXL]; //const-x lines
int SY;
pair<pii, int> ypts[MAXL]; //const-y lines

void transform() {
	for(int i = 1; i <= N; i++) {
		int a = X[i] - Y[i], b = X[i] + Y[i];
		X[i] = a, Y[i] = b;
	}

	int minx = X[1], miny = Y[1];
	for(int i = 2; i <= N; i++) {
		minx = min(minx, X[i]);
		miny = min(miny, Y[i]);
	}

	for(int i = 1; i <= N; i++) {
		X[i] -= minx;
		Y[i] -= miny;
	}
}

pii corners[MAXN];
void go() {
	transform();
	X[0] = X[N];
	Y[0] = Y[N];
	X[N + 1] = X[1];
	Y[N + 1] = Y[1];

	SX = 0, SY = 0;
	for(int i = 1; i <= N; i++) {
		if((Y[i + 1] - Y[i]) * (Y[i - 1] - Y[i]) < 0) {
			ypts[SY++] = make_pair(pii(Y[i], X[i]), i - 1);
		} else {
			xpts[SX++] = make_pair(pii(X[i], Y[i]), i - 1);
		}
	}
	L = N;
	for(int i = 1; i <= N; i++) {
		int dx = (X[i + 1] > X[i]) ? 1 : -1;
		int dy = (Y[i + 1] > Y[i]) ? 1 : -1;
		for(int v = 1; v < abs(Y[i + 1] - Y[i]); v++) {
			int nx = X[i] + dx * v;
			int ny = Y[i] + dy * v;
			xpts[SX++] = make_pair(pii(nx, ny), L);
			ypts[SY++] = make_pair(pii(ny, nx), L);
			L++;
		}
	}

	
	init();
	//cerr << L << ' ' << SX << ' ' << SY << '\n';
	sort(xpts, xpts + SX);
	for(int i = 0; i < SX; i += 2) {
		merge(xpts[i].B, xpts[i + 1].B);
	}
	sort(ypts, ypts + SY);
	for(int i = 0; i < SY; i += 2) {
		merge(ypts[i].B, ypts[i + 1].B);
	}

	for(int i = 0; i < N; i++) {
		corners[i] = pii(getPar(i), i);
	}
	sort(corners, corners + N);
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N;
	for(int i = 1; i <= N; i++) cin >> X[i] >> Y[i];

	go();

	cout << N / 2 << '\n';

	for(int i = 0; i < N; i++) {
		cout << corners[i].B + 1 << ((i % 2) ? '\n': ' ');
	}
	return 0;
}
