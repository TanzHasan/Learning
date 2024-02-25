# A Record of Questions I've Implemented

## Contest 926 - Question C. Sasha and the Casino
This question was rated 1400 on a recent div-2 contest.

This question is pretty mathematical so lets start with this: If you were a serial gambler with infinite money, playing roullette how would you make sure that you made out with at least one dollar? Well, the most surefire way is to bet one dollar on red and then keep betting half the amount of money you spent + 1 (amount_spent/2 + 1). Since you have infinite money, at some point you'll break even + 1.

Back to the problem, Sasha always wins if he has infinite money but he wants to make back money if he wins on the Xth round. He needs a minimum amount of money to do that. If this minimum is less than what he has on round X he will win some money. Otherwise he will run out before he becomes profitable.

At each round he needs to bet the amount_he_spent/(k - 1) + 1 to get more than he lost. Assuming we start by spending 1 we can just iterate through x rounds. Each round he spends just enough to make back what he would have lost, the worst case being he loses all X rounds (this is what we are looking for).

[My code](Solutions/C.SashaAndCasino.py) has the implementation

## Contest 677 - Question F. Zero Remainder Sum
This question was rated 2100 but it's 3 years old and a div-3 so it would probably be something like a 1600 div-2 right now, maybe even less.

I'll start with how I approached this problem: Assuming there is one row, how do we find the ways to select <= m/2 numbers?

At each index, we can either take or not take the number. We can also take however much is needed at most.
I can represent this is as dp[ind][needed].
It can go to the states dp[ind+1][needed-1] or dp[ind+1][needed].
For now let's say this stores the maximum value that works.
Now lets make this multi-row.

How do we do this?
Well for each row we can simply do the same process we did for a single row.
Now we can say that there exists a state dp[row][ind][needed] and it can go to the states dp[row][ind+1][needed-1] or dp[row][ind+1][needed].
If ind == m that means we're at the end of the row so dp[row][m][needed] becomes dp[row+1][0][m/2] because we go to a new row and start from the beginning while refreshing all our choices.
Another case is if needed ever hits 0 we just go to the next row so dp[row][ind][0] becomes dp[row+1][0][m/2].

Now, there is just one last problem. Our final value must be divisible by K. We can't just take the highest value at every index of our dp table since the final remainder must be 0. This means we need to track our current number as we can reach dp[row][ind][needed] multiple ways.
Obviously while this will give a correct answer, dp[row][ind][needed][num] is very expensive because num can be 75 * 75 * 35.
We can do better. Why do we need the exact number?
Our number will work if the remainder of num + remainder of future nums is 0. So all we need to store is the num which is of size K (75) and considerably smaller.

[My code](Solutions/F.ZeroRemainderSum.cpp) has a line by line breakdown detailed explanation.