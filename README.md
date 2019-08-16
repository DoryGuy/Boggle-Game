# Boggle-Game
Sample C++ code for the game boggle

I did this as an exercise, it doesn't have a full dictionary in it yet. I grabbed dictionary-yawl.txt but it has words that don't seem like they should be legal in Boggle, ie "tes" which as far as I can tell is an acronym for "Times Educational Suppliment" which if I was playing against you, I would strongly object. but hey, this is a sample program and no better than the dictionary you give it." 

The current solution is a O(N^2) using recursion, threads and a tri-dictionary for search pruning. I've read that there are faster solutions but I'm exploring this space as a programming exercise. The trouble with this solution is that pruning doesn't really work well when there are a lot of words that can be found, ie little to no pruning occurs.'
