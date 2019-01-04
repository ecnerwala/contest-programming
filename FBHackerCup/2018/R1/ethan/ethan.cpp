#include<bits/stdc++.h>
using namespace std;

const int MAXN = 4e3;
int N;
int K;
int ch[MAXN][2];

vector<int> preorder;
vector<int> postorder;

void dfs(int cur) {
	if (cur == 0) return;
	preorder.push_back(cur);
	dfs(ch[cur][0]);
	dfs(ch[cur][1]);
	postorder.push_back(cur);
}

int par[MAXN];
void reset() {
	for (int i = 1; i <= N; i++) par[i] = i;
}
int getpar(int a) {
	return (par[a] == a) ? a : (par[a] = getpar(par[a]));
}
bool merge(int a, int b) {
	assert(1 <= a && a <= N);
	assert(1 <= b && b <= N);
	a = getpar(a);
	b = getpar(b);
	if (a == b) {
		return false;
	}
	par[a] = b;
	return true;
}

int L[MAXN];

bool go() {
	reset();

	preorder.clear();
	postorder.clear();
	dfs(1);
	assert(int(preorder.size()) == N);
	assert(int(postorder.size()) == N);
	for (int i = 0; i < N; i++) {
		merge(preorder[i], postorder[i]);
	}

	int c = 0;
	for (int i = 1; i <= N; i++) {
		if (par[i] == i) {
			L[i] = c++;
		}
	}
	if (c < K) return false;

	for (int i = 1; i <= N; i++) {
		L[i] = L[getpar(i)];
	}

	for (int i = 1; i <= N; i++) {
		L[i] = (L[i] % K) + 1;
	}

	for (int i = 0; i < N; i++) {
		assert(L[preorder[i]] == L[postorder[i]]);
	}

	for (int i = 1; i <= N; i++) {
		assert(1 <= L[i] && L[i] <= K);
	}
	unordered_set<int> used;
	for (int i = 1; i <= N; i++) {
		used.insert(L[i]);
	}
	assert(int(used.size()) == K);
	return true;
}

int main() {
	int NUM_CASES; cin >> NUM_CASES;
	for (int case_num = 1; case_num <= NUM_CASES; case_num ++) {
		cin >> N >> K;
		assert(1 <= N && N < MAXN);
		assert(1 <= K && K <= N);
		for (int i = 1; i <= N; i++) {
			cin >> ch[i][0] >> ch[i][1];
			assert(0 <= ch[i][0] && ch[i][0] <= N);
			assert(0 <= ch[i][1] && ch[i][1] <= N);
		}
		bool res = go();
		cout << "Case #" << case_num << ":";
		if (res) {
			for (int i = 1; i <= N; i++) {
				cout << ' ' << L[i];
			}
			cout << '\n';
		} else {
			cout << ' ' << "Impossible" << '\n';
		}
	}

	return 0;
}
