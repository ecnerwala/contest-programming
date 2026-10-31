#include "split.h"
#include <cstdio>
#include <cassert>

using namespace std;

int main() {
	int n, m, a, b, c;
	assert(5 == scanf("%d%d%d%d%d", &n, &m, &a, &b, &c));
	vector<int> p(m), q(m);
	for (int i=0; i<m; i++)
		assert(2 == scanf("%d%d", &p[i], &q[i]));
	fclose(stdin);

	vector<int> result = find_split(n, a, b, c, p, q);

	for (int i=0; i<(int)result.size(); i++)
		printf("%s%d", ((i>0)?" ":""), result[i]);
	printf("\n");
	fclose(stdout);
	return 0;
}
