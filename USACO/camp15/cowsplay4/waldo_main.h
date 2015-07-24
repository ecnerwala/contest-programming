#include <cstdio>
#include <vector>
#include <string>
#include <cassert>

using namespace std;

string send_picture(int n, int k, vector<int>& picture);
int find_waldo(int n, vector<string>& messages);

int main(int argc, char** args) {
  vector<string> MSG;
  double average_length = 0.0;
  int n, k;
  scanf("%d %d", &n, &k);
  for(int pic=0; pic<k; pic++) {
    int nc;
    scanf("%d", &nc);
    vector<int> C(nc, 0);
    for(int j=0; j<nc; j++) {
      scanf("%d", &C[j]);
    }
    string msg = send_picture(n, k, C);
    for(char c : msg) {
      assert(c == '0' || c == '1');
    }
    average_length += msg.size();
    MSG.push_back(msg);
  }
  average_length /= k;
  int score = max(0, 100 - int(average_length + 0.5));
  printf("%d %d\n", find_waldo(n, MSG), score);
}
