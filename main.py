from fastapi import FastAPI
from pydantic import BaseModel
from fastapi.middleware.cors import CORSMiddleware
import subprocess
import os

app = FastAPI()

app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Adjust this to match your frontend origin
    allow_credentials=True,
    allow_methods=["*"],  # Allow all HTTP methods (GET, POST, etc.)
    allow_headers=["*"],  # Allow all headers
)

# Pydantic models for the incoming data
class DeleteDNAData(BaseModel):
    name: str
    password: str

class DNAComparisonData(BaseModel):
    name1: str
    dna1: str
    gender1: str
    name2: str
    dna2: str
    gender2: str
    password1: str
    password2: str

class DNAInfoRetrievalData(BaseModel):
    name: str
    password: str

class AddDNAData(BaseModel):
    name: str
    dna: str
    gender: str
    password: str

class OrganDonationData(BaseModel):
    name: str
    phone_number: str
    dob: str  # Date of Birth in DD/MM/YYYY format
    gender: str  # Male, Female, or Other
    organs: list[str]  # List of organ numbers to donate (1-8)


# Path to the C++ executable (ensure it's correct for your environment)
DSA2_PATH = ".\DSA2.exe"


# Route to compare DNA (this will trigger C++ execution)
@app.post("/compare")
async def compare_dna(data: DNAComparisonData):
    command = f".\DSA2 1 {data.name1} {data.name2} {data.dna1} {data.dna2} {data.gender1} {data.gender2} {data.password1} {data.password2}"
    
    try:
        result = subprocess.run(command, shell=True, capture_output=True, text=True)
        if result.returncode == 0:
            report_file = 'dna_match_report.txt'
            if os.path.isfile(report_file):
                with open(report_file, 'r') as file:
                    report_content = file.read()
                return report_content
            else:
                return {"error": "DNA match report file not found."}
        else:
            return {"error": f"C++ execution failed: {result.stderr}"}

    except Exception as e:
        return {"error": f"Error during execution: {str(e)}"}

# Route to retrieve DNA info
@app.post("/retrieve")
async def retrieve_dna(data: DNAInfoRetrievalData):
    command = [".\DSA2", "2", data.name, data.password]
    result = subprocess.run(command, capture_output=True, text=True)
    
    if result.returncode == 0:
        return {"result": result.stdout}
    else:
        return {"result": f"Error: {result.stderr or 'No data found for ' + data.name}."}

# Route to add DNA info
@app.post("/add")
async def add_dna(data: AddDNAData):
    dna_upper = data.dna.upper()
    command = [".\DSA2", "3", data.name, dna_upper, data.gender, data.password]
    
    result = subprocess.run(command, capture_output=True, text=True)
    
    if result.returncode == 0:
        return {"result": result.stdout.strip()}
    else:
        return {"result": f"Error: {result.stderr or 'Failed to add DNA for ' + data.name}."}

# Route to delete DNA info
@app.post("/delete")
async def delete_dna(data: DeleteDNAData):
    # Prepare the command to run the C++ executable for deleting DNA information
    command = [".\DSA2", "4", data.name, data.password]  # Adjusted for the delete operation

    try:
        # Run the command and capture the output
        result = subprocess.run(command, capture_output=True, text=True)

        # Check if the command executed successfully
        if result.returncode == 0:
            return {"result": result.stdout.strip()}
        else:
            return {"result": f"Error: {result.stderr.strip() or 'Failed to delete DNA for ' + data.name}"}

    except Exception as e:
        return {"result": f"Error: {str(e)}"}

# Route to handle organ donation registration
@app.post("/register_donation")
async def register_donation(data: OrganDonationData):
  

    # Convert organ choices to strings for the C++ program
    organ_choices = " ".join(map(str, data.organs))

    # Create the command to run the C++ program with user data
    command = (
        f".\DSA2 5 {data.name} {data.phone_number} {data.dob} {data.gender} {organ_choices}"
    )

    try:
        # Run the C++ program
        result = subprocess.run(command, shell=True, capture_output=True, text=True)

        if result.returncode == 0:
            report_file = 'organ_donation.txt'
            if os.path.isfile(report_file):
                with open(report_file, 'r') as file:
                    report_content = file.read()
                return {"message": "Donation preferences saved successfully.", "report": report_content}
            else:
                return {"error": "Organ donation report file not found."}
        else:
            return {"error": f"C++ execution failed: {result.stderr}"}
    except Exception as e:
        return {"error": f"Error during execution: {str(e)}"}
    

# Route to execute C++ file with command line arguments (just for debugging or specific use cases)
@app.post("/run_cpp")
async def run_cpp(data: DNAComparisonData):
    if not os.path.isfile(DSA2_PATH):
        return {"error": "C++ executable not found."}

    command = f"{DSA2_PATH} {data.name1} {data.dna1} {data.gender1} {data.name2} {data.dna2} {data.gender2}"
    result = subprocess.run(command, shell=True, capture_output=True, text=True)
    
    if result.returncode == 0:
        return {"result": result.stdout}
    else:
        return {"error": f"C++ execution failed: {result.stderr}"}
