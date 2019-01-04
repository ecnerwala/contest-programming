#include<bits/stdc++.h>
using namespace std;

const int MAXN = int(5e5) + 200;
int N;
typedef pair<int, int> pii;
pii V[MAXN];

int main() {
	scanf("%d", &N);
	for (int i = 0; i < N; i++) {
		scanf("%d %d", &V[i].first, &V[i].second);
		V[i].second = V[i].first / V[i].second;
		assert(V[i].second < V[i].first);
	}
	sort(V, V + N);
	reverse(V, V + N);
	int res = 1;
	int kill = V[0].second;
	for (int i = 1; i < N; i++) {
		if (V[i].first <= kill) {
			kill = V[i].second;
			res++;
		} else {
			kill = max(kill, V[i].second);
		}
	}
	printf("%d\n", res);

	return 0;
}
