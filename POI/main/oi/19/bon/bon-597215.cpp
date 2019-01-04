#include<bits/stdc++.h>
using namespace std;

const int MAXM = 2e6;
const int MAXN = 2e6;

int M, N;
int B[MAXM];
int A[MAXN];

const int MAXV = 2e6;
const int V = 1e6 + 20;

bool vouch[MAXV];
int P[MAXV];
int S[MAXV]; // stores start position for each size

int main() {
	ios_base::sync_with_stdio(0);

	P[0] = -1;

	cin >> M;
	for(int i = 0; i < M; i++) {
		cin >> B[i];
		vouch[B[i]] = true;
	}

	cin >> N;
	long long c = 1; // current customer number
	vector<long long> res;
	for(int i = 1; i <= N; i++) {
		int a; cin >> a;
		for(int j = 0; S[a] <= V && j < a; S[a] += a) {
			if(!P[S[a]]) {
				P[S[a]] = i;

				if(vouch[S[a]]) res.push_back(c + j);

				j++;
			}
		}
		c += a;
	}

	cout << res.size() << '\n';
	for(int i = 0; i < int(res.size()); i++) {
		cout << res[i] << '\n';
	}
	return 0;
}
