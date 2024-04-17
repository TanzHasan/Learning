+++
title = "1249 - Minimum Remove to Make Valid Parentheses"
date = "2024-04-06"
+++

[1249 - Minimum Remove to Make Valid Parentheses](https://leetcode.com/problems/minimum-remove-to-make-valid-parentheses/)

If there is a ) and there are no ( before it, then it must be removed.
If there are ( at the end, remove them.

```python
class Solution:
    def minRemoveToMakeValid(self, s) :
        stack=[]
        split_str=list(s)
        for i in range(len(s)):
            if s[i]=='(':
                stack.append(i)
            elif s[i]==')':
                if len(stack) !=0:
                    stack.pop()
                else:
                    split_str[i]=""
        for i in stack:
            split_str[i]=""
        return '' .join(split_str)
```


not sure if its possible to one line this