/* Correct answer is "OK" */
 
#include <cassert>
 
int NumberOfOperations(long long i) {
  return 8;
}
 
int GetOperation(long long i, long long index) {
	int op1[] = {1,2,-2,3,2,-2,-1,-3};
	int op2[] = {1,2,3,-1,-3,3,-3,-2};
  return (i ? op2 : op1)[index];
}
