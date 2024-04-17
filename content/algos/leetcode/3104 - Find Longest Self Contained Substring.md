+++
title = "3104 - Find Longest Self Contained Substring"
date = "2024-04-03"
+++

[3104 - Find Longest Self Contained Substring](https://leetcode.com/problems/find-longest-self-contained-substring/)

This problem was kind of trivial.
Here is the writeup I posted to the leetcode discussions:

Intuition
A substring is valid if letters in the substring do not appear elsewhere. This means the first and last occurrence of a letter must be in the substring.

Approach
Go through the string. Keep track of the first and last occurrences of each letter. We are also using a prefix sum to quickly determine which letters have been used and at what index.
Now, we can do combinations of the first and last occurrences of each letter. This is quadratic but since there are only 26 possibilities for letter combinations we are fine.

Then we have to find which characters are used in the substring. This can be done by using a prefix sum and checking the counts of all the letters before and after the substring. These lines explain how:

```python
for k in range(26):
    if start[k] != stop[k]:
        if stop[k] != 0:
            bad = True
            break
        diffs.append(k)
```
While doing this we also have to make sure that none of the letters used in the string have been used before and none are used after. These are checked in the below code on these lines
```python
if stop[k] != 0:
    bad = True
    break
```
and
```python
if start[k] != prefix[0][k]:
    bad = True
    break
```

Time complexity:
O(n * 676) or O(n \* 26 \* 26)

Space complexity:
O(n*26)
