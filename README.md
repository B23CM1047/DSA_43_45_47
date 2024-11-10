#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
using namespace std;

struct SuffixTreeNode
{
    SuffixTreeNode *children[256];
    int startIndex;
    int stringNumber;

    SuffixTreeNode(int index, int strNum) : startIndex(index), stringNumber(strNum)
    {
        for (int i = 0; i < 256; i++)
        {
            children[i] = nullptr;
        }
    }
};

class SuffixTree
{
private:
    SuffixTreeNode *root;
    string text;
    int stringNumber;

    void insertSuffix(int suffixStart)
    {
        SuffixTreeNode *currentNode = root;
        for (int i = suffixStart; i < text.length(); i++)
        {
            char currentChar = text[i];
            if (currentNode->children[(unsigned char)currentChar] == nullptr)
            {
                currentNode->children[(unsigned char)currentChar] = new SuffixTreeNode(suffixStart, stringNumber);
            }
            currentNode = currentNode->children[(unsigned char)currentChar];
        }
    }

    void buildSuffixTree()
    {
        for (int i = 0; i < text.length(); i++)
        {
            insertSuffix(i);
        }
    }

public:
    SuffixTree(const string &s, int strNum) : text(s), stringNumber(strNum)
    {
        root = new SuffixTreeNode(-1, strNum);
        buildSuffixTree();
    }

    SuffixTreeNode *getRoot()
    {
        return root;
    }

    string getText() const
    {
        return text;
    }
};

// Recursive helper function to find the longest common substring
int findLCS(SuffixTreeNode *node1, SuffixTreeNode *node2, int depth, int currentStart, int &maxLength, int &startIdx)
{
    if (!node1 || !node2)
        return depth;
    int currentDepth = depth;
    for (int i = 0; i < 256; i++)
    {
        if (node1->children[i] && node2->children[i])
        {
            int newDepth = findLCS(node1->children[i], node2->children[i], depth + 1, node1->children[i]->startIndex, maxLength, startIdx);
            if (newDepth > maxLength)
            {
                maxLength = newDepth;
                startIdx = currentStart;
            }
            currentDepth = max(currentDepth, newDepth);
        }
    }
    return currentDepth;
}

// Function to compare two suffix trees and find the longest common substring
string getLongestCommonSubstring(SuffixTree &tree1, SuffixTree &tree2)
{
    int maxLength = 0;
    int startIdx = -1;
    findLCS(tree1.getRoot(), tree2.getRoot(), 0, 0, maxLength, startIdx);
    return (startIdx == -1 || maxLength == 0) ? "" : tree1.getText().substr(startIdx, maxLength);
}

// Save each person's DNA, gender, and name in DNA bank
void saveToDNABank(const string &name, const string &dna, const string &gender, const string &password)
{
    ofstream outFile("DNA_bank.txt", ios::app);
    if (outFile.is_open())
    {
        outFile << "Name: " << name << "\n";
        outFile << "Gender: " << gender << "\n";
        outFile << "DNA Sequence: " << dna << "\n";
        outFile << "password: " << password << "\n";
        outFile << "--------------------------\n";
        outFile.close();
        cout << "Information saved to DNA_bank.txt\n";
    }
    else
    {
        cout << "Error: Unable to open file.\n";
    }
}

// Load DNA information from file by name
bool loadFromDNABank(const string &name, const string &password, string &dna, string &gender)
{
    ifstream inFile("DNA_bank.txt");
    if (!inFile.is_open())
    {
        cout << "Error: Unable to open DNA_bank.txt\n";
        return false;
    }

    string line, storedPassword;
    bool found = false;
    while (getline(inFile, line))
    {
        if (line.find("Name: " + name) != string::npos)
        {
            found = true;
            getline(inFile, line);   // Read gender line
            gender = line.substr(8); // Extract gender
            getline(inFile, line);   // Read DNA sequence line
            dna = line.substr(13);   // Extract DNA sequence
            getline(inFile, line);
            storedPassword = line.substr(10);

            if (password == storedPassword)
            {
                inFile.close();
                return true;
            }
            else
            {
                cout << "Incorrect password. Access denied.\n";
                inFile.close();
                return false;
            }
        }
    }

    inFile.close();
    if (!found)
        cout << "No data found for " << name << ".\n";
    return false;
}

// Check if a name already exists in DNA_bank.txt
bool isNameExisting(const string &name)
{
    ifstream inFile("DNA_bank.txt");
    if (!inFile.is_open())
    {
        cout << "Error: Unable to open DNA_bank.txt\n";
        return false;
    }

    string line;
    while (getline(inFile, line))
    {
        if (line.find("Name: " + name) != string::npos)
        {
            inFile.close();
            return true;
        }
    }
    inFile.close();
    return false;
}

// Input function with choice to load from file or enter new information

// Input function with choice to load from file or enter new information
void getPersonInfo(string &name, const string &password, string &dna, string &gender, int personNumber)
{
    int choice;
    cout << "For person " << personNumber << ", choose option:\n";
    cout << "1. Use stored DNA data\n";
    cout << "2. Enter new DNA data\n";
    cout << "Enter choice (1 or 2): ";
    cin >> choice;

    if (choice == 1)
    {
        cout << "Enter name of person " << personNumber << ": ";
        cin >> name;
        if (!loadFromDNABank(name, password, dna, gender))
        {
            cout << "No data found for " << name << ". Please enter details manually.\n";
            choice = 2;
        }
    }

    if (choice == 2)
    {
        string password;

        do
        {
            cout << "Enter a unique name for person " << personNumber << ": ";
            cin >> name;
            if (isNameExisting(name))
            {
                cout << "The name \"" << name << "\" already exists. Please choose another name.\n";
            }
        } while (isNameExisting(name));

        cout << "Enter DNA gene code of " << name << ": ";
        cin >> dna;
        for (char &c : dna)
        {
            if (islower(c))
            {
                c = toupper(c);
            }
        }
        cout << "Enter gender of " << name << ": ";
        cin >> gender;
        cout << "Set a unique password for this entry\n";
        cin >> password;
        saveToDNABank(name, dna, gender, password);
    }
}

void retrieveDNAInfo()
{
    string name, dna, gender, password;
    cout << "Enter the name to search in DNA bank: ";
    cin >> name;
    if (loadFromDNABank(name, password, dna, gender))
    {
        cout << "\n=== DNA Information ===\n";
        cout << "Name: " << name << "\n";
        cout << "Gender: " << gender << "\n";
        cout << "DNA Sequence: " << dna << "\n";
    }
    else
    {
        cout << "No data found for " << name << ".\n";
    }
}

void addNewDNA()
{
    string name, dna, gender, password;
    do
    {
        cout << "Enter a unique name: ";
        cin >> name;
        if (isNameExisting(name))
        {
            cout << "The name \"" << name << "\" already exists. Please choose another name.\n";
        }
    } while (isNameExisting(name));

    cout << "Enter DNA gene code: ";
    cin >> dna;
    for (char &c : dna)
    {
        if (islower(c))
            c = toupper(c);
    }
    cout << "Enter gender: ";
    cin >> gender;
    cout << "enter password";
    cin >> password;
    saveToDNABank(name, dna, gender, password);
    cout << "DNA Data saved successfully";
}

void deleteFromDNABank(const string &name, const string &password)
{
    // Load DNA and gender for verification using loadFromDNABank
    string dna, gender;
    if (!loadFromDNABank(name, password, dna, gender))
    {
        cout << "No data found for " << name << " or incorrect password.\n";
        return;
    }

    // After verifying with loadFromDNABank, proceed to delete the entry
    ifstream inFile("DNA_bank.txt");
    if (!inFile.is_open())
    {
        cout << "Error: Unable to open DNA_bank.txt\n";
        return;
    }

    vector<string> lines;
    string line;
    bool found = false;

    // Read through the file and store lines except the ones for the target record
    while (getline(inFile, line))
    {
        if (line.find("Name: " + name) != string::npos)
        {
            found = true;
            // Skip lines for the entire record of the found name
            getline(inFile, line); // Skip gender line
            getline(inFile, line); // Skip DNA sequence line
            getline(inFile, line); // Skip password line
            getline(inFile, line); // Skip separator line
        }
        else
        {
            lines.push_back(line);
        }
    }
    inFile.close();

    if (!found)
    {
        cout << "No data found for " << name << ".\n";
        return;
    }

    // Rewrite file without the deleted record
    ofstream outFile("DNA_bank.txt");
    if (!outFile.is_open())
    {
        cout << "Error: Unable to open DNA_bank.txt\n";
        return;
    }
    for (const auto &line : lines)
    {
        outFile << line << "\n";
    }
    outFile.close();

    cout << "DNA information for " << name << " has been deleted from DNA_bank.txt.\n";
}
void setPassword()
{
    string password;
    cout << "Set a new password for DNA information retrieval: ";
    cin >> password;

    ofstream passFile("password.txt");
    if (passFile.is_open())
    {
        passFile << password;
        passFile.close();
        cout << "Password set successfully.\n";
    }
    else
    {
        cout << "Error: Unable to save the password.\n";
    }
}

// Function to verify password for information retrieval
bool verifyPassword()
{
    ifstream passFile("password.txt");
    if (!passFile.is_open())
    {
        cout << "No password set. Please set a password first.\n";
        return false;
    }

    string savedPassword, enteredPassword;
    getline(passFile, savedPassword);
    passFile.close();

    cout << "Enter password to access DNA information retrieval: ";
    cin >> enteredPassword;

    if (enteredPassword == savedPassword)
    {
        return true;
    }
    else
    {
        cout << "Incorrect password. Access denied.\n";
        return false;
    }
}

void registerDonation(
    const string &name,
    const string &phoneNumber,
    const string &dob,
    const string &gender,
    const string& organsList
)
{
    stringstream oss(organsList);
    string organ;
    vector<int> organs;
    while(oss >> organ)
        organs.push_back(stoi(organ));

    // Vector to store organ names based on the choices
    vector<string> selectedOrgans;
    for (int choice : organs)
    {
        switch (choice)
        {
        case 1:
            selectedOrgans.push_back("Eyes");
            break;
        case 2:
            selectedOrgans.push_back("Kidney");
            break;
        case 3:
            selectedOrgans.push_back("Liver");
            break;
        case 4:
            selectedOrgans.push_back("Heart");
            break;
        case 5:
            selectedOrgans.push_back("Lungs");
            break;
        case 6:
            selectedOrgans.push_back("Pancreas");
            break;
        case 7:
            selectedOrgans.push_back("Bone Marrow");
            break;
        case 8:
            selectedOrgans.push_back("Blood");
            break;
        default:
            cout << "Invalid organ choice: " << choice << endl;
        }
    }

    // Save the collected information to the organ_donation.txt file
    ofstream outFile("organ_donation.txt", ios::app);
    if (outFile.is_open())
    {
        outFile << "===========================\n";
        outFile << "Name           : " << name << "\n";
        outFile << "Phone Number   : " << phoneNumber << "\n";
        outFile << "Date of Birth  : " << dob << "\n";
        outFile << "Gender         : " << gender << "\n";
        outFile << "Organs Donated : ";
        if (selectedOrgans.empty())
        {
            outFile << "None\n";
        }
        else
        {
            for (const auto &organ : selectedOrgans)
            {
                outFile << organ << ", ";
            }
            outFile << "\n";
        }
        outFile << "===========================\n";
        outFile.close();
        cout << "Your organ donation preferences and personal details have been saved.\n";
    }
    else
    {
        cout << "Error: Unable to save organ donation preferences.\n";
    }
}

int main(int argc, char *argv[])
{
    // Check if correct number of arguments are passed
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <option> [arguments]\n";
        cout << "Options:\n";
        cout << "1: DNA Test\n";
        cout << "2: DNA Information Retrieval\n";
        cout << "3: Add New DNA Information\n";
        cout << "4: Delete DNA Information\n";
        cout << "5: Organs Donation\n";
    }

    // Parse the option from command line arguments
    int mainChoice = atoi(argv[1]);

    // Process based on the option
    if (mainChoice == 1)
    {
        if (argc < 10)
        {
            cout << "Usage: " << argv[0] << " 1 <name1> <name2>\n";
            return 1;
        }

        string name1 = argv[2];
        string name2 = argv[3];

        // Load DNA data
        string text1 = argv[4], text2 = argv[5], gen1 = argv[6], gen2 = argv[7], password1 = argv[8], password2 = argv[9];

        // if (!loadFromDNABank(name1, text1, gen1) || !loadFromDNABank(name2, text2, gen2)) {
        //     cout << "Error: One or both names not found in DNA bank.\n";
        //     return 1;
        // }

        // Build suffix trees and find LCS
        SuffixTree suffixTree1(text1, 1);
        SuffixTree suffixTree2(text2, 2);

        string lcs = getLongestCommonSubstring(suffixTree1, suffixTree2);
        float matchPercentage = 0;

        if (text1.length() == text2.length())
        {
            matchPercentage = (lcs.length() / text1.length()) * 100;
        }

        // Output the DNA match report
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
        if (lcs.empty())
        {
            cout << "   - No common substring found.\n";
        }
        else
        {
            cout << "   Longest Common Substring : " << lcs << "\n";
            cout << "   Length of LCS            : " << lcs.length() << " characters\n";
        }

        cout << "   Match Percentage         : " << fixed << setprecision(2) << matchPercentage << "%\n";
        ofstream outFile("dna_match_report.txt");

        if (outFile.is_open())
        {
            outFile << "========= DNA MATCH REPORT =========";
            outFile << "------------------------------------\n";
            outFile << "   Person 1 Information:\n";
            outFile << "   Name      : " << name1 << "\n";
            outFile << "   Gender    : " << gen1 << "\n";
            outFile << "   DNA Code  : " << text1 << "\n";
            outFile << "------------------------------------\n";
            outFile << "   Person 2 Information:\n";
            outFile << "   Name      : " << name2 << "\n";
            outFile << "   Gender    : " << gen2 << "\n";
            outFile << "   DNA Code  : " << text2 << "\n";
            outFile << "------------------------------------\n";

            outFile << "   Comparison Results:\n";
            if (lcs.empty())
            {
                outFile << "   - No common substring found.\n";
            }
            else
            {
                outFile << "   Longest Common Substring : " << lcs << "\n";
                outFile << "   Length of LCS            : " << lcs.length() << " characters\n";
            }

            outFile << "   Match Percentage         : " << fixed << setprecision(2) << matchPercentage << "%\n";

            outFile.close(); // Close the file after writing
        }
        else
        {
            cout << "Unable to open file for writing.\n";
        }
    }
    else if (mainChoice == 2)
    {
        if (argc < 3)
        {
            cout << "Usage: " << argv[0] << " 2 <name>\n";
            return 1;
        }

        string name = argv[2];
        string password = argv[3];
        string dna, gender;

        if (loadFromDNABank(name, password, dna, gender))
        {
            cout << "\n=== DNA Information ===\n";
            cout << "Name: " << name << "\n";
            cout << "Gender: " << gender << "\n";
            cout << "DNA Sequence: " << dna << "\n";
        }
        else
        {
            cout << "No data found for " << name << ".\n";
        }
    }
    else if (mainChoice == 3)
    {
        if (argc < 4)
        {
            cout << "Usage: " << argv[0] << " 3 <name> <dna> <gender>\n";
            return 1;
        }

        string name = argv[2];
        string dna = argv[3];
        string gender = argv[4];
        string password = argv[5];

        // Save to DNA bank
        saveToDNABank(name, dna, gender, password);
    }
    else if (mainChoice == 4)
    {
        if (argc != 4)
        {
            cerr << "Usage: ./DSA2 <name> <password>\n";
            return 1;
        }

        string name = argv[2];
        string password = argv[3];

        deleteFromDNABank(name, password);
    }
    else if (mainChoice == 5)
    {
        string name = argv[2];
        string phone = argv[3];
        string dob = argv[4];
        string gen = argv[5];
        string choice;
        for (int i = 6; i < argc; ++i) {
            choice += argv[i];
            if (i < argc - 1) {
                choice += " "; // Add a space between arguments
            }
        }

        registerDonation(name, phone, dob, gen, choice);
    }
    else
    {
        cout << "Invalid option. Use a valid option (1-5).\n";
        return 1;
    }

    return 0;
}
