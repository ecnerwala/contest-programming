#include "biscuits.h"
#include <cassert>
#include <cstdio>

using namespace std;

int main() {
	int q;
	assert(scanf("%d", &q) == 1);
	vector<int> k(q);
	vector<long long> x(q);
	vector<vector<long long>> a(q);
	vector<long long> results(q);
	for (int t = 0; t < q; t++) {
		assert(scanf("%d%lld", &k[t], &x[t]) == 2);
		a[t] = vector<long long>(k[t]);
		for (int i = 0; i < k[t]; i++) {
			assert(scanf("%lld", &a[t][i]) == 1);
		}
	}
	fclose(stdin);

	for (int t = 0; t < q; t++) {
		results[t] = count_tastiness(x[t], a[t]);
	}
	for (int t = 0; t < q; t++) {
		printf("%lld\n", results[t]);
	}
	fclose(stdout);
	return 0;
}

