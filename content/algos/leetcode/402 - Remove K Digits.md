+++
title = "402 - Remove K Digits"
date = "2024-04-11"
+++

Day 8: https://leetcode.com/problems/remove-k-digits

Explanation:

This is a pretty good question. It's very greedy, simply think about it like this: If you have the number 123 which number do you remove? The answer is 3 because if you remove 2, you're replacing it with a greater element (making it 13) and if you remove 1, you're also doing that (23). Now what about 132? You also want to replace 3 because you're replacing a high number with a lower number. This means, if we have a number to the right that is lower, we should remove our current number, otherwise we keep it. Let's say we have 331 and k = 2. We want to remove both 3s. This means we should store our preserved values in a stack because we need to be able to remove them later.

Easy to read code
```python
class Solution:
    def removeKdigits(self, num: str, k: int) -> str:
        stack = deque()
        for i in num:
            while k and stack and stack[-1] > i:
                stack.pop()
                k-=1
            stack.append(i)
        for i in range(k):
            stack.pop()
        while stack and stack[0]== "0": stack.popleft()
        return ''.join(stack) or "0"
```