#include "plants.h"
#include <cstdio>
#include <cassert>
#include <vector>

static int n, k, q;
static std::vector<int> r;
static std:: vector<int> x;
static std:: vector<int> y;
static std:: vector<int> answer;
int main() {
	assert(scanf("%d%d%d", &n, &k, &q) == 3);
	r.resize(n);
	answer.resize(q);
	for (int i = 0; i < n; i++) {
		int value;
		assert(scanf("%d", &value) == 1);
		r[i] = value;
	}
	x.resize(q);
	y.resize(q);
	for (int i = 0; i < q; i++) {
		assert(scanf("%d%d", &x[i], &y[i]) == 2);
	}
	fclose(stdin);

	init(k, r);
	for (int i = 0; i < q; i++) {
		answer[i] = compare_plants(x[i], y[i]);
	}

	for (int i = 0; i < q; i++) {
		printf("%d\n", answer[i]);
	}

	fclose(stdout);

	return 0;
}
