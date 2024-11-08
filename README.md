#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
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
void saveToDNABank(const string& name, const string& dna, const string& gender) {
    ofstream outFile("DNA_bank.txt", ios::app);
    if (outFile.is_open()) {
        outFile << "Name: " << name << "\n";
        outFile << "Gender: " << gender << "\n";
        outFile << "DNA Sequence: " << dna << "\n";
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
    
    string line;
    bool found = false;
    while (getline(inFile, line)) {
        if (line.find("Name: " + name) != string::npos) {
            found = true;
            getline(inFile, line);  // Read gender line
            gender = line.substr(8); // Extract gender
            getline(inFile, line);   // Read DNA sequence line
            dna = line.substr(13);   // Extract DNA sequence
            break;
        }
    }
    inFile.close();
    return found;
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
          if (islower(c)) {
            c = toupper(c);
         }
      }
        cout << "Enter gender of " << name << ": ";
        cin >> gender;
        saveToDNABank(name, dna, gender);
    }
}

void retrieveDNAInfo() {
    string name, dna, gender;
    cout << "Enter the name to search in DNA bank: ";
    cin >> name;
    if (loadFromDNABank(name, dna, gender)) {
        cout << "\n=== DNA Information ===\n";
        cout << "Name: " << name << "\n";
        cout << "Gender: " << gender << "\n";
        cout << "DNA Sequence: " << dna << "\n";
    } else {
        cout << "No data found for " << name << ".\n";
    }
}

void addNewDNA() {
    string name, dna, gender;
    do {
        cout << "Enter a unique name: ";
        cin >> name;
        if (isNameExisting(name)) {
            cout << "The name \"" << name << "\" already exists. Please choose another name.\n";
        }
    } while (isNameExisting(name));
    
    cout << "Enter DNA gene code: ";
    cin >> dna;
    for (char &c : dna) {
        if (islower(c)) c = toupper(c);
    }
    cout << "Enter gender: ";
    cin >> gender;
    saveToDNABank(name, dna, gender);
}

void deleteFromDNABank(const string& name) {
    ifstream inFile("DNA_bank.txt");
    if (!inFile.is_open()) {
        cout << "Error: Unable to open DNA_bank.txt\n";
        return;
    }

    // Temporary storage for lines to keep
    vector<string> lines;
    string line;
    bool found = false;

    // Read through the file and save lines that don't match the specified name
    while (getline(inFile, line)) {
        if (line.find("Name: " + name) != string::npos) {
            found = true;
            // Skip lines for the entire record of the found name
            getline(inFile, line); // Skip gender line
            getline(inFile, line); // Skip DNA sequence line
            getline(inFile, line); // Skip separator line
        } else {
            lines.push_back(line);
        }
    }
    inFile.close();

    if (!found) {
        cout << "No data found for " << name << ".\n";
        return;
    }

    // Rewrite file with updated content
    ofstream outFile("DNA_bank.txt");
    if (!outFile.is_open()) {
        cout << "Error: Unable to open DNA_bank.txt\n";
        return;
    }
    for (const auto& line : lines) {
        outFile << line << "\n";
    }
    outFile.close();

    cout << "DNA information for " << name << " has been deleted from DNA_bank.txt.\n";
}

void setPassword() {
    string password;
    cout << "Set a new password for DNA information retrieval: ";
    cin >> password;

    ofstream passFile("password.txt");
    if (passFile.is_open()) {
        passFile << password;
        passFile.close();
        cout << "Password set successfully.\n";
    } else {
        cout << "Error: Unable to save the password.\n";
    }
}

// Function to verify password for information retrieval
bool verifyPassword() {
    ifstream passFile("password.txt");
    if (!passFile.is_open()) {
        cout << "No password set. Please set a password first.\n";
        return false;
    }

    string savedPassword, enteredPassword;
    getline(passFile, savedPassword);
    passFile.close();

    cout << "Enter password to access DNA information retrieval: ";
    cin >> enteredPassword;

    if (enteredPassword == savedPassword) {
        return true;
    } else {
        cout << "Incorrect password. Access denied.\n";
        return false;
    }
}

#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <string>

using namespace std;

void askOrganDonation() {
    string name, phoneNumber, dob, gender;
    char donateChoice;

    // Ask if they want to donate organs
    cout << "Do you want to donate organs in the future? (Y/N): ";
    cin >> donateChoice;

    // Convert the choice to uppercase for easier comparison
    donateChoice = toupper(donateChoice);

    if (donateChoice == 'Y') {

        cout << "Please enter your personal details:\n";
    cout << "Name: ";
    cin.ignore();  // To clear the input buffer
    getline(cin, name);
    cout << "Phone Number: ";
    getline(cin, phoneNumber);
    cout << "Date of Birth (DD/MM/YYYY): ";
    getline(cin, dob);
    cout << "Gender (Male/Female/Other): ";
    getline(cin, gender);

        cout << "Please select the organs you want to donate (select the numbers corresponding to your choices):\n";
        cout << "1. Eyes\n";
        cout << "2. Kidney\n";
        cout << "3. Liver\n";
        cout << "4. Heart\n";
        cout << "5. Lungs\n";
        cout << "6. Pancreas\n";
        cout << "7. Skin\n";
        cout << "8. Bone Marrow\n";
        cout << "9. Small Intestine\n";
        cout << "10. Blood\n";

        vector<string> selectedOrgans;
        int choice;
        while (true) {
            cout << "Enter a number (1-10) to select an organ to donate or 0 to finish selecting: ";
            cin >> choice;

            if (choice == 0) break; // Stop selection when 0 is entered

            if (choice < 1 || choice > 10) {
                cout << "Invalid choice. Please select a number between 1 and 10.\n";
            } else {
                switch (choice) {
                    case 1: selectedOrgans.push_back("Eyes"); break;
                    case 2: selectedOrgans.push_back("Kidney"); break;
                    case 3: selectedOrgans.push_back("Liver"); break;
                    case 4: selectedOrgans.push_back("Heart"); break;
                    case 5: selectedOrgans.push_back("Lungs"); break;
                    case 6: selectedOrgans.push_back("Pancreas"); break;
                    case 7: selectedOrgans.push_back("Skin"); break;
                    case 8: selectedOrgans.push_back("Bone Marrow"); break;
                    case 9: selectedOrgans.push_back("Small Intestine"); break;
                    case 10: selectedOrgans.push_back("Blood"); break;
                }
            }
        }

        // Save the collected information to the organ_donation.txt file
        ofstream outFile("organ_donation.txt", ios::app);
        if (outFile.is_open()) {
            outFile << "===========================\n";
            outFile << "Name           : " << name << "\n";
            outFile << "Phone Number   : " << phoneNumber << "\n";
            outFile << "Date of Birth  : " << dob << "\n";
            outFile << "Gender         : " << gender << "\n";
            outFile << "Organs Donated : ";
            if (selectedOrgans.empty()) {
                outFile << "None\n";
            } else {
                for (const auto& organ : selectedOrgans) {
                    outFile << organ << ", ";
                }
                outFile << "\n";
            }
            outFile << "===========================\n";
            outFile.close();
            cout << "Your organ donation preferences and personal details have been saved.\n";
        } else {
            cout << "Error: Unable to save organ donation preferences.\n";
        }
    } else {
        cout << "You chose not to donate organs in the future.\n";
    }
}


int main() {
   int mainChoice;
    cout << "Choose an option:\n";
    cout << "1. DNA Test\n";
    cout << "2. DNA Information Retrieval\n";
    cout << "3. Add New DNA Information\n";
    cout << "4. Delete DNA Information\n";
    cout << "5. Set or Update Password\n";
    
    
    cout << "Enter choice (1, 2, 3, 4, 5 ): ";
    cin >> mainChoice;
    
     if (mainChoice == 1) {
    string text1, text2, gen1, gen2, name1, name2;
    cout << "Note: To get the report more accurately, make sure the length of the gene code is the same for both people.\n";

    // Get info for person 1
    getPersonInfo(name1, text1, gen1, 1);

    // Get info for person 2
    getPersonInfo(name2, text2, gen2, 2);

    // Build suffix trees and find LCS
    SuffixTree suffixTree1(text1, 1);
    SuffixTree suffixTree2(text2, 2);

    string lcs = getLongestCommonSubstring(suffixTree1, suffixTree2);
    float ratio = 0;
    float matchPercentage = 0;

    // Calculate match percentage
    if (text1.length() == text2.length()) {
        ratio = (float)lcs.length() / text1.length();
        matchPercentage = ratio * 100;
    }

    // Display Report
    cout << "\n\n========= DNA MATCH REPORT =========\n";
    cout << "------------------------------------\n";
    cout << "   Person 1 Information:\n";
    cout << "   Name      : " << name1 << "\n";
    cout << "   Gender    : " << gen1 << "\n";
    cout << "   DNA Code  : " << text1 << "\n";
    cout << "------------------------------------\n";
    cout << "   Person 2 Information:\n";
    cout << "   Name      : " << name2 << "\n";
    cout << "   Gender    : " << gen2 << "\n";
    cout << "   DNA Code  : " << text2 << "\n";
    cout << "------------------------------------\n";
    
    cout << "   Comparison Results:\n";
    if (lcs.empty()) {
        cout << "   - No common substring found.\n";
    } else {
        cout << "   Longest Common Substring : " << lcs << "\n";
        cout << "   Length of LCS            : " << lcs.length() << " characters\n";
    }

    if (text1.length() == text2.length()) {
        cout << "   Match Percentage         : " << fixed << setprecision(2) << matchPercentage << "%\n";

        // Display relationship based on percentage and gender
        cout << "   Relationship Prediction  : ";
        if (matchPercentage == 100.0) {
            if (gen1 == gen2) {
                cout << "Identical twins.\n";
            } else {
                cout << "Identical twins (gender difference could be due to a recording error).\n";
            }
        } else if (matchPercentage == 50.0) {
            if (gen1 == "male" && gen2 == "male") {
                cout << "They may be father and son.\n";
            } else if (gen1 == "female" && gen2 == "female") {
                cout << "They may be mother and daughter.\n";
            } else {
                cout << "They may be parent and child.\n";
            }
        } else if (matchPercentage == 25.0) {
            cout << "They may be half-siblings or grandparent and grandchild.\n";
        } else if (matchPercentage == 12.5) {
            cout << "They may be first cousins.\n";
        } else {
            cout << "No close relation detected for this match percentage.\n";
        }
    } else {
        cout << "   Length mismatch warning  : The length of the DNA gene codes differs.\n";
        cout << "   Percentage matched for Person 1 with Person 2: " << fixed << setprecision(2) << ((float)lcs.length() / text1.length()) * 100 << "%\n";
        cout << "   Percentage matched for Person 2 with Person 1: " << fixed << setprecision(2) << ((float)lcs.length() / text2.length()) * 100 << "%\n";
        cout << "   * To get a more accurate relationship, provide equal length DNA codes for both people.\n";
    }
    cout << "====================================\n\n";
     }
      else if (mainChoice == 2) {
        if (verifyPassword()) {
            retrieveDNAInfo();
        }
    } 
      else if (mainChoice ==3){
        addNewDNA();
      }
      else if(mainChoice == 4)
      {
        string name;
        cout << "Enter the name of the person to delete from DNA bank: ";
        cin >> name;
        deleteFromDNABank(name);
      }
      else if (mainChoice == 5) {
        setPassword();
    } 
     else {
        cout << "Invalid choice.\n";
    }

    askOrganDonation();
    return 0;
}
