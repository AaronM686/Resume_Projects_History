
// This is my solution for the "Balanced Brackets" challenge from HackerRank.
// <https://www.hackerrank.com/challenges/balanced-brackets/problem>
// This is not a complete file, just the solution part,
// where I filled-in the function as provided in the challenge.
// This solution clears all tests.

/*
 * Complete the 'isBalanced' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts STRING s as parameter.
 */

string isBalanced(string s) {

// Use recursion on substrings within each bracket...
// first find the outermost bracket pair,
// then cut a substring and call this function recursively to see if each sub-string is balanced internally...

    // Null case returns YES (needed for recursion to work)
    if (s.length() ==0){
        return "YES";
    }
    
    char MatchToFind;

    switch (s[0]){
        case '[':
            MatchToFind =']';
        break;
        
        case '{':
            MatchToFind = '}';
        break;
        
        case '(':
            MatchToFind = ')';
        break;
        
        default:
            printf("Logic Error! unknown symbol!\n");
            return string("NO");
    } // end switch statement
    
    // This needs to be more sophisticated to count up/down:
    // size_t IndxSplit = s.find_first_of(MatchToFind);
    
    int IndxSplit = INT32_MIN;
    
    int MatchCounter = 1; // starting at 1 becuase we already have an opening symbol, now look for the proper
    //  (matching) closing symbol.
    for (int i = 1; ((i < s.length()) && (MatchCounter != 0)); i++)
    {
        if (s[i] == s[0]){
            MatchCounter++;
        }
        
        if (s[i] == MatchToFind){
            MatchCounter--;
            
            if (MatchCounter == 0){
                IndxSplit = i;
                break;
            }
        }
        

    }
    
    // closing match not found, return "NO" !
    if (MatchCounter != 0){
        return string("NO"); 
    }
    
    // in trivial case of two characters and found match in second character, return YES.
    if ((IndxSplit == 1) && (s.length() == 2)){
        return string("YES");
    }

    // Then split into two substrings and test them:
    string LeftSubstring = s.substr(1,IndxSplit-1);
    string RightSubtring = s.substr(IndxSplit+1);

    //debug printf
    printf("'%s' - '%s'\n",LeftSubstring.c_str(),RightSubtring.c_str());

    if ((isBalanced(LeftSubstring) == "YES") && (isBalanced(RightSubtring) == "YES")){
        return string("YES");
    }

    // all else fails, return NO:
    return string("NO");

}
