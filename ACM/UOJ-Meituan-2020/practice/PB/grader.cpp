/* This is sample grader for the contestant */
#include "find.h"
#include <stdio.h>
#include <assert.h>
#include <cstring>
#include <algorithm>

#define N 110000

static int A[N];
static int tot_query;
static int n, x;

int query(int i) {
	assert(1 <= i && i <= n);
	++tot_query;
	return A[i];
}

int main()
{
	scanf("%d%d", &n, &x);
	for (int i = 1; i <= n; ++i) scanf("%d", &A[i]);

	tot_query = 0;
	int result = find_pos(n, x);
	printf("%d %d\n", result, tot_query);
	return 0;
}
