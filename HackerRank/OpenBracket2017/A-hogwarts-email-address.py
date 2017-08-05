#!/bin/python3

import sys

def isValid(email):
    return email.lower() == email and len(email) == len('abcde@hogwarts.com') and email[:5].isalpha() and email[5:] == '@hogwarts.com'

if __name__ == "__main__":
    s = input().strip()
    result = isValid(s)
    print("Yes" if result else "No")
