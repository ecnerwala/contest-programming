#include<bits/stdc++.h>
using namespace std;

const int MAXN = 2e5;
int N;
int K;
double D[MAXN];
int cnt[MAXN];

int main() {
	int last;
	int prv;
	scanf("%d %d %d %d", &N, &K, &last, &prv);
	N--;
	for(int i = 0; i < N; i++) {
		int cur;
		scanf("%d", &cur);
		D[i] = cur - prv;
		prv = cur;
	}

	priority_queue<pair<double, int> > pq;

	for(int i = 0; i < N; i++) {
		D[i] /= 2;
		D[i] *= D[i];

		pq.push(make_pair(D[i] / 2, i));
	}

	while(K--) {
		int i = pq.top().second;
		pq.pop();
		cnt[i] ++;
		pq.push(make_pair(D[i] / (cnt[i] + 1) / (cnt[i] + 2), i));
	}

	double res = 0;
	for(int i = 0; i < N; i++) {
		res += D[i] / (cnt[i] + 1);
	}
	printf("%.10f\n", res);

	return 0;
}
