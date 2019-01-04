#include<iostream>
#include<cstring>
#include<algorithm>
#include<vector>
#include<queue>
#include<cassert>

using namespace std;

#define A first
#define B second

typedef long long ll;
typedef pair<int, int> pii;

const int MAXN = 600000;
const int MAXM = 600000;
const int MAXK = 600000;

int N, M, K;

int O[MAXM];
vector<int> S[MAXN];
ll P[MAXN];

int L[MAXK], R[MAXK];
ll V[MAXK];

int T;
int W[MAXN];

ll D[MAXM];

void go() {
	int numLeft = N;

	for(int i = 0; i < N; i++) W[i] = -1;

	for(T = 0; T * T < K; T++);

	memset(D, 0, sizeof(D));
	
	for(int a = 0; a * T < K; a++) {
		//cerr << a << '\n';

		ll num = 0;
		for(int b = 0; b < min(T, K - a * T); b++) {
			int l = L[a * T + b];
			int r = R[a * T + b];
			ll v = V[a * T + b];


			D[l] += v;
			D[r + 1] -= v;

			if(l > r) {
				num += v;
			}
			
		}

		vector<int> proc;

		for(int i = 0; i < M; i++) {
			num += D[i];
			D[i] = 0;// gotta reset it now

			assert(num >= 0);

			int n = O[i];
			if(W[n] == -1) {
				if(P[n] > 0 && P[n] <= num) proc.push_back(n);
				P[n] -= num;
			}
		}

		for(__typeof(proc.begin()) ni = proc.begin(); ni != proc.end(); ni++) {
			int n = *ni;//nation

			assert(W[n] == -1);

			//cerr << "processing " << n << " with a = " << a << " and P = " << P[n] << '\n';

			for(int b = min(T, K - a * T) - 1; b >= 0; b--) {
				int l = L[a * T + b];
				int r = R[a * T + b];
				ll v = V[a * T + b];
				
				for(__typeof((S[n]).begin()) si = S[n].begin(); si != S[n].end(); si++) {
					int s = *si;// sector

					if((l <= r && (l <= s && s <= r)) || (l > r && (s <= r || l <= s))) {
						P[n] += v;
					}

				}

				//cerr << "processing " << n << " with a = " << a << " b = " << b << " and P = " << P[n] << '\n';

				if(P[n] > 0) {
					W[n] = a * T + b;
					break;
				}
			}

			assert(P[n] > 0);

			P[n] = 0;
			numLeft --;
			if(numLeft == 0) return;
		}
	}
}

int main() {
	ios_base::sync_with_stdio(0);

	cin >> N >> M;

	for(int i = 0; i < M; i++) {
		cin >> O[i];
		O[i] --;
		S[O[i]].push_back(i);
	}

	for(int i = 0; i < N; i++) {
		cin >> P[i];
	}

	cin >> K;

	for(int i = 0; i < K; i++) {
		cin >> L[i] >> R[i] >> V[i];
		L[i]--, R[i]--;
	}

	go();

	for(int i = 0; i < N; i++) {
		if(W[i] == -1) cout << "NIE" << '\n';
		else cout << (W[i] + 1) << '\n';
	}
}
