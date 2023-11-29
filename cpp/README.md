# Boggle-Game
Sample C++ code for the game boggle

I did this as an exercise, it doesn't have a full dictionary in it yet. I grabbed dictionary-yawl.txt but it has words that don't seem like they should be legal in Boggle, ie "tes" which as far as I can tell is an acronym for "Times Educational Suppliment" which if I was playing against you, I would strongly object. but hey, this is a sample program and no better than the dictionary you give it." 

The current solution is a O(N^2) using recursion, threads and a quint-dictionary for search pruning. I've read that there are faster solutions but I'm exploring this space as a programming exercise. The trouble with this solution is that pruning doesn't really work well when there are a lot of words that can be found, ie little to no pruning occurs.

    However with a Tri-level dictionary, where we group words by their first 3 letters, did improve the lookup time and did some pruning. I then tried a next level with a "quad" root dictionary below the trr-level one, and got a 100% improvement in pruning. But with a high scoring board, ie lots of words it also didn't prune as well as I would have liked. So for a lot more complexity I added yet another level, a quint-level dictionary. That was a significant improvement in pruning. A sample test board with only 3 words in it was solved in 5 milli seconds. The best board was done in just over 1 second. This is a 200% improvement over the raw dictionary and straight lookup.

   So if you are looking to have some fun with a pruning and level dictionary seach have fun! Adding another level to the dicitionary may even improve the search time even more.'
