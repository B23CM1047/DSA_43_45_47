# DSA_43_45_47

#include <iostream>
#include <vector>
using namespace std;


struct SuffixTreeNode {
    
    SuffixTreeNode* children[26];
    int startIndex; 

   
    SuffixTreeNode(int index) : startIndex(index) {
        
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
};

class SuffixTree {
private:
    SuffixTreeNode* root; 
    string text;          

    
    void insertSuffix(int suffixStart) {
        SuffixTreeNode* currentNode = root;

       
        for (int i = suffixStart; i < text.length(); i++) {
            char currentChar = text[i];
            int index = currentChar - 'a'; 

            
            if (currentNode->children[index] == nullptr) {
               
                currentNode->children[index] = new SuffixTreeNode(suffixStart);
            }

            
            currentNode = currentNode->children[index];
        }
    }

    
    void printTree(SuffixTreeNode* node, int depth) {
       
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != nullptr) {
                char edgeChar = 'a' + i; 
                cout << string(depth, '-') << edgeChar << " (" << node->children[i]->startIndex << ")" << endl;
                
                printTree(node->children[i], depth + 1);
            }
        }
    }

public:
   
    SuffixTree(const string& s) : text(s) {
        root = new SuffixTreeNode(-1); 
        buildSuffixTree();
    }

    
    void buildSuffixTree() {
        for (int i = 0; i < text.length(); i++) {
            insertSuffix(i); 
        }
    }

   
    void printSuffixTree() {
        cout << "Suffix Tree Structure:" << endl;
        printTree(root, 0); 
    }
};

int main() {
    string text;
    cin >> text;

    SuffixTree suffixTree(text);

   
    suffixTree.printSuffixTree();

    return 0;
}
