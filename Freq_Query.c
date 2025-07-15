
// This is my (Aaron Mosher) code solution to the "FrequencyQueries" problem
// on HackerRank:
// <https://www.hackerrank.com/challenges/frequency-queries/problem>
// this is not a complete program, just the user-provided function necessary
// to complete the challenge. This completes and clears all tests for the challenge.

// call this After CounterMap has been already updated !
void UpdateFreqValueFromMM (std::map<int,long> &CounterMap, std::multimap<long,int> &FreqReverseMMap,
 int Value, int AddOrRemove)
{
    
    long FrequencyNew = CounterMap[Value];
    
    // if adding then the old frequency was one less, if removing then the old frequency was one more.
    long FrequencyOld = FrequencyNew - ((long)AddOrRemove);
    
    assert((FrequencyNew > 0) || (FrequencyOld > 0));
    if ((FrequencyNew <= 0) && (FrequencyOld <= 0)){
        printf("Logic Error 1\n");
    }
    
    // lookup the "Frequency" from the Counter Map.
    // use this "Frequency" as the key to Remove or Add it from the FrequencyMap.
    auto RangeBoundaries = FreqReverseMMap.equal_range(FrequencyOld);
    
    // note must pre-increment when using Iterator with inclusive range [begin,end]
    for (auto RangeIter = RangeBoundaries.first; RangeIter != RangeBoundaries.second; ++RangeIter)
    {
        // looking for the key entry that has this value, to remove it..
        if (RangeIter->second == Value){
            FreqReverseMMap.erase(RangeIter); // remove this entry
            break; // done with this loop.
        }
    }
    
    // now add it back to the correct Frequency key:
    FreqReverseMMap.insert({FrequencyNew,Value});
}

// Complete the freqQuery function below.
vector<int> freqQuery(vector<vector<int>> queries) {

    vector<int> ResultsReturn;
    
    // create a map for the array values
    std::map<int,long> CounterMap;
    
    // creating a "reverse lookup" for frequencies of each value in CounterMap,
    // this saves us from having to iterate through all of CounterMap looking for a Value
    std::multimap<long,int> FreqReverseMMap;
    
    // for every command-operand pair in the Queries vector....
    for (auto Iter : queries)
    {
        int Value = Iter[1];
        
        switch (Iter[0]){ // what is the command?
            case 1: // adding a value to the map
            {
                
                // debug printf
                printf("Adding Value %d\n",Value);
                
                long PrevCount = CounterMap[Value];
                CounterMap[Value] = PrevCount + 1l;
                
                // do something to update this...
                UpdateFreqValueFromMM(CounterMap,FreqReverseMMap,Value,+1);

            }
            break;
            
            case 2: // removing a value from the map
            {
                
                
                // debug printf
                printf("Removing Value %d\n",Value);
                
                long PrevCount = CounterMap[Value];
                
                if (PrevCount > 0){
                    CounterMap[Value] = PrevCount - 1l;
                    
                    // do something to update this...
                    UpdateFreqValueFromMM(CounterMap,FreqReverseMMap,Value,-1);
                }
                else {
                    printf("Warning: Tried to remove value %d which was already missing!\n",Value);

                }
                
              
            }
            break;
            
            case 3:
            {   // query for frequency.
            
                // find any Value that has the exact Frequency we are looking for
                int FoundCount = FreqReverseMMap.count(Value);
                
                /*  Old code ignore this, its slow.
                // loop through and count how many have exactly "Value" occurences in the map
                // then add that to the Results Vector output: 1 or 0.
                bool ExactFreqFound = false;
                // a naive way to do this is to iterate thru all values in Counter Map and find
                // any that match count to frequency exactly, but this takes some time...
                for (auto CounterIter : CounterMap) {
                    if (CounterIter.second == Value){
                        ExactFreqFound = true;
                        break; // done with this loop
                    }
                }// end for every element in CounterMap
                */
            
                if (FoundCount){
                    ResultsReturn.push_back(1);
                }
                else{
                    ResultsReturn.push_back(0);
                }
            }
            break;
            
            default:
                printf("Unknown Operand %d, no action.\n",Iter[0]);
            
        }// end switch statement
        

    } // for every entry in Queries
    
    
    return (ResultsReturn);
}
