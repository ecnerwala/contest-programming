#include "walk.h"
#include <cstdio>
#include <cassert>

using namespace std;

int main() {
	int n, m;
	assert(2 == scanf("%d%d", &n, &m));
	vector<int> x(n), h(n);
	for (int i = 0; i < n; i++)
		assert(2 == scanf("%d%d", &x[i], &h[i]));
	vector<int> l(m), r(m), y(m);
	for (int i = 0; i < m; i++)
		assert(3 == scanf("%d%d%d", &l[i], &r[i], &y[i]));
	int s, g;
	assert(2 == scanf("%d%d", &s, &g));
	fclose(stdin);

	long long result = min_distance(x, h, l, r, y, s, g);

	printf("%lld\n", result);
	fclose(stdout);
	return 0;
}
