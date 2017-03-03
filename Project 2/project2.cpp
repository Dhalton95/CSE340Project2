/*******************/
// Richard Huber
// 1207319785
// CSE 340
// Project 2
/*******************/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include "lexer.h"
#include <vector>

using namespace std;


// Create a vector to hold the strings for all symbols
vector<string> allSymbols;

class Rule
{
    private:
        // All class level variables for each object of Rule
        int LHS;
        vector<int> RHS;
        int RHS_size;
        bool eliminated;
        bool seen;
    
    public:
        Rule()
        {
            this->RHS_size = 0;
            this->eliminated = false;
            this->seen = false;
        }
        // Function to set the left hand side of the Rule
        void setLHS(int index)
        {
            this->LHS = index;
        }
        // Function to get the LHS index
        int getLHS()
        {
            return this->LHS;
        }
    
        // Functin to add to the right hand side of a Rule
        void addRHS(int index)
        {
            this->RHS.push_back(index);
            this->RHS_size++;
        }
    
        // Function to determine if a symbol is included in a rule (to help count the occurrences)
        bool isInRule(string aSymbol)
        {
            // boolean to be returned at the end
            bool toReturn = false;
            
            // If the symbol at the index of LHS in allSymbols vector is the symbol that was passed
            // to the function
            if (allSymbols[this->LHS] == aSymbol)
            {
                // Set toReturn to true
                toReturn = true;
            }
            
            // For loop to traverse the array holding the indices in RHS
            for (int i = 0; i < RHS_size; i++)
            {
                // If the symbol at any index of RHS in allSymbols is the symbol that was passed
                // to the function
                if (allSymbols[this->RHS[i]] == aSymbol)
                {
                    // Set toReturn to true
                    toReturn = true;
                }
            }
            // Return the boolean
            return toReturn;
        }
    
        // Function to determine if a LHS symbol is generating by checking if any of the rule's RHS members are
        // NOT generating
        bool isGenerating(vector<bool> generatingArray)
        {
            // Initialize boolean toReturn to true
            bool toReturn = true;
            
            // For loop to traverse the RHS vector
            for (int i = 0; i < RHS_size; i++)
            {
                // If ANY element of the right hand side is not generating
                if (!generatingArray[RHS[i]])
                {
                    // Set toReturn to false
                    toReturn = false;
                }
            }
            // Return the boolean we designated to return
            return toReturn;
        }
    
        // Function to determine if the rule contains any symbols that are not generating
        bool hasSymbolNotGenerating(vector<bool> generating)
        {
            // Initialize boolean to be returned to false
            bool toReturn = false;
            
            // If the symbol at LHS index is not generating
            if (!generating[LHS])
            {
                // Set toReturn to true
                toReturn = true;
            }
            
            // For loop to traverse the right hand side
            for (int i = 0; i < this->RHS_size; i++)
            {
                // If any element of the RHS is not generating
                if (!generating[RHS[i]])
                {
                    // Set toReturn to true
                    toReturn = true;
                }
            }
            // Return the boolean
            return toReturn;
        
        }
    
        // Function to eliminate a particular rule by setting the class level bool eliminated to true
        void eliminate()
        {
            this->eliminated = true;
        }
    
        // Function to check whether or not a rule is eliminated
        bool isEliminated()
        {
            return this->eliminated;
        }
    
        // Function to set the right hand side of a rule to reachable if the left hand side is reachable
        void setRHSReachable(vector<bool> &reachable)
        {
            // Iterate through RHS vector
            for (int i = 0; i < RHS_size; i++)
            {
                // Set all indices of RHS in reachable to true
                reachable[RHS[i]] = true;
            }
            // Set rule's seen variable to true so we know not to keep looping as we
            // are checking what is reachable
            this->seen = true;
        }
    
        // Function to check whether or not we have seen that we can change a RHS to reachable or not
        bool hasBeenSeen()
        {
            return this->seen;
        }
    
        // Function to determine if a given rule should be displayed after finding useless symbols
        bool shouldBeDisplayed(vector<bool> useless)
        {
            // Boolean to return at the end
            bool toReturn = true;
            // If the LHS is useless
            if (useless[LHS])
            {
                // Set toReturn to false, because the rule should not be printed
                toReturn = false;
            }
            
            // Traverse the RHS
            for (int i = 0; i < RHS_size; i++)
            {
                // If any element of the RHS is useless
                if (useless[RHS[i]])
                {
                    // Set toReturn to false, because the rule should not be printed
                    toReturn = false;
                }
            }
            // Return the designated boolean
            return toReturn;
        }
    
    
        bool UpdateFIRST(vector<vector<bool> > &allFIRSTSets, int FIRSTFOLLOWsize)
        {
            // Boolean we will return to indicate if we updated any FIRST sets while looking at this rule
            bool didUpdate = false;
            
            /****************/
            // THIS SECTION OF CODE CHECKS FOR CalCULATING FIRST SETS RULE NUMBER 3
            /****************/
            for (int i = 0; i < FIRSTFOLLOWsize; i++)
            {
                // Add all elements of the first RHS symbol (RHS[0]) FIRST set to the LHS first set
                if (allFIRSTSets[i][RHS[0]] && !allFIRSTSets[i][LHS] && i != 0)
                {
                    allFIRSTSets[i][LHS] = true;
                    // If we changed anything, set didUpdate to true;
                    didUpdate = true;
                }
            }
            /****************/
            
            
            /****************/
            // THIS SECTION OF CODE CHECKS FOR CALCULATING FIRST SETS RULE NUMBER 4
            /****************/
            
            // Boolean to determine whether or not we have seen a symbol on the RHS that does not
            // have # in its FIRST set
            bool haveSeenNonEpsilon = false;
            
            // For loop to traverse all indices of the RHS of the rule
            for (int index = 0; index < RHS_size; index++)
            {
                // If epsilon is in the first set of a leading character on the RHS
                if (allFIRSTSets[0][RHS[index]] && index != RHS_size - 1)
                {
                    // Traverse the RHS + 1 symbol's FIRST set
                    for (int j = 0; j < FIRSTFOLLOWsize; j++)
                    {
                        // If RHS[index + 1] has something in its FIRST set that the LHS doesn't, and
                        // and it is not epsilon
                        if (allFIRSTSets[j][RHS[index + 1]] && !allFIRSTSets[j][LHS] && j != 0)
                        {
                            // Add it to LHS FIRST set
                            allFIRSTSets[j][LHS] = true;
                            // Set didUpdate to true
                            didUpdate = true;
                        }
                    }
                }
                // If we see a symbol in the RHS that does not have # in its FIRST set
                else
                {
                    // Set haveSeenNonEpsilon to true
                    haveSeenNonEpsilon = true;
                }
                
                // On each loop iteration, check if we have seen a symbol who doesn't have # in its FIRST set
                if (haveSeenNonEpsilon)
                {
                    // If we have, we don't want to add later symbols who could have # in their FIRST set to LHS first set,
                    // so we break from the loop
                    break;
                }
            }
            
            /****************/
            
            
            /****************/
            // THIS SECTION OF CODE CHECKS FOR CALCULATING FIRST SETS RULE NUMBER 5
            /****************/
            // Boolean to determine if a non-terminal can generate epsilon
            bool generatesEpsilon = true;
            
            // Loop through the rule's RHS
            for (int i = 0; i < RHS_size; i++)
            {
                // If any of its members does not have # in its FIRST set
                if (!allFIRSTSets[0][RHS[i]])
                {
                    // Set generates epsilon to false
                    generatesEpsilon = false;
                }
            }
            
            // If the LHS generates epsilon and we have not already seen that # is in the LHS FIRST set
            if (generatesEpsilon && !allFIRSTSets[0][LHS])
            {
                // Add # to the FIRST set of the LHS
                allFIRSTSets[0][LHS] = true;
                // Set didUpdate to true
                didUpdate = true;
            }
            
            
            // Return the boolean indicating whether or not we updated any FIRST sets
            return didUpdate;
        }
    
    
        bool updateFOLLOW(vector<vector<bool> > &allFOLLOWSets, vector<vector<bool> > &allFIRSTSets, int FIRSTFOLLOWsize,
                          int startSymbol)
        {
            bool didUpdate = false;
            
            /********************/
            //THIS SECTION OF CODE CHECKS FOR CALCULATING FOLLOW SETS RULE NUMBER 2
            /********************/
            
            // For loop to traverse the possibilities of what could be in the FOLLOW set
            // Starts at 1 because # cannot be in FOLLOW sets
            for (int i = 1; i < FIRSTFOLLOWsize; i++)
            {
                // If there is something in the LHS's FOLLOW set that is not in the last symbol of the RHS's
                if (allFOLLOWSets[i][LHS] && !allFOLLOWSets[i][RHS[RHS_size - 1]])
                {
                    // Add the elements of LHS FOLLOW set to last symbol of RHS FOLLOW set
                    allFOLLOWSets[i][RHS[RHS_size - 1]] = true;
                    // Set didUpdate to true, so we know to keep going
                    didUpdate = true;
                }
            }
            /*********************/
            
            
            /*********************/
            // THIS SECTION OF CODE CHECKS FOR CALCULATING FOLLOW SETS RULE NUMBER 3
            /*********************/
            // This outer for loop will traverse the RHS and look at each symbol
            for (int bIndex = 0; bIndex < RHS_size; bIndex++)
            {
                // boolean to determine if everything after b can generate epsilon
                bool successorsGenEp = true;
                // Once we have a b, traverse everything in the RHS that is after it
                for (int pastBIndex = bIndex + 1; pastBIndex < RHS_size; pastBIndex++)
                {
                    // If any symbol after b does not generate epsilon
                    if (!allFIRSTSets[0][RHS[pastBIndex]])
                    {
                        // Set the boolean to false, because everything after b does NOT generate epsilon,
                        // so we wouldn't apply rule 3
                        successorsGenEp = false;
                    }
                }
                
                // If everything after b CAN generate epsilon
                if (successorsGenEp)
                {
                    // Traverse allFOLLOWSets
                    for (int i = 0; i < FIRSTFOLLOWsize; i++)
                    {
                        // If there is anything in the LHS FOLLOW set that is not in our current
                        // bIndex of the RHS's FOLLOW set
                        if (allFOLLOWSets[i][LHS] && !allFOLLOWSets[i][RHS[bIndex]])
                        {
                            // Add the elements of LHS FOLLOW to RHS[bIndex] FOLLOW
                            allFOLLOWSets[i][RHS[bIndex]] = true;
                            // Set didUpdate to true so we know to keep going
                            didUpdate = true;
                        }
                    }
                }
            }
            /*********************/
            
            /*********************/
            // THIS SECTION OF CODE CHECKS FOR CALCULATING FOLLOW SETS RULE NUMBER 4
            /*********************/
            
            // For loop to traverse the RHS of the rule
            for (int RHSIndex = 0; RHSIndex < RHS_size - 1; RHSIndex++)
            {
                // If the symbol we are looking at is a non terminal
                if (RHS[RHSIndex] >= startSymbol)
                {
                    // Traverse the FIRST and FOLLOW sets of the RHS[RHSIndex + 1] and RHS[RHSIndex] respectively
                    for (int i = 1; i < FIRSTFOLLOWsize; i++)
                    {
                        // If there is anything in RHS[RHSIndex + 1]]'s FIRST set that is not
                        // in RHS[RHSIndex]]'s FOLLOW set
                        if (allFIRSTSets[i][RHS[RHSIndex + 1]] && !allFOLLOWSets[i][RHS[RHSIndex]])
                        {
                            // Add the element to RHS[RHSIndex]]'s FOLLOW set
                            allFOLLOWSets[i][RHS[RHSIndex]] = true;
                            // Update didUpdate so we know to keep going
                            didUpdate = true;
                        }
                    }
                }
            }
            /**********************/
            
            /**********************/
            // THIS SECTION OF CODE CHECKS FOR CALCULATING FOLLOW SETS RULE NUMBER 5
            /**********************/
            
            // For loop to traverse the RHS of the rule as the B index
            for (int bIndex = 0; bIndex < RHS_size - 2; bIndex++)
            {
                // If the B that we are looking at in the RHS is a non terminal
                if (RHS[bIndex] >= startSymbol)
                {
                    // Index to traverse the RHS after the B we are looking at
                    int afterBIndex = bIndex + 1;
                    
                    
                    // Boolean to determine whether or not we have seen a symbol after B that does not generate epsilon
                    // If we see a symbol that doesn't generate epsilon, we should exit the following inner loop and
                    // start with a new B index
                    bool haveSeenNonEp;
                    
                    do
                    {
                        // At the beginning of each loop through what follows B in the RHS, set haveSeenNonEp to false
                        haveSeenNonEp = false;
                        
                        // If a symbol after B generates epsilon
                        if (allFIRSTSets[0][RHS[afterBIndex]])
                        {
                            // For loop to traverse the FIRST and FOLLOW sets of RHS[afterBIndex + 1] and RHS
                            for (int i = 1; i < FIRSTFOLLOWsize; i++)
                            {
                                // If there is something in the FIRST set of whatever is after the epsilon
                                // generating symbols after B that is not in B's FOLLOW set
                                if (allFIRSTSets[i][RHS[afterBIndex + 1]] && !allFOLLOWSets[i][RHS[bIndex]])
                                {
                                    // Add the item to B's FOLLOW set
                                    allFOLLOWSets[i][RHS[bIndex]] = true;
                                    // Set didUpdate to true so we know to keep going
                                    didUpdate = true;
                                }
                            }
                            
                        }
                        // If we see a symbol after B that does NOT generate epsilon
                        else
                        {
                            // Set haveSeenNonEp to true so we will exit the loop
                            haveSeenNonEp = true;
                        }
                        
                        // Increment the after B index with each loop iteration
                        afterBIndex++;
                        
                    }while (!haveSeenNonEp && afterBIndex <= RHS_size - 2);
                    
                }
            }
            
            // Return the boolean indicating whether or not we updated any FOLLOW sets
            return didUpdate;
        
        }
    
        // Function to calculate the FIRST set of the RHS (used in determining if the grammar has a
        // recursive descent predictive parser)
        void calculateRHSFIRST(vector<bool> &FIRSTset, int FIRSTFOLLOWsize, vector<vector<bool> > allFIRSTSets)
        {
            
            /****************/
            // THIS SECTION OF CODE CHECKS FOR CalCULATING FIRST SETS RULE NUMBER 3
            /****************/
            for (int i = 0; i < FIRSTFOLLOWsize; i++)
            {
                // Add all elements of the first RHS symbol (RHS[0]) FIRST set to the first set
                if (allFIRSTSets[i][RHS[0]] && !FIRSTset[i] && i != 0)
                {
                    FIRSTset[i] = true;
                    
                }
            }
            /****************/
            
            
            
            /****************/
            // THIS SECTION OF CODE CHECKS FOR CALCULATING FIRST SETS RULE NUMBER 4
            /****************/
            
            // Boolean to determine whether or not we have seen a symbol on the RHS that does not
            // have # in its FIRST set
            bool haveSeenNonEpsilon = false;
            
            // For loop to traverse all indices of the RHS of the rule
            for (int index = 0; index < RHS_size; index++)
            {
                // If epsilon is in the first set of a leading character on the RHS
                if (allFIRSTSets[0][RHS[index]] && index != RHS_size - 1)
                {
                    // Traverse the RHS + 1 symbol's FIRST set
                    for (int j = 0; j < FIRSTFOLLOWsize; j++)
                    {
                        // If RHS[index + 1] has something in its FIRST set that the FIRST set we are creating doesn't,
                        // and it is not epsilon
                        if (allFIRSTSets[j][RHS[index + 1]] && !FIRSTset[j] && j != 0)
                        {
                            // Add it to FIRST set
                            FIRSTset[j] = true;
                            
                        }
                    }
                }
                // If we see a symbol in the RHS that does not have # in its FIRST set
                else
                {
                    // Set haveSeenNonEpsilon to true
                    haveSeenNonEpsilon = true;
                }
                
                // On each loop iteration, check if we have seen a symbol who doesn't have # in its FIRST set
                if (haveSeenNonEpsilon)
                {
                    // If we have, we don't want to add later symbols who could have # in their FIRST set to LHS first set,
                    // so we break from the loop
                    break;
                }
            }
            
            /****************/
            
            
            /****************/
            // THIS SECTION OF CODE CHECKS FOR CALCULATING FIRST SETS RULE NUMBER 5
            /****************/
            // Boolean to determine if a non-terminal can generate epsilon
            bool generatesEpsilon = true;
            
            // Loop through the rule's RHS
            for (int i = 0; i < RHS_size; i++)
            {
                // If any of its members does not have # in its FIRST set
                if (!allFIRSTSets[0][RHS[i]])
                {
                    // Set generates epsilon to false
                    generatesEpsilon = false;
                }
            }
            
            // If the RHS generates epsilon and we have not already seen that # is in the LHS FIRST set
            if (generatesEpsilon && !FIRSTset[0])
            {
                // Add # to the FIRST set
                FIRSTset[0] = true;
            }
            
        }
    
    
        // Function to print the contents of the rule
        void print()
        {
            cout << allSymbols[this->LHS];
            cout << " -> ";
            for (int i = 0; i < RHS_size; i++)
            {
                cout << allSymbols[this->RHS[i]] << " ";
            }
            cout << endl;
        }
};


// Create a vector to hold the universe for FIRST and FOLLOW sets
vector<string> FIRSTFOLLOWUniverse;
// Create a vector to hold all of the information for the rules
vector<Rule> allRules;

// Global count variables
int terminalCount = 0;
int nonTerminalCount = 0;
int allSymbolsSize = 0;
int startSymbol;
int FIRSTFOLLOWsize = 0;




// Function used to get the index of a string in the allSymbols vector
int getIndex(string seek)
{
    // Initialize index to -1
    int index = -1;
    
    // Loop through the allSymbols vector
    for (int i = 0; i < allSymbolsSize; i++)
    {
        // If the string is seen in the vector, set index to the index we are at in allSymbols
        if (strcmp(allSymbols[i].c_str(), seek.c_str()) == 0)
            {
                index = i;
            }
    }
    
    // Return the index we found
    return index;
}




int main (int argc, char* argv[])
{
    int task;
    
    if (argc < 2)
    {
        cout << "Error: missing argument\n";
        return 1;
    }
    
    /*
     Note that by convention argv[0] is the name of your executable,
     and the first argument to your program is stored in argv[1]
     */
    
    task = atoi(argv[1]);
    
    // TODO: Read the input grammar at this point from standard input
    
    // Create a new LexicalAnalyzer object to parse the input
    LexicalAnalyzer lex = *new LexicalAnalyzer();
    
    
    // Add the epsilon and end of file representations to the first two indices of the allSymbols vector
    allSymbols.push_back("#");
    allSymbols.push_back("$");
    
    
    // Boolean used to determine if the user entered any terminals at all
    bool hasTerminal = false;
    
    // Get the first token
    Token tok = lex.GetToken();
    
    // While the token is not HASH
    while (tok.token_type != HASH)
    {
        // Add the token lexeme to the allSymbols vector and get the next token
        allSymbols.push_back(tok.lexeme);
        tok = lex.GetToken();
        // If the while loop is ever entered, set hasTerminal to true
        hasTerminal = true;
        terminalCount++;
    }
    
    // At this point, everything in the allSymbols vector also makes up the universe for FIRST and FOLLOW
    // sets, so we can set FIRSTFOLLOWUniverse equal to allSymbols
    FIRSTFOLLOWUniverse = allSymbols;
    
    FIRSTFOLLOWsize = (int)FIRSTFOLLOWUniverse.size();
    
    // If there are no terminals entered
    if (!hasTerminal)
    {
        // Print error message and return
        cout << "Error: No terminals." << endl;
        return 1;
    }
    
    // Boolean used to determine if the user enters any non terminals
    bool hasNonTerminal = false;
    // Get the next token
    tok = lex.GetToken();
    
    // Boolean to determine if we are looking at the first non terminal (to find start symbol)
    bool firstNonTerminal = true;
    
    // While the next token is not HASH
    while (tok.token_type != HASH)
    {
        // Add the token lexeme to the allSymbols vector and get the next token
        allSymbols.push_back(tok.lexeme);
        
        // If we are looking at the first non terminal
        if (firstNonTerminal)
        {
            // Set start symbol to the index of the last element in allSymbols (since the last element we
            // put in was the first non terminal [the start symbol])
            startSymbol = (int)allSymbols.size() - 1;
            // Set firstNonTerminal to false so that we only get the first non terminal index
            firstNonTerminal = false;
        }
        tok = lex.GetToken();
        // If the while loop is ever entered, set hasNonTerminal to true
        hasNonTerminal = true;
        nonTerminalCount++;
    }
    
    // If there are no non terminals entered
    if (!hasNonTerminal)
    {
        // Print error message and return
        cout << "Error: No non-terminals." << endl;
        return 1;
    }
    
    // Calculate the size of the allSymbols vector. (# terminals + # nonterminals + 2 [# and $])
    allSymbolsSize = terminalCount + nonTerminalCount + 2;
    
    
    // Initialize a boolean to determine if the user enters any rules and an int to keep track
    // of how many rules are read in
    bool hasRule = false;
    int ruleListSize = 0;
    
    // Get the next token from the input
    tok = lex.GetToken();
    
    
    // This outer while loop is used to traverse through the rule-list
    // While the token type is not the DOUBLEHASH
    while (tok.token_type != DOUBLEHASH)
    {
        // If the while loop is ever entered, it means the user did enter at least 1 rule
        hasRule = true;
        
        // Create a new object of the Rule class
        Rule *newRule = new Rule();
        
        // Set the left hand side to the index of the token in the allSymbols vector
        newRule->setLHS(getIndex(tok.lexeme));
        
        // Get the next token
        tok = lex.GetToken();
        
        // If the next token is ARROW
        if (tok.token_type == ARROW)
        {
            // Get the token after the arrow
            tok = lex.GetToken();
            // Initialize a boolean to determine if the user enters anything on the right hand side
            // (if the right hand side is epsilon or not)
            bool hasRHS = false;
            
            // This inner while loop is used to traverse through the RHS of each rule WITHIN the rule list
            while (tok.token_type != HASH)
            {
                // If the loop is ever entered, we know the user entered a right hand side to the rule
                hasRHS = true;
                // Add the index of the token we got last to the RHS vector in the newRule
                newRule->addRHS(getIndex(tok.lexeme));
                // Get the next token
                tok = lex.GetToken();
            }
            
            // If there was nothing on the right hand side
            if (!hasRHS)
            {
                // Add the index of the epsilon representation from allSymbols to RHS of newRule
                newRule->addRHS(getIndex("#"));
            }
        }
        
        // After we read everything from the newRule, add it to the allRules vector and increment ruleListSize
        allRules.push_back(*newRule);
        ruleListSize++;
        
        // Get the next token to progress through the rule-list section of the input
        tok = lex.GetToken();
    }
    
    // If the user never entered any rules
    if (!hasRule)
    {
        // Print an error messagea and return
        cout << "Error: No rules." << endl;
        return 1;
    }
    
        
    
    /*
     Hint: You can modify and use the lexer from previous project
     to read the input. Note that there are only 4 token types needed
     for reading the input in this project.
     
     WARNING: You will need to modify lexer.cc and lexer.h to only
     support the tokens needed for this project if you are going to
     use the lexer.
     */
    
    switch (task) {
        case 1:
        {
            
            // For loop to traverse the allSymbols vector
            // It starts at index 2 because indices 0 and 1 are # and $ which shouldn't be counted
            for (int i = 2; i < allSymbolsSize; i++)
            {
                // For each symbol as we traverse the allSymbols vector, create a new count for the occurrences
                // of that symbol
                int aSymbolCount = 0;
                
                // For loop to traverse the rule list, searching for each symbol
                for (int j = 0; j < ruleListSize; j++)
                {
                    // If the rule we are looking at contains the symbol we are looking at
                    if (allRules[j].isInRule(allSymbols[i]))
                    {
                        //increment the current symbol's count
                        aSymbolCount++;
                    }
                }
                // Display each symbol and its count in the format specified
                cout << allSymbols[i] << ": " << aSymbolCount << endl;
            }
            
            break;
        }
            
        case 2:
        {
            
            // Create boolean vector to represent symbols that are generating
            vector<bool> generating(allSymbolsSize, false);
            //Create a boolean vector to represent the symbols that are reachable
            vector<bool> reachable(allSymbolsSize, false);
            // Set first index of generating to true (it will always be the #)
            generating[0] = true;
            
            // Start at index 2 (the first terminal) and iterate up to terminalCount + 2 because
            // that will be the index of the last terminal
            for (int i = 2; i < terminalCount + 2; i++)
            {
                // Set the terminal indexes in the generating vector to true
                generating[i] = true;
            }
            
            // boolean to determine if changes were made so we know when to stop
            bool changeMade;
            
            do
            {
                // Set changeMade to false for the beginning
                changeMade = false;
                
                // For loop to traverse the list of rules
                for (int rulesIndex = 0; rulesIndex < ruleListSize; rulesIndex++)
                {
                    // If the rule we are looking at has a LHS that we can find to be generating AND if
                    // we have not already seen that the LHS can be generating
                    if (allRules[rulesIndex].isGenerating(generating) && generating[allRules[rulesIndex].getLHS()] == false)
                    {
                        // Make the change and set generating at the index of the rule's LHS to true
                        generating[allRules[rulesIndex].getLHS()] = true;
                        // Set changemade to true so we know to go around the do-while loop again
                        changeMade = true;
                    }
                }
                
            }while (changeMade); // Loop while changes are still being made
            
            // Traverse the allRules vector and "eliminate" any rules that include a symbol
            // that is not generating
            for (int i = 0; i < ruleListSize; i++)
            {
                if (allRules[i].hasSymbolNotGenerating(generating))
                {
                    allRules[i].eliminate();
                }
            }
            
            // Set reachable at index of startSymbol to true
            reachable[startSymbol] = true;
            
            do
            {
                // Set changeMade to false at start of every loop
                changeMade = false;
                
                // Traverse the allRules vector
                for (int rulesIndex = 0; rulesIndex < ruleListSize; rulesIndex++)
                {
                    // If the rule has not been eliminated
                    if (!allRules[rulesIndex].isEliminated())
                    {
                        // If the LHS of the rule is reachable and we have not set the RHS of this rule to
                        // reachable already
                        if (reachable[allRules[rulesIndex].getLHS()] && !allRules[rulesIndex].hasBeenSeen())
                        {
                            // Set the RHS of the rule to reachable
                            allRules[rulesIndex].setRHSReachable(reachable);
                            // Set changeMade to true so that we keep looping
                            changeMade = true;
                        }
                    }
                }
                
            }while (changeMade == true);
            
            
            // Boolean vector to show which symbols are useless and which ones aren't
            vector<bool> useless(allSymbolsSize, false);
            
            // Traverse generating and reachable vectors
            for (int i = 0; i < allSymbolsSize; i++)
            {
                // If an index is not generating or not reachable
                if (!generating[i] || !reachable[i])
                {
                    // Set its value in useless to true
                    useless[i] = true;
                }
            }
            
            // For loop traverses the vector containing all rules
            for (int i = 0; i < ruleListSize; i++)
            {
                // If the rule we are looking at is not eliminated and it should be displayed (meaning
                // no symbols in the rule are useless)
                if (!allRules[i].isEliminated() && allRules[i].shouldBeDisplayed(useless))
                {
                    // Print the rule
                    allRules[i].print();
                }
            }
            
            break;
        }
            
        case 3:
        {
            
            // Create a 2D bool vector to hold all FIRST sets and initialize all entries to false
            vector <vector <bool> > allFIRSTSets;
            allFIRSTSets.resize(FIRSTFOLLOWsize, vector<bool>(allSymbolsSize, false));
            
            // We know that epsilon will always be in column 0 row 0, so we can initialize it as its own FIRST set
            allFIRSTSets[0][0] = true;
            
            // This loops through only the terminal indexes and initializes each terminal FIRST
            // set to itself
            for (int i = 2; i < terminalCount + 2; i++)
            {
                allFIRSTSets[i][i] = true;
            }
            
            //Boolean used to determine if we have made any changes to the FIRST sets, so we know when to
            // stop looping
            bool changeMade;
            
            do
            {
                // Initialize changeMade to false at the top of every loop
                changeMade = false;
                
                // Iterate through the rules
                for (int i = 0; i < ruleListSize; i++)
                {
                    // If the rule we are looking at updates anything in the FIRST sets
                    if (allRules[i].UpdateFIRST(allFIRSTSets, FIRSTFOLLOWsize))
                    {
                        changeMade = true;
                    }
                }
                
            }while(changeMade);
            
            
            // Outer for loop used to index all of the non-terminals whose FIRST
            // sets we need to print
            for (int i = terminalCount + 2; i < allSymbolsSize; i++)
            {
                // Initialize each set's size to 0
                int setSize = 0;
                
                cout << "FIRST(" << allSymbols[i] << ") = { ";
                
                // First, loop through the set and see how many items are in it
                for (int j = 0; j < FIRSTFOLLOWsize; j++)
                {
                    if (allFIRSTSets[j][i])
                    {
                        // Increment set size
                        setSize++;
                    }
                }
                
                // Int used to keep track of how many items in the set we have printed
                int printedCount = 0;
                for (int j = 0; j < FIRSTFOLLOWsize; j++)
                {
                    // If a symbol is included in a non-terminal's FIRST set
                    if (allFIRSTSets[j][i])
                    {
                        // Print the item
                        cout << FIRSTFOLLOWUniverse[j];
                        // Increment the number of things we have printed
                        printedCount++;
                        
                        // If we didn't just print the last item
                        if (printedCount < setSize)
                        {
                            // Print a comma
                            cout << " , ";
                        }
                        else
                        {
                            // If we just printed the last item in the set, print only a space
                            cout << " ";
                        }
                    }
                    
                    
                }
                
                cout << "}" << endl;
            }
            
            
            
            
            break;
        }
        case 4:
        {
            // TODO: perform task 4.
            
            // In order to calculate FOLLOW sets, we must first calculate the FIRST sets
            
            // Create a 2D bool vector to hold all FIRST sets and initialize all entries to false
            vector <vector <bool> > allFIRSTSets;
            allFIRSTSets.resize(FIRSTFOLLOWsize, vector<bool>(allSymbolsSize, false));
            
            // We know that epsilon will always be in column 0 row 0, so we can initialize it as its own FIRST set
            allFIRSTSets[0][0] = true;
            
            // This loops through only the terminal indexes and initializes each terminal FIRST
            // set to itself
            for (int i = 2; i < terminalCount + 2; i++)
            {
                allFIRSTSets[i][i] = true;
            }
            
            //Boolean used to determine if we have made any changes to the FIRST sets, so we know when to
            // stop looping
            bool changeMade;
            
            do
            {
                // Initialize changeMade to false at the top of every loop
                changeMade = false;
                
                // Iterate through the rules
                for (int i = 0; i < ruleListSize; i++)
                {
                    // If the rule we are looking at updates anything in the FIRST sets
                    if (allRules[i].UpdateFIRST(allFIRSTSets, FIRSTFOLLOWsize))
                    {
                        changeMade = true;
                    }
                }
                
            }while(changeMade);
            
            
            
            // Now that we have the FIRST sets, we can work on obtaining the FOLLOW sets
            
            // Create a 2D bool vector to hold the FOLLOW sets of all symbols. Initialize everything to false
            vector<vector<bool> > allFOLLOWSets;
            allFOLLOWSets.resize(FIRSTFOLLOWsize, vector<bool>(allSymbolsSize, false));
            
            // Add eof to the start symbol's FOLLOW set
            allFOLLOWSets[1][startSymbol] = true;
            
            // Loop to determine if we are still making changes to the FOLLOW sets
            do
            {
                // Initially, set changeMade to false
                changeMade = false;
                
                // Traverse the rule list
                for (int i = 0; i < ruleListSize; i++)
                {
                    // If the rule we are looking at can update the FOLLOW sets
                    if (allRules[i].updateFOLLOW(allFOLLOWSets, allFIRSTSets, FIRSTFOLLOWsize, startSymbol))
                    {
                        // Set changeMade to true
                        changeMade = true;
                    }
                }
                
            }while(changeMade);
            
            
            // Outer for loop used to index all of the non-terminals whose FOLLOW
            // sets we need to print
            for (int i = terminalCount + 2; i < allSymbolsSize; i++)
            {
                // Initialize each set's size to 0
                int setSize = 0;
                
                cout << "FOLLOW(" << allSymbols[i] << ") = { ";
                
                // First, loop through the set and see how many items are in it
                for (int j = 0; j < FIRSTFOLLOWsize; j++)
                {
                    if (allFOLLOWSets[j][i])
                    {
                        // Increment set size
                        setSize++;
                    }
                }
                
                // Int used to keep track of how many items in the set we have printed
                int printedCount = 0;
                for (int j = 0; j < FIRSTFOLLOWsize; j++)
                {
                    // If a symbol is included in a non-terminal's FIRST set
                    if (allFOLLOWSets[j][i])
                    {
                        // Print the item
                        cout << FIRSTFOLLOWUniverse[j];
                        // Increment the number of things we have printed
                        printedCount++;
                        
                        // If we didn't just print the last item
                        if (printedCount < setSize)
                        {
                            // Print a comma
                            cout << " , ";
                        }
                        else
                        {
                            // If we just printed the last item in the set, print only a space
                            cout << " ";
                        }
                    }
                    
                    
                }
                
                cout << "}" << endl;
            }
            
            break;
        }
            
        case 5:
        {
            // In order to determine if a grammar has a recursive descent predictive parser, we must first
            // calculate the FIRST and FOLLOW sets
            
            // Create a 2D bool vector to hold all FIRST sets and initialize all entries to false
            vector <vector <bool> > allFIRSTSets;
            allFIRSTSets.resize(FIRSTFOLLOWsize, vector<bool>(allSymbolsSize, false));
            
            // We know that epsilon will always be in column 0 row 0, so we can initialize it as its own FIRST set
            allFIRSTSets[0][0] = true;
            
            // This loops through only the terminal indexes and initializes each terminal FIRST
            // set to itself
            for (int i = 2; i < terminalCount + 2; i++)
            {
                allFIRSTSets[i][i] = true;
            }
            
            //Boolean used to determine if we have made any changes to the FIRST sets, so we know when to
            // stop looping
            bool changeMade;
            
            do
            {
                // Initialize changeMade to false at the top of every loop
                changeMade = false;
                
                // Iterate through the rules
                for (int i = 0; i < ruleListSize; i++)
                {
                    // If the rule we are looking at updates anything in the FIRST sets
                    if (allRules[i].UpdateFIRST(allFIRSTSets, FIRSTFOLLOWsize))
                    {
                        changeMade = true;
                    }
                }
                
            }while(changeMade);
            
            
            
            // Now that we have the FIRST sets, we can work on obtaining the FOLLOW sets
            
            // Create a 2D bool vector to hold the FOLLOW sets of all symbols. Initialize everything to false
            vector<vector<bool> > allFOLLOWSets;
            allFOLLOWSets.resize(FIRSTFOLLOWsize, vector<bool>(allSymbolsSize, false));
            
            // Add eof to the start symbol's FOLLOW set
            allFOLLOWSets[1][startSymbol] = true;
            
            // Loop to detemine if we are done updating FOLLOW sets
            do
            {
                // Initially set changeMade to false
                changeMade = false;
                
                // Loop through the rule list
                for (int i = 0; i < ruleListSize; i++)
                {
                    // If the rule we are looking at can update any FOLLOW sets
                    if (allRules[i].updateFOLLOW(allFOLLOWSets, allFIRSTSets, FIRSTFOLLOWsize, startSymbol))
                    {
                        // Set changeMade to true
                        changeMade = true;
                    }
                }
                
            }while(changeMade);
            
            
            // Boolean to determine if the grammar has a recursive descent predictive parser
            bool hasRDPP = true;
            
            /********************/
            // THIS SECTION OF CODE CHECKS FOR CONDITION 1 IN DETERMINING IF THE GRAMMAR HAS A
            // RECURSIVE DESCENT PREDICTIVE PARSER
            /********************/
            
            // Traverse the FIRST sets of the non terminals, starting at the start symbol
            for (int i = startSymbol; i < allSymbolsSize; i++)
            {
                // If the non terminal can generate epsilon in one or more steps
                if (allFIRSTSets[0][i])
                {
                    // Traverse the FIRST and FOLLOW sets for that non terminal
                    for (int j = 0; j < FIRSTFOLLOWsize; j++)
                    {
                        // If both the FIRST and the FOLLOW set contain the same item
                        if (allFIRSTSets[j][i] && allFOLLOWSets[j][i])
                        {
                            // Set hasRDPP to false, because the intersection of the FIRST and FOLLOW
                            // sets does not yield the empty set
                            hasRDPP = false;
                        }
                    }
                }
            }
            /********************/
            
            
            /********************/
            // THIS SECTION OF CODE CHECKS FOR CONDITION 2 IN DETERMINING IF THE GRAMMAR HAS A RECURSIVE
            // DESCENT PREDICTIVE PARSER
            /********************/
            
            // Outer for loop traverses the rule list
            for (int i = 0; i < ruleListSize; i++)
            {
                // the inside for loop starts are the next rule in the list to compare LHS's
                for (int j = i + 1; j < ruleListSize; j++)
                {
                    // If two different rules share the same LHS
                    if (allRules[i].getLHS() == allRules[j].getLHS())
                    {
                        // Create two new boolean vectors to hold the FIRST sets of the ith rule and
                        // the jth rule
                        vector<bool> iRuleFIRST(FIRSTFOLLOWsize, false);
                        vector<bool> jRuleFIRST(FIRSTFOLLOWsize, false);
                        
                        // Calculate the FIRST sets for the RHS of both of the rules
                        allRules[i].calculateRHSFIRST(iRuleFIRST, FIRSTFOLLOWsize, allFIRSTSets);
                        allRules[j].calculateRHSFIRST(jRuleFIRST, FIRSTFOLLOWsize, allFIRSTSets);
                        
                        // Traverse the FIRST sets of the RHS of both rules
                        for (int index = 0; index < FIRSTFOLLOWsize; index++)
                        {
                            // If the intersection fo the FIRST sets yields anything but empty
                            if (iRuleFIRST[index] && jRuleFIRST[index])
                            {
                                // Sets hasRDPP to false
                                hasRDPP = false;
                            }
                        }
                    }
                }
            }
            
            
            
            
            // If the language has a recursive descent predictive parser
            if (hasRDPP)
            {
                // Print YES
                cout << "YES" << endl;
            }
            // Otherwise
            else
            {
                // Print NO
                cout << "NO" << endl;
            }
            
            break;
        }
            
        default:
        {
            cout << "Error: unrecognized task number " << task << "\n";
            break;
        }
    }
    return 0;
}

