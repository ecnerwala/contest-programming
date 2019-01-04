#include<iostream>
#include<vector>
#include<queue>

using namespace std;

#define ITER(i, v) for(__typeof((v).begin()) i = (v).begin(); i != (v).end(); i++) 

const int MAXM = 1e6 + 20;
const int MAXN = 1e6 + 20;
const int MAXV = 1e6 + 20;

int M;
int A[MAXM];

int N;
vector<int> B[MAXN];
int pos[MAXN];

vector<int> cnd[MAXV];

int main() {
	ios_base::sync_with_stdio(0);
	cin >> M;
	for(int i = 0; i < M; i++) {
		cin >> A[i];
	}

	cin >> N;
	for(int i = 0; i < N; i++) {
		int s; cin >> s;
		B[i].resize(s);
		for(int j = 0; j < s; j++) {
			cin >> B[i][j];
		}
		pos[i] = 0;
		cnd[B[i][pos[i]]].push_back(i);
	}

	for(int i = 0; i < M; i++) {
		int v = A[i];
		vector<int> val = cnd[v];
		cnd[v].clear();
		ITER(it, val) {
			int j = *it;
			pos[j] ++;
			if(pos[j] < B[j].size()) cnd[B[j][pos[j]]].push_back(j);
		}
	}

	for(int i = 0; i < N; i++) {
		cout << (pos[i] == B[i].size() ? "TAK" : "NIE") << '\n';
	}
	return 0;
}

