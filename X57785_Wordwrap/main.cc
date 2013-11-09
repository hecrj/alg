#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

/**
 * Represents a paragraph.
 */
class Paragraph
{
    /**
     * Words of the paragraph.
     * From now on:
     * n = words.size()
     */
    vector<string> words;
    
    /**
     * Given 0 <= i < n:
     * lines[i] = k, where words[k] is the word that should start the line where
     * words[i] is located when words[0..i] are optimally arranged
     */
    vector<int> lines;
    
    /**
     * Given 0 <= i < n:
     * optimalCosts[i] = optimal cost of arranging the words[0..i]
     */
    vector<int> optimalCosts;
    
    /**
     * Given 0 <= i < n:
     * Given i < j < n:
     * lineCosts[i][j] = cost of having words[i..j] on the same line
     */
    vector< vector<int> > lineCosts;
    
    /**
     * Calculates the line costs given the target line width.
     * 
     * Cost:
     * Cost(compute line widths) + Cost(compute line costs) = O(n^2) + O(n^2) = O(n^2)
     * More details below.
     * 
     * @param width Target line width
     */
    void computeLineCosts(int width)
    {
        // Initialize lineCosts structure
        lineCosts = vector< vector<int> >(words.size(), vector<int>(words.size()));
        
        // To save some time and memory, we reuse the lineCosts structure to calculate
        // the line widths first, and then we calculate the line costs in the same
        // structure.
        
        // Line widths
        // Given 0 <= i < n:
        // Calculate the width for each line that starts with words[i]
        for(int i = 0; i < words.size(); ++i)
        {
            // Base case
            // Line width of the line that only contains words[i] equals to words[i] width
            lineCosts[i][i] = words[i].size();
            
            // Given i < j < n
            // The width of the line that starts with words[i] and ends with words[j] can be
            // defined recursively as the sum of:
            // 1. The width of the line that starts with words[i] and ends with words[j-1]
            // 2. Width of words[j]
            // 3. 1 (because the additional space separator)
            for(int j = i+1; j < words.size(); ++j)
            {
                lineCosts[i][j] = lineCosts[i][j-1] + words[j].size() + 1;
                
                // We are doing constant work O(1) for every j. Thus, the cost of this loop is:
                // (n - (i+1)) * O(1) = O(n)
            }
            
            // Cost of this loop:
            // n * O(n) = O(n^2)
        }
        
        // Line costs
        // Given 0 <= i < words.size():
        // Calculate the cost for each line that starts with words[i]
        for(int i = 0; i < lineCosts.size(); ++i)
        {
            // Given i < j < words.size()
            for(int j = i; j < lineCosts.size(); ++j)
            {
                // The cost of the line that starts with words[i] and ends with words[j] is:
                // (width of the line - target line width) ^ 2 
                
                // lineCosts[i][j] contains the cost of the line (previously calculated)
                
                // We calculate here the square root of the cost
                int cost_2 = lineCosts[i][j] - width;
                
                // Finally, the cost is calculated from the square root
                lineCosts[i][j] = cost_2 * cost_2;
            }
            
            // This loop has the same structure as the loop above, thus the cost is:
            // O(n^2)
        }
    }
    
    /**
     * Prints the paragraph words[0..index] in lines recursively.
     * 
     * Cost:
     * Constant work for every words[0..index] => O(index)
     * 
     * @param index Index of the last word to print
     */
    void printLines(int index)
    {
        // Base case
        if(index < 0)
            return;
        
        // lines[index] - 1 is the word before the actual line
        // We can use recursivity to print words[0..lines[index]-1] correctly (by induction).
        printLines(lines[index] - 1);
        
        // Now, we only need to print the current line that contains words[lines[index]..index]
        for(int i = lines[index]; i <= index; ++i)
        {
            cout << words[i];
            
            if(i != index)
                cout << ' ';
        }
        
        cout << endl;
    }
    
public:
    /**
     * Reads a paragraph from the default input stream.
     * It skips empty lines until some word is read, then an
     * empty line marks the end of the paragraph.
     *
     * Cost: O(n), where n is the number of words read
     */
    void read()
    {
        string s;
        
        while(getline(cin, s))
        {
            if(s.empty() and not empty())
                break;
            
            istringstream line(s);

            while(line >> s)
                words.push_back(s);
        }
        
        // Paragraph consists in only one line by default
        // Every word is in the same line of the first word
        lines = vector<int>(words.size(), 0);
    }
    
    /**
     * Tells whether the paragraph is empty or not.
     * @return True if the paragraph does not have any word, false otherwise
     */
    bool empty()
    {
        return words.size() < 1;
    }
    
    /**
     * Formats the words of the paragraphs in lines optimally (lowest penalty).
     * Detailed information: https://www.jutge.org/problems/X57785_es/statement
     * 
     * Every optimum arrangement can be described recursively:
     * If n = 1, the optimum arrangement is a line with the word itself.
     * If n > 1, there exists k: 0 <= k < n, such that the optimum arrangement is:
     * optimumArrangement(k)
     * words[k+1..n]
     * 
     * Thus, we can find the optimum arrangement for n words recursively:
     * 1. Find the optimum arrangement for k words, for every k: 0 <= k < n.
     * 2. Find the optimum arrangement for n words:
     *    - For every words[i] try to add a line with the words[i..n] to the arrangement
     *      i-1.
     *    - The optimum arrangement is the one with the lowest cost.
     * 
     * Cost:
     * The cost is the sum of:
     * 1. Cost of computeLineCosts = O(n^2)
     * 2. Cost of main loop:
     *    sum{j=0 -> n}( sum{i=1 -> j}( O(1) ) ) = O(n^2)
     * 
     * Thus, the total cost is:
     * O(n^2) + O(n^2) = O(n^2)
     * 
     * @param width Target line width
     * @return The penalty/cost of arranging the words[0..n-1] optimally
     */
    int wordwrap(int width)
    {
        // First, compute the line costs
        computeLineCosts(width);
        
        // Initialize optimal costs
        optimalCosts = vector<int>(words.size());
        
        // Given 0 <= j < words.size()
        // Calculate the optimal cost of arranging the words[0..j]
        for(int j = 0; j < words.size(); ++j)
        {
            // Set default minimum cost and index
            int min_cost = lineCosts[0][j];
            int min_index = 0;
            
            // First iteration can be skipped because the default
            // minimum cost.
            // 
            // Given 0 < i <= j:
            // Check if there exists a better way to arrange the words[0..j] putting
            // the words[i..j] on a new line. If it exists, update min_cost and min_index
            // accordingly.
            //
            // Invariants:
            // 1. min_cost  = minimum cost of arranging the words[0..j] considering
            //    having the words[k..j] on its own line, for every 1 <= k <= i
            // 2. min_index = index of the word that should start the line that produces
            //    min_cost
            for(int i = 1; i <= j; ++i)
            {
                // Cost of adding the line with words[i..j] to the optimum arrangement
                // of words[0..i-1]
                int cost = optimalCosts[i-1] + lineCosts[i][j];
                
                // If necessary, update minimum cost and index to fulfill invariant
                if(cost < min_cost)
                {
                    min_cost = cost;
                    min_index = i;
                }
            }
            
            // Note that we only update the min_cost if the cost of adding the new line is
            // strictly less than the min_cost. This guarantees that if there is more than one
            // optimum (sub)arrangement the one that has more words in the last line is selected,
            // because default min_index is 0 and variable i is always incremented.
            
            // By invariant and recursive approach:
            // 1. min_cost  = minimum cost of arranging the words[0..j]
            // 2. min_index = index of the word that should start the line that produces
            //    min_cost 
            optimalCosts[j] = min_cost;
            lines[j] = min_index;
        }
        
        // Return the penalty/cost of arranging the words[0..n-1] optimally
        return optimalCosts[words.size() - 1];
    }
    
    /**
     * Prints the paragraph in the default output stream.
     * 
     * Cost:
     * Cost of printLines(n) = O(n)
     */
    void print()
    {
        printLines(words.size() - 1);
    }
};

/**
 * Reads a target line width and paragraphs from the default input stream.
 * Prints every read paragraph optimally formatted and its penalty.
 * 
 * Solution to the problem: https://www.jutge.org/problems/X57785_es/statement
 * @return Execution status
 */
int main()
{
    // Read target line width
    int width;
    cin >> width;
    
    // First paragraph?
    bool first = true;
    
    // While not end of file/input reached
    while(!cin.eof())
    {
        // Read a new paragraph
        Paragraph p;
        p.read();
        
        // An empty paragraph means the input has been closed before
        // reading any word
        if(not p.empty())
        {
            // Wordwrap paragraph and get penalty
            int penalty = p.wordwrap(width);
            
            // Print separator, if necessary
            if(first) first = false;
            else cout << endl;
            
            // Print paragraph and penalty
            p.print();
            cout << "Penalty: " << penalty << endl;
        }
    }
    
    return 0;
}
