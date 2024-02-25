# A Record of Questions I've Implemented

## Contest 677 - Question F. Zero Remainder Sum
This question was rated 2100 but it's 3 years old and a div-3 so it would probably be something like a 1600 div-2 right now, maybe even less. I sight solved this problem, but tried to be clever with the implementation. I suppose there is a reason no one does DP the way I tried to: it sucks.

I'll start with how I approached this problem: Assuming there is one row, how do we find the ways to select <= m/2 numbers?
At each number, we can either take or not take the number.
I can represent this is as dp[ind][needed]. It can go to the states dp[ind+1][needed-1] or dp[ind+1][needed]. Choose a number and decrease how much you need or don't. Now lets make this multi-row.