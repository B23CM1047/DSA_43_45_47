#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>

using namespace std;

struct SuffixTreeNode {
    SuffixTreeNode* children[256];
    int startIndex;
    int stringNumber;

    SuffixTreeNode(int index, int strNum) : startIndex(index), stringNumber(strNum) {
        for (int i = 0; i < 256; i++) {
            children[i] = nullptr;
        }
    }
};

class SuffixTree {
private:
    SuffixTreeNode* root;
    string text;
    int stringNumber;

    void insertSuffix(int suffixStart) {
        SuffixTreeNode* currentNode = root;
        for (int i = suffixStart; i < text.length(); i++) {
            char currentChar = text[i];
            if (currentNode->children[(unsigned char)currentChar] == nullptr) {
                currentNode->children[(unsigned char)currentChar] = new SuffixTreeNode(suffixStart, stringNumber);
            }
            currentNode = currentNode->children[(unsigned char)currentChar];
        }
    }

    void buildSuffixTree() {
        for (int i = 0; i < text.length(); i++) {
            insertSuffix(i);
        }
    }

public:
    SuffixTree(const string& s, int strNum) : text(s), stringNumber(strNum) {
        root = new SuffixTreeNode(-1, strNum);
        buildSuffixTree();
    }

    SuffixTreeNode* getRoot() {
        return root;
    }

    string getText() const {
        return text;
    }
};

// Recursive helper function to find the longest common substring
int findLCS(SuffixTreeNode* node1, SuffixTreeNode* node2, int depth, int currentStart, int& maxLength, int& startIdx) {
    if (!node1 || !node2) return depth;
    int currentDepth = depth;
    for (int i = 0; i < 256; i++) {
        if (node1->children[i] && node2->children[i]) {
            int newDepth = findLCS(node1->children[i], node2->children[i], depth + 1, node1->children[i]->startIndex, maxLength, startIdx);
            if (newDepth > maxLength) {
                maxLength = newDepth;
                startIdx = currentStart;
            }
            currentDepth = max(currentDepth, newDepth);
        }
    }
    return currentDepth;
}

// Function to compare two suffix trees and find the longest common substring
string getLongestCommonSubstring(SuffixTree& tree1, SuffixTree& tree2) {
    int maxLength = 0;
    int startIdx = -1;
    findLCS(tree1.getRoot(), tree2.getRoot(), 0, 0, maxLength, startIdx);
    return (startIdx == -1 || maxLength == 0) ? "" : tree1.getText().substr(startIdx, maxLength);
}

// Save each person's DNA, gender, and name in DNA bank
void saveToDNABank(const string& name, const string& dna, const string& gender, const string& password) {
    ofstream outFile("DNA_bank.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "Name: " << name << "\n";
        outFile << "Gender: " << gender << "\n";
        outFile << "DNA Sequence: " << dna << "\n";
        outFile << "Password: " << password << "\n";
        outFile << "--------------------------\n";
        outFile.close();
        cout << "Information saved to DNA_bank.txt\n";
    } else {
        cout << "Error: Unable to open file.\n";
    }
}

// Load DNA information from file by name
bool loadFromDNABank(const string& name, string& dna, string& gender) {
    ifstream inFile("DNA_bank.txt");
    if (!inFile.is_open()) {
        cout << "Error: Unable to open DNA_bank.txt\n";
        return false;
    }
    
    string line, storedPassword, enteredPassword;
    bool found = false;
    while (getline(inFile, line)) {
        if (line.find("Name: " + name) != string::npos) {
            found = true;
            getline(inFile, line);  // Read gender line
            gender = line.substr(8); // Extract gender
            getline(inFile, line);   // Read DNA sequence line
            dna = line.substr(13);   // Extract DNA sequence
            getline(inFile, line);   // Read password line
            storedPassword = line.substr(10); // Extract password
            
            // Ask for password verification
            cout << "Enter password " << name << ": ";
            cin >> enteredPassword;

            if (enteredPassword == storedPassword) {
                inFile.close();
                return true;
            } else {
                cout << "Incorrect password. Access denied.\n";
                inFile.close();
                return false;
            }
        }
    }
    inFile.close();
    if (!found) cout << "No data found for " << name << ".\n";
    return false;
}
// Check if a name already exists in DNA_bank.txt
bool isNameExisting(const string& name) {
    ifstream inFile("DNA_bank.txt");
    if (!inFile.is_open()) {
        cout << "Error: Unable to open DNA_bank.txt\n";
        return false;
    }
    
    string line;
    while (getline(inFile, line)) {
        if (line.find("Name: " + name) != string::npos) {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

// Input function with choice to load from file or enter new information
void getPersonInfo(string& name, string& dna, string& gender, int personNumber) {
    int choice;
    cout << "For person " << personNumber << ", choose option:\n";
    cout << "1. Use stored DNA data\n";
    cout << "2. Enter new DNA data\n";
    cout << "Enter choice (1 or 2): ";
    cin >> choice;
    
    if (choice == 1) {
        cout << "Enter name of person " << personNumber << ": ";
        cin >> name;
        if (!loadFromDNABank(name, dna, gender)) {
            cout << "No data found for " << name << ". Please enter details manually.\n";
            choice = 2;
        }
    }

    if (choice == 2) {
        string password;
        do {
            cout << "Enter a unique name for person " << personNumber << ": ";
            cin >> name;
            if (isNameExisting(name)) {
                cout << "The name \"" << name << "\" already exists. Please choose another name.\n";
            }
        } while (isNameExisting(name));
        
        cout << "Enter DNA gene code of " << name << ": ";
        cin >> dna;
        for (char &c : dna) {
            if (islower(c)) c = toupper(c);
        }
        cout << "Enter gender of " << name << ": ";
        cin >> gender;
        
        cout << "Set a unique password for this entry: ";
        cin >> password;

        saveToDNABank(name, dna, gender, password);
    }
}

float calculateMatchPercentage(const string& dna1, const string& dna2) {
    //int minLength = min(dna1.length(), dna2.length());
    int matchCount = 0;

    // Count matches at each corresponding position
    for (int i = 0; i < dna1.length(); i++) {
        if (dna1[i] == dna2[i]) {
            matchCount++;
        }
    }

    // Calculate match percentage based on the length of the shorter DNA
    float matchPercentage = (static_cast<float>(matchCount) / dna1.length()) * 100;
    return matchPercentage;
}

int main() {
   
    string text1, text2, gen1, gen2, name1, name2;
     
    // Get info for person 1
    getPersonInfo(name1, text1, gen1, 1);

    // Get info for person 2
    getPersonInfo(name2, text2, gen2, 2);

    
    // Build suffix trees and find LCS
    auto start1 = chrono::high_resolution_clock::now();
    SuffixTree suffixTree1(text1, 1);
    SuffixTree suffixTree2(text2, 2);

    string lcs = getLongestCommonSubstring(suffixTree1, suffixTree2);
    float ratio = 0;
    float matchPercentage = 0;

    auto end1 = chrono::high_resolution_clock::now();
    chrono::duration <double, std::milli> duration1 = end1 - start1;
    cout << "Execution Time: " << fixed << setprecision(3) << duration1.count() << " ms" << endl;


    // Calculate match percentage
    if (text1.length() == text2.length()) {
        ratio = (float)lcs.length() / text1.length();
        matchPercentage = ratio * 100;
    }

    
    cout << "   Comparison Results:\n";
    if (lcs.empty()) {
        cout << "   - No common substring found.\n";
    } else {
        cout << "   Longest Common Substring : " << lcs << "\n";
        cout << "   Length of LCS            : " << lcs.length() << " characters\n";
    }

    if (text1.length() == text2.length()) {
        cout << "   Match Percentage using suffix trees       : " << fixed << setprecision(2) << matchPercentage << "%\n";
    
     cout << "Execution Time: " << duration1.count() << " ms" << endl;

    }

        
    int check;
    cout << "Do you compare the DNA's using string comparison? ";
    cout << "1. yes" <<endl <<"2. No" <<endl;
    cin >> check;
    if(check==1) {
    auto start = chrono::high_resolution_clock::now();

    float MatchPercentage11 = calculateMatchPercentage(text1, text2);
    cout << "Match Percentage using string comparison w.r.t person 1: " << fixed << setprecision(2) << MatchPercentage11 << "%\n";
    float MatchPercentage12 = calculateMatchPercentage(text2, text1);
    cout << "Match Percentage using string comparison w.r.t person 2: " << fixed << setprecision(2) << MatchPercentage12 << "%\n";

    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, std::milli> duration = end - start;
    cout << "Execution Time: " << duration.count() << " ms" << endl;

     cout << "Comparison between 2 DNA s is successfully Done";
    } 
    else {
        cout << "Comparison between 2 DNA s is successfully Done";
    }
      
    return 0;
}
