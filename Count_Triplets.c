
// this is my code snippet for the solution to HackerRank "Count Triplets" challenge.
// <https://www.hackerrank.com/challenges/count-triplets-1/problem>
// this is not the complete program, just the user-provided functions necessary to
// complete the Challenge on HackerRank.
// This completes and clears all tests.

// old function just for concept reference:
// "brute force" with nested loops, slow but just a start:
// Todo: change this to hash lookup table...
long countTriplets_slow(vector<long> arr, long r) {
    long NumTriplets = 0;
    
    // This is our candidate to test the following for a sequence...
    for (size_t i = 0; i<arr.size(); i++) {
        
        long NextInSeq = arr[i] * r;
        size_t IndexOfMatch = 0;
        // test the following numbers in the array to see if we can find "NextInSeq"
        for (size_t j = i+1; j < arr.size(); j++){
            // if we have a match...
            if ((j < arr.size()) && (arr[j] == NextInSeq)) {
                IndexOfMatch = j;
                
                long IndexOfFinalMatch = 0;
                // now look for the final (of the triplet) in the sequence:
                long FinalInSeq = NextInSeq * r;
                
                // test the following numbers in the array to see if we can find "NextInSeq"
                for (size_t k = IndexOfMatch+1; k < arr.size(); k++){
                    // if we have a match...
                    if ((k < arr.size()) && (arr[k] == FinalInSeq))  {
                        IndexOfFinalMatch = k;
                        
                        NumTriplets++;
                        
                        // printf("Found Match at [%ld,%ld,%ld]\n",i,IndexOfMatch,IndexOfFinalMatch);
                    }
                    else {
                        if ((k < arr.size()) && (arr[k] > FinalInSeq)){
                            break; // no reason to continue the loop if we've encountered larger numbers
                        } 
                    }
                }// for j look looking for next in sequence
                
            }
            else {
                if((j < arr.size()) && (arr[j] > NextInSeq)){
                    break; // no reason to search anymore if we've encountered large numbers.
                }
            }
        }// for j look looking for next in sequence
        
        
    } // end for i loop over arr

    return NumTriplets;
}

// Complete the countTriplets function below...
long countTriplets(vector<long> arr, long r) {
    size_t NumTriplets = 0;
    
    // "occurence maps" for the right and left of the iteration
    std::map<long,long> RightMap;
    std::map<long,long> LeftMap;
    
    // first create the overall occurence map
    for (int i = 0; i < arr.size(); i++){
        RightMap[arr[i]] = RightMap[arr[i]] + 1;
    }
    
    // debug printf: print the occurence map and values
    for (auto Iter : RightMap){
        printf("[%ld]:%ld\t",Iter.first,Iter.second);
    }     printf("\n");
    
    // Check the array from tip to tail, decrementing values off of our RightMap and incrementing them onto the Left Map
    for (long i = 0; i <arr.size(); i++){
        // decrement the current iteration value from the Right map
        RightMap[arr[i]] = RightMap[arr[i]] - 1l;

        
        // this only works if evenly divisible by r...
        if ((arr[i] == 1) || ((arr[i] % r)== 0)){
            long NumMatches = RightMap[arr[i]*r] * LeftMap[arr[i] / r];
            //printf("Considering arr[%ld] = %ld, found %ld matches on left and %ld matches on right = %ld\n",
            //        i,arr[i], LeftMap[arr[i] / r],RightMap[arr[i]*r],NumMatches);

            NumTriplets += NumMatches;
        }// only if evenly divisible by r
                
        // increment the current iteration value into the Left map
        LeftMap[arr[i]] = LeftMap[arr[i]] + 1l;
    }
    
    // debug printf: print the occurence map and values
    for (auto Iter : LeftMap){
        printf("[%ld]:%ld\t",Iter.first,Iter.second);
    }     printf("\n");
   
    
    return NumTriplets;
}
