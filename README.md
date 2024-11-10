This project is a DNA gene comparison application designed with a FastAPI-based Python API linking a frontend with a backend written in C++. It offers DNA comparison and data management functionalities with a DNA Bank and an Organ Donation feature.


Overview :
The DNA Gene Comparison System allows users to:
1.Compare DNA sequences for genetic similarity
2.Manage and retrieve DNA records from a DNA Bank
3.Register as an organ donor

The comparison process utilizes suffix trees to efficiently find the longest common substring (LCS) between two DNA sequences, providing a high-accuracy matching rate. Execution times for suffix tree and traditional string comparisons are displayed, highlighting efficiency gains with suffix trees, particularly for large DNA inputs.

Setup and Installation : 
To run the system, ensure that FastAPI is installed in your Python environment, along with necessary dependencies.

1.Install FastAPI and Dependencies:

pip install fastapi

2.Start the FastAPI server:

uvicorn main:app --reload

3.Compile the C++ backend: Ensure the backend C++ code is compiled and ready to interact with the FastAPI application.

Usage
Upon running the application, users are presented with the following options:

1.DNA Comparison
2.Information Retrieval
3.Add DNA to the Bank
4.Delete DNA from the Bank
5.Organ Donation Registration
Users can interact with the system via a frontend interface built with HTML, CSS, and JavaScript. Data is managed with text files and file handling.

Options
1. DNA Comparison
Users can select DNA sequences from the DNA Bank or input new DNA sequences.
DNA records are managed in a text file called DNA_Bank.txt.
If using stored data, the user provides a name, and details are fetched from the DNA Bank.
Comparison is done using suffix trees to find the Longest Common Substring (LCS).
The system calculates and displays percentage similarity, LCS, match percentage, and relationship prediction based on predefined constraints.

2. Information Retrieval
User provides name and password for secure access.
If the password matches the stored password, the user’s details are displayed; otherwise, access is denied.

3. Add to DNA Bank
Users can add new DNA records with details like name, gender, DNA sequence, and a password.
This information is stored in DNA_Bank.txt.

4. Delete DNA from the Bank
Users provide a name and password for secure deletion of DNA records from the DNA Bank.

5. Organ Donation Registration
Users can select organs to donate and provide their phone number.
Organ donation details are stored in a separate file called organ_donation.txt.


Implementation Details
Backend: Written in C++, handling core operations.
API: A Python layer using FastAPI serves as a bridge between the C++ backend and the frontend.
Frontend: HTML, CSS, and JavaScript provide an interactive UI for users.
File Handling: Text files (DNA_Bank.txt, organ_donation.txt) manage data storage and retrieval.
Suffix Tree Comparison: Efficiently finds LCS for DNA sequences, enhancing comparison accuracy over traditional string matching.


File Structure
main.py: FastAPI server code linking frontend and backend.
DNA_Bank.txt: Stores DNA records securely.
organ_donation.txt: Stores organ donation information.
frontend/: Contains HTML, CSS, and JavaScript files for the UI.
backend_cpp/: C++ codebase for DNA operations.
