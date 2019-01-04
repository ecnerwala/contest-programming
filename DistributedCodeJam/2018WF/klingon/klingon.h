// Sample input 1, in CPP.
#include <assert.h>

typedef long long ll;

static int correct_answers[7001] = {1, 1, 1, 1, 1};

static int current_question;
static int number_of_wrong_answers;
static int done;

ll GetNumberOfQuestions() {
  return 7000;
}

ll GetAllowedNumberOfWrongAnswers() {
  return 3000;
}

ll GetNumberOfPossibleAnswers() {
  return 2;
}

int Answer(ll answer) {
  assert(answer >= 0 && answer < GetNumberOfPossibleAnswers());
  if (done) return 0;
  //int correct_answer = correct_answers[current_question];
  int correct_answer = 1;
  if (answer != correct_answer) {
    number_of_wrong_answers += 1;
  }
  current_question += 1;
  if (number_of_wrong_answers > GetAllowedNumberOfWrongAnswers()) {
    number_of_wrong_answers = 0;
    current_question = 0;
    return -1;
  }
  if (current_question == GetNumberOfQuestions()) {
    done = 1;
    return 42;
  }
  return 0;
}
