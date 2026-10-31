#include <string>
#include <vector>

#ifndef _H_LLMMATH_

#define _H_LLMMATH_

std::vector< std::pair< std::string, std::string > > generate_completions(std::string instruction, int max_tokens = 256, double temperature = 0.7, int n = 1, std::string prefix = "");

double score_rationale(std::string question, std::string str);

#endif