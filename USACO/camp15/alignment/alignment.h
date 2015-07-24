#ifndef ALIGNMENT_H
#define ALIGNMENT_H

#include <vector>

std::vector<double> convolve(std::vector<double> X, std::vector<double> Y) {
  std::vector<double> Z(X.size() + Y.size() - 1);
  for (int i = 0; i < int(X.size()); i++) {
    for (int j = 0; j < int(Y.size()); j++) {
      Z[i + j] += X[i] * Y[j];
    }
  }
  return Z;
}

#endif
