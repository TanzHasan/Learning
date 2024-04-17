+++
title = "1614 - Maximum Nesting Depth of the Parentheses"
date = "2024-04-04"
+++

[1614 - Maximum Nesting Depth of the Parentheses](https://leetcode.com/problems/maximum-nesting-depth-of-the-parentheses/description/)

Easy daily. Find max depth using stack and return.
Two One line solutions below:
```c++
class Solution {
public:
    int maxDepth(string s) {
        return accumulate(s.begin(), s.end(), make_pair(0, vector<int>{0}), [](auto res, auto c) {
            res.second.push_back(res.second.back() + (c == '(' ? 1 : (c == ')' ? -1 : 0)));
            return make_pair(max(res.first, res.second.back()), res.second);
        }).first;
    }
};
```
and 
```python
class Solution:
    def maxDepth(self, s: str) -> int:
        return max(accumulate([1 if (i == '(') else (-1 if i ==')' else 0) for i in s ]))
```
