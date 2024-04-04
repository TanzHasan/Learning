
inf = float('inf') # this is done by LC
class Solution:
    def maxSubstringLength(self, s: str) -> int:
        offset = ord('a')
        occurrences = [0]*26
        pos = [[inf, -inf] for _ in range(26)]
        for i, letter in enumerate(s):
            letter_ind = ord(letter) - offset
            occurrences[letter_ind] += 1
            pos[letter_ind][0] = min(i, pos[letter_ind][0])
            pos[letter_ind][1] = max(i, pos[letter_ind][1])
        prefix = [occurrences]
        for i in s:
            prefix.append(list(prefix[-1]))
            prefix[-1][ord(i)-offset]-=1
        ans = -1
        for i in range(26):
            for j in range(26):
                l, h = pos[i][0], pos[j][1]
                if l > h: continue
                if l == 0 and h == len(s)-1: continue
                start = prefix[l]
                stop = prefix[h+1]
                diffs = []
                bad = False
                for k in range(26):
                    if start[k] != stop[k]:
                        if stop[k] != 0:
                            bad = True
                            break
                        diffs.append(k)
                if bad:
                    continue
                for k in diffs:
                    if start[k] != prefix[0][k]:
                        bad = True
                        break
                if bad:
                    continue
                ans = max(ans, h+1-l)
        return ans