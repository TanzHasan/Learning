import sys
from functools import lru_cache
input = sys.stdin.readline

def solve():
    n = int(input())
    ans = max(n//15 - 3, 0)
    n -= ans * 15
    needed = dp((1, 3, 6, 10, 15), n)
    print(needed + ans)

@lru_cache(None)
def dp(choose, rem):
    lowest = float('inf')
    if rem < 0:
        return float('inf')
    if rem == 0:
        return 0
    for i in range(len(choose)):
        lowest = min(lowest, 1 + dp(choose[i:], rem - choose[i])) 
    return lowest

t = int(input())
for _ in range(t):
    solve()