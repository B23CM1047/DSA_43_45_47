// Send DNA comparison data to backend
// Send DNA comparison data to backend and also save the DNA to the bank
// Send DNA comparison data to backend and also save the DNA to the bank
async function handleDNAComparison(event) {
    event.preventDefault();

    const data = {
        name1: document.getElementById("name1").value,
        dna1: document.getElementById("dna1").value,
        gender1: document.getElementById("gender1").value,
        password1: document.getElementById("password1").value,
        name2: document.getElementById("name2").value,
        dna2: document.getElementById("dna2").value,
        gender2: document.getElementById("gender2").value,
        password2: document.getElementById("password2").value
    };

    try {
        // First, compare DNA (you may need to implement this in your backend as well)
        const comparisonResponse = await fetch("http://127.0.0.1:8000/compare", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(data)
        });

        // Handle the comparison result
        if (comparisonResponse.ok) {
            const comparisonResult = await comparisonResponse.text();
            document.getElementById("comparisonResult").innerHTML = comparisonResult;
        } else {
            document.getElementById("comparisonResult").innerHTML = "Error: Unable to compare DNA.";
        }

        // Save Person 1's DNA, including password
        const saveToBankResponse1 = await fetch("http://127.0.0.1:8000/add", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                name: data.name1,
                dna: data.dna1,
                gender: data.gender1,
                password: data.password1
            })
        });

        if (!saveToBankResponse1.ok) {
            document.getElementById("comparisonResult").innerHTML += "<br>Error: Unable to save Person 1's DNA.";
        }

        // Save Person 2's DNA, including password
        const saveToBankResponse2 = await fetch("http://127.0.0.1:8000/add", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                name: data.name2,
                dna: data.dna2,
                gender: data.gender2,
                password: data.password2
            })
        });

        if (!saveToBankResponse2.ok) {
            document.getElementById("comparisonResult").innerHTML += "<br>Error: Unable to save Person 2's DNA.";
        }

    } catch (error) {
        document.getElementById("comparisonResult").innerHTML = "Error: " + error.message;
    }
}

function showSection(sectionId) {
    const sections = document.querySelectorAll('.content-section');
    sections.forEach(section => {
        section.style.display = 'none';
    });
    document.getElementById(sectionId).style.display = 'block';
}

// Organ Donation Form Submission Handler
async function handleOrganDonation(event) {
    event.preventDefault();

    const donorName = document.getElementById("donorName").value;
    const phoneNumber = document.getElementById("phoneNumber").value;
    const dob = document.getElementById("dob").value;
    const gender = document.getElementById("gender").value;
    
    const selectedOrgans = Array.from(document.querySelectorAll("input[name='organ']:checked")).map(checkbox => checkbox.value);

    // Here, you would send data to the backend
    console.log({
        donorName,
        phoneNumber,
        dob,
        gender,
        selectedOrgans
    });

    const data = {
        name: donorName,
        phone_number: phoneNumber,
        dob,
        gender,
        organs: selectedOrgans
    }

    const response = await fetch("http://127.0.0.1:8000/register_donation",  {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(data)
    })
    if(response.status === 200) {
        alert("Organ donation preferences submitted successfully.");
    } else {
        alert("Organ donation preferences submitted unsuccessfully.");
    }
}

// Retrieve DNA information by name and password from backend
async function handleInfoRetrieval(event) {
    event.preventDefault();
    const data = {
        name: document.getElementById("retrieveName").value,
        password: document.getElementById("retrieveNamepassword").value  // Added password field
    };

    try {
        const response = await fetch("http://127.0.0.1:8000/retrieve", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(data)
        });

        const result = await response.text();
        document.getElementById("retrievalResult").innerText = result;
    } catch (error) {
        document.getElementById("retrievalResult").innerText = "Error: " + error.message;
    }
}


// Add new DNA information to backend
async function handleAddToBank(event) {
    event.preventDefault();
    const data = {
        name: document.getElementById("newName").value,
        dna: document.getElementById("newDNA").value,
        gender: document.getElementById("newGender").value,
        password: document.getElementById("newPassword").value  // Added password field
    };

    try {
        const response = await fetch("http://127.0.0.1:8000/add", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(data)
        });

        const result = await response.text();
        document.getElementById("addResult").innerText = result;
    } catch (error) {
        document.getElementById("addResult").innerText = "Error: " + error.message;
    }
}

// Delete DNA information from backend
async function handleDeleteDNA(event) {
    event.preventDefault();
    const data = {
        name: document.getElementById("deleteName").value,
        password: document.getElementById("deleteNamepassword").value // Added password field
    };

    try {
        const response = await fetch("http://127.0.0.1:8000/delete", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(data)
        });

        if (response.ok) {
            const result = await response.text();
            document.getElementById("deleteResult").innerText = result;
        } else {
            document.getElementById("deleteResult").innerText = "Error: Unable to delete DNA information.";
        }
    } catch (error) {
        document.getElementById("deleteResult").innerText = "Error: " + error.message;
    }
}

// Helper function to handle C++ execution
async function handleCppExecution(data) {
    try {
        const response = await fetch("http://127.0.0.1:8000/run_cpp", {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(data)
        });

        const result = await response.text();
        document.getElementById("cppExecutionResult").innerText = result;
    } catch (error) {
        document.getElementById("cppExecutionResult").innerText = "Error: " + error.message;
    }
}

// Show the selected section and hide others
function showSection(sectionId) {
    // Hide all sections
    const sections = document.querySelectorAll('.content-section');
    sections.forEach(section => section.style.display = 'none');

    // Show the selected section
    const selectedSection = document.getElementById(sectionId);
    if (selectedSection) {
        selectedSection.style.display = 'block';
    }
}
