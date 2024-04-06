# A Record of Questions I've Implemented

## Contest 931 - [E. Sasha And Tree Cutting](https://codeforces.com/problemset/problem/1929/E)

This problem is from a recent competition is rated 2300. This is the highest rated problem I have ever done to date.

This problem has several moving parts so lets break them down 1 by 1.
First consider the various paths.
Do you have to deal with these paths, and if so, how?
We can get the edges of a path through a simple dfs.
Now, let's try holding all the edges of every path in memory.
What's the longest a path can be?
The answer is n-1 if it is a big linked list, so the total number of edges would be O(n\*k) which is not great.
We only want to color one edge so why do we need the entire edge set for every path?
We can describe each edge by the number of paths that go through it.
This way we don't have to consider n*k edges which have the same paths and as a result are functionally identical.
We can represent each path as a bitset or integer of k bits where k is at most 20.
An edge will have a binary value 101 if paths 0 and 2 goes through it and path 1 does not for example.

Now, this might seem scary at first 2^20 (2^k) is only a little better than 100,000\*20 (n\*k) but it will become obvious that it's impossible to have 2^k possible path representations.
Since this is a tree, each path has the possibilityof either joining or diverging from a new path and they can only do so once each time because you can't join and leave multiple times.
So essentially, every time you add a new path, you add at most 2\*k more unique path bitsets.
Thus we are upper bounded by (2\*k)\*k/2 and this number will be no more than 400 (we can also round up to 1000 and it will still be fine).

Now once we have all the unique bitsets for the paths we have to find the minimal combination of which will allow for all paths to be taken.
To do this we can perform a straightforward dp where we can either take or not take and edge.
The dp is the simplest part of this problem, the states are dp(current_bitset, current_edge) = min(dp(current_bitset | current_edge, next_edge) + 1, dp(current_bitset, next_edge)).

This solution will be, at worst, O(n\*k (2^k)\*(k^2)).
The n\*k term comes from the dfs and the (2^k)\*(k^2) comes from dp(bitset, edges).

My [code](Solutions/E.SashaAndTreeCutting.cpp) implements this dp.

## Contest 931 - [B. Yet Another Coin Problem](https://codeforces.com/contest/1934/problem/B)

This problem is from a recent competition and hasn't been rated yet. I would probably put it in the 1000-1400 range.

At first glance it seems possible to perform dynamic programming on this problem.
This is a trap: the value of each number is 1e9. This will make your dp recurrence 1e9 * 5 which is way too much.

What we can do is observe that 1 3 6 10 and 15 all have a common multiple of 30.
Any sufficiently high number will require multiple 30s.
This means we can just take many 30s as we need for free.
This can lower our dp to a manageable size.
Alternatively we can notice we will only ever want 2 10s, some number of 6s, etc and we can just use math to solve it.

My [code](Solutions/B.YetAnotherCoinProblem.py) uses the dp followup.

## Contest 926 - [Question D. Paprika and Permutation](https://codeforces.com/contest/1617/problem/C)

This problem was rated 1300 in an old div-2. It's quite straightforward.

If we have an item already in the permutation no need to change it.
If we see an item twice it must replace something smaller than it by half.
The reasoning for this is just that % can only create something of size n/2 - 1 or greater (try doing n%n-1 ... n%n/2-1, n%n/2).
If we see an item that is new we must replace it with the same.

My [code](Solutions/C.PaprikaandPermutation.py).

## Contest 926 - [Question D. Sasha and a Walk in the City](https://codeforces.com/contest/1929/problem/D)

This problem was rated 1900 in a recent div-2. It's very hard.

This question took me 3 hours to solve.

I approached it like this: all the root nodes are completely free to include or exclude.
Any given path will only have 2 of them and they can only be the start and end.
So where do I go from there?
Consider the tree with the edges: 1-2 and 1-3. 3 and 2 can be our root nodes so how do I calculate how many good paths are in this tree?

Well I know there is the empty set and the set of just 2 and the set of 1 from my previous realization that the roots are free.
Now in order to solve this further I realized I should probably consider their relationship to 1.
The set {2, 3} is valid because the simple paths 2-1-3 and 3-1-2 go through 2 and 3 at most 2 times.
Once I reach the node 1, I can choose to to include the path down to root 2 or the path down to root 3.
Now, I can find the number of sets that include both trees 2 and 3. I can do this by multiplying them together. So this gives us the ```value[node] = value[child_1] * value[child_2] * ...``` After I get all the values I can just sum, right?

This is wrong. I don't always want go down every path.
I also have all the values = 1 at the end and the answer clearly isn't n.
So, I need to choose which path I can go down. This modification can be done by the following:
```
value[node] = (value[child_1] + 1) * (value[child_2] + 1) * ...
```
This makes sense because in our example, if I want to ever use 1, I can't choose both 2 and 3 so I need 1 more possiblity for both of them.

It's easy to see if we color these nodes.
At the beginning we can do node 2, we can color that black.
Then we can color node 3 black too.
Now we get to the full tree 2-1-3.
We can color the following combinations black 1 & 2, 1 & 3, 2 & 3 and 1.
Now we have 4 + 1 + 1 = 6 nodes that have a black node.
Once I have this, I just need to consider the empty set.
To do that, I just add 1 to our answer.

Thus my answer is $\sum value[node] + 1$.

My [code](Solutions/D.SashaandaWalkintheCity.cpp) shows how to implement this approach.
This was a tough question so the explanation is not as good as it can be. I will revisit this soon

## Contest 926 - [Question C. Sasha and the Casino](https://codeforces.com/contest/1929/problem/D)
This question was rated 1400 on a recent div-2 contest.

This question is pretty mathematical so lets start with this: If you were a serial gambler with infinite money and playing roullette how would you make sure that you made out with at least one dollar? Well, the most surefire way is to bet one dollar on red and then keep betting half the amount of money you spent + 1 (amount_spent/2 + 1). Since you have infinite money, at some point you'll break even + 1.

Back to the problem, Sasha always wins if he has infinite money but he wants to make back money if he wins on the Xth round. He needs a minimum amount of money to do that. If this minimum is less than what he has on round X he will win some money. Otherwise he will run out before he becomes profitable.

At each round he needs to bet the amount_he_spent/(k - 1) + 1 to get more than he lost. The roulette here has more than 2 colors. Assuming I start by spending 1 I can just iterate through x rounds. Each round he spends just enough to make back what he would have lost, the worst case being he loses all X rounds (this is what I am looking for).

[My code](Solutions/C.SashaAndCasino.py) has the implementation.

## Contest 677 - Question F. Zero Remainder Sum
This question was rated 2100 but it's 3 years old and a div-3 so it would probably be something like a 1600 div-2 right now, maybe even less.

I'll start with how I approached this problem: Assuming there is one row, how do I find the ways to select <= m/2 numbers?

At each index, I can either take or not take the number. I can also take however much is needed at most.
I can represent this is as dp[ind][needed].
It can go to the states dp[ind+1][needed-1] or dp[ind+1][needed].
For now let's say this stores the maximum value that works.
Now lets make this multi-row.

How do I do this?
Well for each row I can simply do the same process I did for a single row.
Now I can say that there exists a state dp[row][ind][needed] and it can go to the states dp[row][ind+1][needed-1] or dp[row][ind+1][needed].
If ind == m that means I'm at the end of the row so dp[row][m][needed] becomes dp[row+1][0][m/2] because I go to a new row and start from the beginning while refreshing all our choices.
Another case is if needed ever hits 0 I just go to the next row so dp[row][ind][0] becomes dp[row+1][0][m/2].

Now, there is just one last problem. Our final value must be divisible by K. I can't just take the highest value at every index of our dp table since the final remainder must be 0. This means I need to track our current number as I can reach dp[row][ind][needed] multiple ways.
Obviously while this will give a correct answer, dp[row][ind][needed][num] is very expensive because num can be 75 * 75 * 35.
I can do better. Why do I need the exact number?
Our number will work if the remainder of num + remainder of future nums is 0. So all I need to store is the num which is of size K (75) and considerably smaller.

[My code](Solutions/F.ZeroRemainderSum.cpp) has a line by line breakdown detailed explanation.