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
		for(int dx = -R; dx < N; dx++) {
			for(int dy = -C; dy < M; dy++) {
				bool good = true;
				for(int i = 0; i < R; i++) {
					for(int j = 0; j < C; j++) {
						if(S[i][j] == '.') continue;
						if(!(0 <= dx + i && dx + i < N)) good = false;
						else if(!(0 <= dy + i && dy + j < M)) good = false;
						else if(F[dx + i][dy + j] == '.') good = false;
					}
				}
				if(!good) continue;
				for(int i = 0; i < R; i++) {
					if(!(0 <= dx + i && dx + i < N)) continue;
					for(int j = 0; j < C; j++) {
						if(!(0 <= dy + i && dy + j < M)) continue;
						if(S[i][j] == '.') continue;
						F[dx + i][dy + j] = 'x';
					}
				}
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
