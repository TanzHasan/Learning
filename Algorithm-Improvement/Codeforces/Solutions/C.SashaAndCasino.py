import sys

input = sys.stdin.readline


def solve():
    k, x, a = map(int, input().split())
    willwin = 1
    for _ in range(x):
        willwin += willwin // (k - 1) + 1

    print("Yes" if willwin <= a else "No")


t = int(input().strip())
for _ in range(t):
    solve()
