#include "job.h"
#include <cstdio>
#include <cassert>

using namespace std;

int main() {
	int n;
	assert(1 == scanf("%d", &n));
	vector<int> p(n);
	for (int i = 0; i < n; i++)
		assert(1 == scanf("%d", &p[i]));
	vector<int> u(n);
	for (int i = 0; i < n; i++)
		assert(1 == scanf("%d", &u[i]));
	vector<int> d(n);
	for (int i = 0; i < n; i++)
		assert(1 == scanf("%d", &d[i]));
	fclose(stdin);

	long long result = scheduling_cost(p, u, d);

	printf("%lld\n", result);
	fclose(stdout);
	return 0;
}
