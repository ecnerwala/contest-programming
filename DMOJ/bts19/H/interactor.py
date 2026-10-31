import argparse
import random
import sys

ERROR_CODE = -400

# generator constraints
PRICE_BND = 10**7
MARKET_BND = 80
NUM_CASES = 10

# solution constraints
MAX_DAYS = 40000
QUERY_BND = 10**18


def random_price():
    return random.randint(0, PRICE_BND)


def random_market():
    return (random_price(), random_price())


def readln():
    # get rid of extraneous whitespace
    return ' '.join(input().split()).split()


def writeln(*args):
    print(*args)


def FAIL():
    writeln(ERROR_CODE)
    sys.exit(2)


def run_case(subtask):
    # generate test case
    N = random.randint(1, [1, 2, 4, 10, MARKET_BND][subtask - 1])
    markets = [random_market() for i in range(N)]

    # interact
    query_data = {}

    writeln(N)

    def handle_purchase(day, values):
        A_b, A_s, B_b, B_s, P_j = values
        if not (0 <= A_b <= QUERY_BND and 0 <= A_s <= QUERY_BND and
                0 <= B_b <= QUERY_BND and 0 <= B_s <= QUERY_BND and
                abs(P_j) <= QUERY_BND):
            FAIL()
        lesser_than_p, greater_than_p = 0, 0
        for market in markets:
            profit = (A_s * market[0] - A_b * market[0] + B_s * market[1] - B_b * market[1])
            greater_than_p += (profit > P_j)
            lesser_than_p += (profit < P_j)
        ans = greater_than_p - lesser_than_p
        if ans != 0:
            ans //= abs(ans)
        query_data[day] = (ans, values)
        writeln(ans)

    def handle_comparison(values):
        j, k = values
        if j not in query_data.keys() or k not in query_data.keys() or query_data[j][0] != 0:
            FAIL()
        A_bj, A_sj, B_bj, B_sj, P_j = query_data[j][1]
        A_bk, A_sk, B_bk, B_sk, P_k = query_data[k][1]

        banana_prices = []
        for market in markets:
            profit_j = (A_sj * market[0] - A_bj * market[0] + B_sj * market[1] - B_bj * market[1])
            profit_k = (A_sk * market[0] - A_bk * market[0] + B_sk * market[1] - B_bk * market[1])
            if profit_j >= P_j and profit_k >= P_k:
                banana_prices.append(market[1])
        if len(banana_prices) <= 1:
            banana_prices.extend([-1, -1])
            writeln(*banana_prices[:2])
        else:
            writeln(*random.sample(banana_prices, 2))

    def handle_answer():
        try:
            user_markets = list(tuple(map(int, readln())) for i in range(N))
        except Exception:
            FAIL()
        return sorted(user_markets) == sorted(markets)

    for day in range(1, MAX_DAYS + 2):
        query = readln()
        try:
            values = list(map(int, query[1:]))
        except Exception:
            FAIL()
        # since answering does not count as a day, you can answer
        # past the last day
        answer_day = (day == MAX_DAYS + 1)
        if not answer_day and len(query) == 6 and query[0] == '?':
            handle_purchase(day, values)
        elif not answer_day and len(query) == 3 and query[0] == 'C':
            handle_comparison(values)
        elif len(query) == 1 and query[0] == '!':
            return handle_answer()
        else:
            FAIL()
    FAIL()


def main():
    parser = argparse.ArgumentParser(description='BTS19P8 INTERACTOR')
    parser.add_argument('subtask', choices=(1, 2, 3, 4, 5), type=int, help='The subtask number.')
    subtask = parser.parse_args().subtask

    num_correct = 0
    for case in range(NUM_CASES):
        num_correct += run_case(subtask)

    # at least 90% correct
    if num_correct * 10 >= NUM_CASES * 9:
        print('Passed {correct}/{total} cases, AC'.format(correct=num_correct, total=NUM_CASES), file=sys.stderr)
        return 0
    else:
        print('Passed {correct}/{total} cases, WA'.format(correct=num_correct, total=NUM_CASES), file=sys.stderr)
        return 1


sys.exit(main())
