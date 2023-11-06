/*
Name: Gabe DiMartino
Class: CPSC122, Fall 2023
Date: November 5, 2023
Programming Assignment: PA5
Description: This cpp contains all of the definitions of functions
*/

#include "header.h"

     /*************************************************************
    * Function: clearScreen ()
    * Date Created: 9/2/23
    * Date Last Modified: 9/2/23
    * Description: Uses an if statement to determine what OS is uesed
    * then runs the system command to clear the terminal
    * Input parameters: System OS
    * Returns: Clear Screen
    * Pre: Correct library
    * Post: Clear Screen
    *************************************************************/

void clearScreen(){ //function to clear terminal
#ifdef _WIN32 //If OS is WIN32 run cls else use clear
    system("cls");
#else
    system("clear");
#endif
}

    /*************************************************************
    * Function: fileOpen ()
    * Date Created: 9/2/23
    * Date Last Modified: 11/5/23
    * Description: Opens any file passed to it. 
    * Input parameters: file name and the fStream variable
    * Returns: T/F if sucessful throws error if F
    * Pre: Read/Write access in directory
    * Post: Working file
    *************************************************************/

bool fileOpen(std::string filename, std::ifstream& inFile) {
    inFile.open(filename); //open file
    return inFile.is_open();
}

    /*************************************************************
    * Function: fileWrite ()
    * Date Created: 9/17/23
    * Date Last Modified: 9/17/23
    * Description: Opens any file passed to it. If the file doesnt exist
    * the program creates it
    * Input parameters: file name and the fStream variable
    * Returns: T/F if sucessful throws error if F
    * Pre: Read/Write access in directory
    * Post: Working file
    *************************************************************/

bool fileWrite(const std::string filename, std::ofstream& outFile){
    outFile.open(filename, std::ofstream::out | std::ofstream::trunc ); //open a file to write to

    if (!outFile.is_open()) {
        outFile.clear(); //if it does not exist

        outFile.open(filename, std::ofstream::out); //create file

        if (!outFile.is_open()) { 
            return false; //after a second attempt, error the file
        }

        outFile.close(); //close file
        outFile.open(filename, std::ios::in | std::ios::out); //open again
    }

    return outFile.is_open();

}

   /*************************************************************
    * Function: getDataFromCSV ()
    * Date Created: 9/17/23
    * Date Last Modified: 11/6/23
    * Description: Converts the CSV data into a linked list
    * Input parameters: file, headers vector, list
    * Returns: void
    * Pre: Existing CSV file is opened
    * Post: populated list
    *************************************************************/

void getDataFromCSV(std::ifstream& inFile, PetStoreList& storeList) {
    std::vector<std::string> headers;
    std::string dataLine;

    getHeaderLine(inFile, headers); // Get the header line and store it in headers vector

    std::cout << "Processed " << headers.size() << " header columns: "; //Format the headers with commas
        for (size_t i = 0; i < headers.size(); i++) {
            std::cout << headers[i];
            if (i < headers.size() - 1) {
                std::cout << ", ";
            }
        }

    std::cout << std::endl << std::endl;

    while (std::getline(inFile, dataLine)) {
        std::istringstream dataToParse(dataLine);
        std::string storeName, pName, pType;
        int numDays;
        
        if (dataLine.empty()) {
            break; // Exit the loop if an empty line is encountered
        }

        std::getline(dataToParse, storeName, ',');
        std::getline(dataToParse, pName, ',');
        std::getline(dataToParse, pType, ',');
        dataToParse >> numDays;

        std::cout << "Processed a " << pType << ", \"" << pName << "\" ... "
        << numDays << " day(s) on site at store \"" << storeName << "\"" << std::endl;

        storeList.addPetData(storeName,pName,pType,numDays);
    }
}

   /*************************************************************
    * Function: getHeaderLine ()
    * Date Created: 9/17/23
    * Date Last Modified: 9/18/23
    * Description: Converts the CSV header line into a vector
    * Input parameters: opened file and header vector
    * Returns: vectors containing header row
    * Pre: Existing CSV file is opened
    * Post: Vectors containing CSV data
    *************************************************************/

void getHeaderLine(std::ifstream& inFile, std::vector<std::string>& headers){
    std::string headerLine, cell;

    std::getline(inFile, headerLine);
    std::istringstream lineToParse(headerLine);
    while (std::getline(lineToParse, cell, ',')) { //For the text seperated by a comma add the element to the vector
        headers.push_back(cell); //For the first line only
    }

}

    /*************************************************************
    * Function: processData ()
    * Date Created: 9/2/23
    * Date Last Modified: 9/17/23
    * Description: This function process the vectors and outputs
    * the result to the terminal
    * Input parameters: header row and column vectors
    * Returns: pointer int value for dynamic array
    * Pre: Data from CSV must be input into vectors
    * Post: Processed vectors in the terminal.
    *************************************************************/

bool processData(const std::string filename, std::ifstream& inFile, PetStoreList& storeList){

    if (fileOpen(filename, inFile)) { //If the file can open run the following
        clearScreen(); //Clear screen
        getDataFromCSV(inFile, storeList); //Get the data from CSV to the indivudal vectors
        std::cout << "All pet store data processed!" <<std::endl << std::endl;
        std::cout<< "Generating summary report..." <<std::endl<<std::endl;
        std::cout<< "Done!" <<std::endl<<std::endl;
        inFile.close(); //Close the file
        return true;

    } else {
        std::cerr << "The file " + filename + " has encountered an error. Please make sure the file is in the correct directory and you have the appropriate permissions to read." << std::endl;
        return false;
    }
}

PetStoreList::PetStoreList(){
    headPtr = nullptr;
};

PetStoreList::~PetStoreList(){
};

PetStoreList::PetStoreData* PetStoreList::createNode(std::string storeName){
    PetStoreData* newPetStoreNode = new PetStoreData;
    newPetStoreNode->petStoreName = storeName;
    newPetStoreNode->nextStore = nullptr;

    return newPetStoreNode;
}

void PetStoreList::insertAtEnd(PetStoreData* newStoredata){
    if (headPtr == nullptr) {
            headPtr = newStoredata;
    } else {
        PetStoreData* helperNodePtr = headPtr;
        while (helperNodePtr->nextStore != nullptr) {
            helperNodePtr = helperNodePtr->nextStore;
        }
        newStoredata->nextStore = nullptr;
        helperNodePtr->nextStore = newStoredata;
     }
}

void PetStoreList::addPetData(std::string storeName, std::string pName, std::string pType, int numDays) {
    PetStoreData* helperNodePtr = headPtr;
    PetData newPetData;
    PetStoreData* newStore;

    if (storeInList(storeName)) {
        
        while (helperNodePtr != nullptr) {
            if (helperNodePtr->petStoreName == storeName) {
                newPetData.petName = pName;
                newPetData.petType = pType;
                newPetData.numDaysAtStore = numDays;
                helperNodePtr->petData.push_back(newPetData);
                return;
            }
            helperNodePtr = helperNodePtr->nextStore;
        }
    } else {
        // If the store doesn't exist, create it and then add the pet data
        newStore = createNode(storeName);
        insertAtEnd(newStore);
        addPetData(storeName, pName, pType, numDays); // Recursively call with the newly created store
    }
}


void PetStoreList::displayPetList() const {
    PetStoreData* storePtr = headPtr;

    while (storePtr != nullptr) {
        std::cout<< "__________________________________" <<std::endl;
        std::cout << "Pet Store Name: " << storePtr->petStoreName << std::endl;
        std::cout << "**********************************" << std::endl;
        std::cout << "| Number of pets: " << storePtr->petData.size() << std::endl;
        std::cout << "| Pet Information: " << std::endl;

        for (size_t i = 0; i < storePtr->petData.size(); i++) {
            std::cout << "> Pet " << i + 1 << " Name: " << storePtr->petData[i].petName << std::endl;
            std::cout << "> Pet Type: " << storePtr->petData[i].petType << std::endl;
            std::cout << "> Number of days at store: " << storePtr->petData[i].numDaysAtStore << std::endl;
            std::cout<<"----------------------------"<<std::endl;
        }

        storePtr = storePtr->nextStore;
    }

}

bool PetStoreList::storeInList(std::string name) {
    PetStoreData* helperPtr = headPtr;
    
    while (helperPtr != nullptr) {
        if (helperPtr->petStoreName == name) {
            return true;
        }
        helperPtr = helperPtr->nextStore;
    }
    
    return false;
}

void PetStoreList::calculatePetSummary(){
    PetStoreData* helperPtr = headPtr;
    summaryData.numPets = 0;
    int tempVariable = 0;

    while (helperPtr != nullptr){
        summaryData.numPets += helperPtr->petData.size();
        helperPtr = helperPtr->nextStore;
    }

    helperPtr = headPtr;
    tempVariable = 0;

    while (helperPtr != nullptr) {
        for (size_t i = 0; i < helperPtr->petData.size(); i++) {
            tempVariable += helperPtr->petData[i].numDaysAtStore;
        }
        helperPtr = helperPtr->nextStore;
    }

    if (summaryData.numPets != 0) {
        summaryData.averageDaysAtStore = tempVariable / summaryData.numPets;
    } else {
        summaryData.averageDaysAtStore = 0;
    }

    helperPtr = headPtr;
    tempVariable = helperPtr->petData[0].numDaysAtStore;
    

    while (helperPtr != nullptr){
        for(size_t i = 0; i < helperPtr->petData.size(); i++){
            if (helperPtr->petData[i].numDaysAtStore < tempVariable)
            tempVariable = helperPtr->petData[i].numDaysAtStore;
        }
        helperPtr = helperPtr->nextStore;
    }

    summaryData.minDaysAtStore = tempVariable;

    helperPtr = headPtr;
    tempVariable = 0;

    while (helperPtr != nullptr){
        for(size_t i = 0; i < helperPtr->petData.size(); i++){
            if (helperPtr->petData[i].numDaysAtStore > tempVariable)
            tempVariable = helperPtr->petData[i].numDaysAtStore;
        }
        helperPtr = helperPtr->nextStore;
    }

    summaryData.maxDaysAtStore = tempVariable;

}

void PetStoreList::displayPetSummary() const{
    std::cout<< "__________________________________" <<std::endl;
    std::cout<< "Total number of pets: "<< summaryData.numPets <<std::endl;
    std::cout<< "Average number of days at pet store: "<< summaryData.averageDaysAtStore <<std::endl;
    std::cout<< "Shortest stay at pet store: "<< summaryData.minDaysAtStore <<std::endl;
    std::cout<< "Longest stay at pet store: "<< summaryData.maxDaysAtStore <<std::endl;
}

void PetStoreList::writePetSummary(std::ofstream& outfile){
    outfile<< "Pet Store CSV Summary Report" <<std::endl;
    outfile<< "__________________________________" <<std::endl;
    outfile<< "Total number of pets: "<< summaryData.numPets <<std::endl;
    outfile<< "Average number of days at pet store: "<< summaryData.averageDaysAtStore <<std::endl;
    outfile<< "Shortest stay at pet store: "<< summaryData.minDaysAtStore <<std::endl;
    outfile<< "Longest stay at pet store: "<< summaryData.maxDaysAtStore <<std::endl;
}

void PetStoreList::writePetList(std::ofstream& outfile){
    PetStoreData* storePtr = headPtr;

    while (storePtr != nullptr) {
        outfile << "__________________________________" <<std::endl;
        outfile << "Pet Store Name: " << storePtr->petStoreName << std::endl;
        outfile << "**********************************" << std::endl;
        outfile << "| Number of pets: " << storePtr->petData.size() << std::endl;
        outfile << "| Pet Information: " << std::endl;

        for (size_t i = 0; i < storePtr->petData.size(); i++) {
            outfile << "> Pet " << i + 1 << " Name: " << storePtr->petData[i].petName << std::endl;
            outfile << "> Pet Type: " << storePtr->petData[i].petType << std::endl;
            outfile << "> Number of days at store: " << storePtr->petData[i].numDaysAtStore << std::endl;
            outfile<<"----------------------------"<<std::endl;
        }

        storePtr = storePtr->nextStore;
    }

}