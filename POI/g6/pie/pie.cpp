#include<bits/stdc++.h>
using namespace std;

const int MAXN = 200, MAXM = 200;
int N, M;
char F[MAXN][MAXM];
int P;
const int MAXR = 20, MAXC = 20;
int R, C;
char S[MAXR][MAXC];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < N; i++) {
		cin >> F[i];
	}
	cin >> P;
	while(P--) {
		cin >> R >> C;
		for(int i = 0; i < R; i++) {
			cin >> S[i];
		}
		int minX = R;
		int maxX = -1;
		int minY = C;
		int maxY = -1;
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C; j++) {
				if (S[i][j] == '.') continue;
				minX = min(minX, i);
				maxX = max(maxX, i);
				minY = min(minY, j);
				maxY = max(maxY, j);
			}
		}
		if (maxX == -1) continue;

		for(int dx = -minX; dx < N - maxX; dx++) {
			for(int dy = -minY; dy < M - maxY; dy++) {
				for(int i = minX; i <= maxX; i++) {
					for(int j = minY; j <= maxY; j++) {
						if(S[i][j] == '.') continue;
						if(F[dx + i][dy + j] == '.') {
							goto notgood;
						}
					}
				}
				for(int i = minX; i <= maxX; i++) {
					for(int j = minY; j <= maxY; j++) {
						if(S[i][j] == '.') continue;
						F[dx + i][dy + j] = 'x';
					}
				}
notgood:;
			}
		}
	}
	bool good = true;
	for(int i = 0; i < N; i++) {
		for(int j = 0; j < M; j++) {
			if(F[i][j] == '#') good = false;
		}
	}
	cout << (good ? "TAK" : "NIE") << '\n';
	return 0;
}
