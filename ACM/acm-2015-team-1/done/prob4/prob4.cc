#include<bits/stdc++.h>
using namespace std;

const double INF = 1e9;

inline int sq(int a) { return a * a; }

const double EPS = 1e-5;

typedef pair<int, int> pii;
#define A first
#define B second

// WSEN-
const int mx[5] = {0, 1, 0, -1, 0}, my[5] = {-1, 0, 1, 0, 0};
const char dirs[5] = {'W', 'S', 'E', 'N', '.'};

const int MAXX = 110;
const int MAXY = 110;
int X, Y;
int S; // sigma

char grid[MAXX][MAXY];
int dist[MAXX][MAXY][MAXX][MAXY];
int normalization[MAXX][MAXY];

void bfs(int a, int b, int res[MAXX][MAXY]) {
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			res[i][j] = -1;
		}
	}
	queue<pii> q;
	res[a][b] = 0;
	q.push(pii(a, b));
	while(!q.empty()) {
		int x = q.front().A, y = q.front().B;
		q.pop();
		if(grid[x][y] == '#') continue;
		for(int d = 0; d < 4; d++) {
			int nx = x + mx[d], ny = y + my[d];
			if(res[nx][ny] != -1) continue;
			res[nx][ny] = res[x][y] + 1;
			q.push(pii(nx, ny));
		}
	}
}

double prob[MAXX][MAXY];
double temp[MAXX][MAXY];

void print() {
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			cerr << prob[i][j] << ' ';
		}
		cerr << '\n';
	}
	cerr << '\n';
}

void normalize() {
	double sum = 0;
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			sum += prob[i][j];
		}
	}
	assert(sum);
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			prob[i][j] /= sum;
		}
	}
}

void move() {
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			temp[i][j] = 0;
		}
	}
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			if(grid[i][j] == '#') continue;
			int cnt = 0;
			for(int d = 0; d < 4; d++) {
				int x = i + mx[d], y = j + my[d];
				if(grid[x][y] == '#') continue;
				cnt++;
			}
			if(!cnt) {
				temp[i][j] += prob[i][j];
			} else {
				for(int d = 0; d < 4; d++) {
					int x = i + mx[d], y = j + my[d];
					if(grid[x][y] == '#') continue;
					temp[x][y] += prob[i][j] / cnt;
				}
			}
		}
	}
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			prob[i][j] = temp[i][j];
		}
	}
	normalize();
}

void update(int a, int b) {
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			if(grid[i][j] == '#') continue;
			prob[i][j] *= exp(-0.5 * (sq(i - a) + sq(j - b)) / S / S) / normalization[i][j];
		}
	}
	normalize();
}

int hound(int a, int b) {
	double best = INF;
	int dir = -1;
	for(int d = 0; d < 5; d++) {
		int x = a + mx[d], y = b + my[d];
		if(grid[x][y] == '#') continue;
		double ev = 0;
		for(int i = 0; i < X; i++) {
			for(int j = 0; j < Y; j++) {
				ev += dist[x][y][i][j] * prob[i][j];
			}
		}
		if(ev < best + EPS) best = ev, dir = d;
	}
	assert(dir != -1);
	return dir;
}

int main() {
	ios_base::sync_with_stdio(0);
	cin >> Y >> X;
	for(int i = 0; i < X; i++) {
		cin >> grid[i];
	}
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			bfs(i, j, dist[i][j]);
		}
	}

	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			normalization[i][j] = 0;
			for(int a = 0; a < X; a++) {
				for(int b = 0; b < Y; b++) {
					if(grid[a][b] == '#') continue;
					normalization[i][j] += exp(-0.5 * (sq(i - a) + sq(j - b)) / S / S);
				}
			}
		}
	}

	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			if(grid[i][j] == '#') prob[i][j] = 0;
			else prob[i][j] = 1;
		}
	}
	normalize();
	//print();

	int a = 0, b = 0;
	for(int i = 0; i < X; i++) {
		for(int j = 0; j < Y; j++) {
			if(grid[i][j] == 'h') a = i, b = j;
		}
	}
	cin >> S;

	string s;
	getline(cin, s);
	while(getline(cin, s)) {

		move();
		//print();
		if(s[0] == 's') {
			// silence
			// do nothing
			//cerr << "silent\n";
		} else {
			stringstream str;
			str << s;
			int x, y; str >> y >> x;
			//cerr << x << ' ' << y << "\n";
			update(x, y);
			//print();
		}

		//cerr << "hound " << a << ' ' << b << '\n';
		int dir = hound(a, b);
		a += mx[dir], b += my[dir];
		cout << dirs[dir] << '\n';
	}

	return 0;
}
