from collections import deque
import sys
input = sys.stdin.readline

def solve():
    n = int(input())
    a = list(map(int, input().split()))
    seen = [0]*(n+1)
    a.sort()
    missing = deque()
    last_seen = 0
    ans = 0
    for i in a:
        for num in range(last_seen + 1, min(n+1, i)):
            missing.append(num)
            last_seen = num
        last_seen = i
        if i <= n and not seen[i]:
            seen[i] = 1
        else:
            if missing and missing[0]*2 < i:
                seen[missing.popleft()] = 1
                ans += 1
            else:
                print(-1)
                return

    print(ans)


    
t = int(input().strip())
for _ in range(t):
    solve()