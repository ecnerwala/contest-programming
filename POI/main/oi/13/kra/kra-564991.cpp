#include<iostream>

using namespace std;

const int MAXN = 6e5;
const int MAXM = 6e5;

int N;
int R[MAXN];
int M;
int K[MAXN];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> N >> M;
	for(int i = 0; i < N; i++) cin >> R[i];
	for(int i = 0; i < M; i++) cin >> K[i];
	for(int i = 1; i < N; i++) {
		if(R[i - 1] < R[i]) R[i] = R[i - 1];
	}

	int depth = N;
	for(int i = 0; i < M && depth >= 0; i++) {
		int k = K[i];
		int mi = -1;
		int ma = depth;
		while(ma - mi > 1) {
			int md = (mi + ma) / 2;
			if(R[md] >= k) {
				mi = md;
			} else {
				ma = md;
			}
		}
		depth = mi;
	}
	cout << depth + 1 << '\n';
	return 0;
}
