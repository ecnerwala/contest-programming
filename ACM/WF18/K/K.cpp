#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e4;
int N;
int D[MAXN];
pair<int, int> keeps[MAXN];
const int MAXM = 2e5;
int M;

int main() {
	scanf("%d %d", &N, &M);
	assert(N >= 2);
	for (int i = 0; i < M; i++) {
		int u, v; scanf("%d %d", &u, &v);
		D[u] ++;
		D[v] ++;
	}

	assert(M >= N-1);

	for (int i = 0; i < N; i++) {
		keeps[i] = {D[i] - 1, i};
		D[i] = 0;
	}
	sort(keeps, keeps + N);

	int rem = N-2;
	for (int i = 0; i < N; i++) {
		if (keeps[i].first <= rem) {
			D[keeps[i].second] = 1 + keeps[i].first;
			rem -= keeps[i].first;
		} else {
			break;
		}
	}
	int res = 0;
	assert(rem >= 0);
	for (int i = 0; i < N; i++) {
		if (D[i] == 0) {
			res++;
			D[i] = 1 + rem;
			rem = 0;
		}
	}

	stack<int> leafs;
	stack<int> srcs;
	for (int i = 0; i < N; i++) {
		assert(D[i]);
		if (D[i] == 1) {
			leafs.push(i);
		} else {
			srcs.push(-1-i);
			for (int j = 1; j < D[i]; j++) {
				srcs.push(i);
			}
		}
	}

	printf("%d\n", res);
	printf("%d %d\n", N, N-1);
	for (int i = 0; i < N-1; i++) {
		while (!srcs.empty() && srcs.top() < 0) {
			leafs.push(-1-srcs.top());
			srcs.pop();
		}
		if (i == N-2) {
			assert(srcs.empty());
			assert(leafs.size() == 2);
			int a = leafs.top(); leafs.pop();
			int b = leafs.top(); leafs.pop();
			printf("%d %d\n", a, b);
		} else {
			assert(!srcs.empty());
			assert(!leafs.empty());
			int a = leafs.top(); leafs.pop();
			int b = srcs.top(); srcs.pop();
			printf("%d %d\n", a, b);
		}
	}
	
	return 0;
}
