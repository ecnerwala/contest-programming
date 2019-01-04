#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

const int MAXN = 5e5;
int N;
int A[MAXN];
int B[MAXN];

int main() {
	scanf("%d", &N);
	for(int i = 1; i <= N; i++) scanf("%d", &(A[i]));
	for(int i = 1; i <= N; i++) scanf("%d", &(B[i]));

	priority_queue<pair<int, int> > shipments;
	ll suma = 0, sumb = 0;
	for(int i = 1; i <= N; i++) {
		suma += A[i];
		shipments.push(make_pair(B[i], i));
		sumb += B[i];
		if(sumb > suma) {
			sumb -= shipments.top().first;
			shipments.pop();
		}
	}
	vector<int> res;
	while(!shipments.empty()) {
		res.push_back(shipments.top().second);
		shipments.pop();
	}
	sort(res.begin(), res.end());
	printf("%d\n", int(res.size()));
	if(!res.empty()) {
		printf("%d", res[0]);
		for(int i = 1; i < int(res.size()); i++) {
			printf(" %d", res[i]);
		}
	}
	printf("\n");

	return 0;
}
