#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;

const string ClientsFileName = "Clients.txt";// File name for client data
const string Separator = "#//#";// Separator used for splitting client data

// Structure to hold client data
struct Client {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
};

// Encrypts a PIN code by shifting characters
string EncryptPinCode(string PinCode) {
    string Encrypted = "";
    for (char c : PinCode)
        Encrypted += char(c + 3);
    return Encrypted;
}

// Decrypts the encrypted PIN code by shifting characters back
string DecryptPinCode(string Encrypted) {
    string Original = "";
    for (char c : Encrypted)
        Original += char(c - 3);
    return Original;
}

// Splits a string into tokens based on the delimiter
vector<string> SplitString(const string& Line, const string& Delim) {
    vector<string> Tokens;
    size_t Start = 0, End;
    while ((End = Line.find(Delim, Start)) != string::npos) {
        Tokens.push_back(Line.substr(Start, End - Start));
        Start = End + Delim.length();
    }
    if (Start < Line.length())
        Tokens.push_back(Line.substr(Start));
    return Tokens;
}

// Converts a client record to a string line for file storage
string ConvertRecordToLine(Client C) {
    return C.AccountNumber + Separator +
        EncryptPinCode(C.PinCode) + Separator +
        C.Name + Separator +
        C.Phone + Separator +
        to_string(C.AccountBalance);
}

// Converts a line of text from the file back into a Client record
Client ConvertLineToRecord(string Line) {
    Client C;
    vector<string> Parts = SplitString(Line, Separator);
    if (Parts.size() >= 5) {
        C.AccountNumber = Parts[0];
        C.PinCode = DecryptPinCode(Parts[1]);
        C.Name = Parts[2];
        C.Phone = Parts[3];
        C.AccountBalance = stod(Parts[4]);// Convert string to double
    }
    return C;
}

// Loads client data from a file and returns a vector of Client objects.
vector<Client> LoadClientsDataFromFile(string FileName) {
    vector<Client> Clients;
    fstream File(FileName, ios::in);
    string Line;

    if (File.is_open()) {
        // Read each line from the file and convert it to a Client record
        while (getline(File, Line))
            Clients.push_back(ConvertLineToRecord(Line));

        File.close();
    }

    return Clients;
}

// Saves all client data back to the file
void SaveAllClientsToFile(string FileName, vector<Client> Clients) {
    fstream File(FileName, ios::out | ios::trunc);

    if (File.is_open()) {
        // Write each client record to the file
        for (Client C : Clients)
            File << ConvertRecordToLine(C) << endl;

        File.close();
    }
}

// Prints client data in a formatted table
void PrintClient(Client C) {
    cout << "| " << setw(16) << left << C.AccountNumber
        << "| " << setw(11) << left << C.PinCode
        << "| " << setw(29) << left << C.Name
        << "| " << setw(16) << left << C.Phone
        << "| " << setw(13) << fixed << setprecision(2) << C.AccountBalance << "|\n";
}

// Prints all clients in a formatted table
void PrintAllClients(vector<Client> Clients) {
    system("cls");
    cout << "\n\t\tClient List (" << Clients.size() << " clients)\n";
    cout << "+-----------------+------------+------------------------------+-----------------+--------------+\n";
    cout << "| Account Number  | Pin Code   | Name                         | Phone           | Balance      |\n";
    cout << "+-----------------+------------+------------------------------+-----------------+--------------+\n";

    for (Client C : Clients)
        PrintClient(C);

    cout << "+-----------------+------------+------------------------------+-----------------+--------------+\n";
    system("pause");
}

// Reads client data from user input
Client ReadClientData() {
    Client C;
    cout << "Enter Account Number: ";
    getline(cin >> ws, C.AccountNumber);
    cout << "Enter Pin Code: ";
    getline(cin, C.PinCode);
    cout << "Enter Name: ";
    getline(cin, C.Name);
    cout << "Enter Phone: ";
    getline(cin, C.Phone);
    cout << "Enter Account Balance: ";
    cin >> C.AccountBalance;
    cin.ignore();
    return C;
}

// Adds a new client to the file
void AddClient() {
    system("cls");
    cout << "========== Add New Client ==========\n\n";

    // Read new client data from the user
    Client C = ReadClientData();

    // Open the file in append mode to add new client data
    fstream File(ClientsFileName, ios::out | ios::app);

    if (File.is_open()) {
        // Write the new client data to the file
        File << ConvertRecordToLine(C) << endl;

        // Close the file after writing
        File.close();
        cout << "\nClient added successfully!\n";
    }
    else {
        cout << "Error writing to file.\n";
    }

    system("pause");
}

// Find a client by account number
void FindClient() {
    system("cls");
    cout << "========== Find Client ==========\n\n";

    // Prompt the user to enter the account number to search for
    string AccNumber;
    cout << "Enter Account Number: ";
    cin >> AccNumber;

    // Load all client data from the file
    vector<Client> Clients = LoadClientsDataFromFile(ClientsFileName);

    // Iterate through the list of clients to find the matching account number
    for (Client C : Clients) {
        if (C.AccountNumber == AccNumber) {
            cout << "\nClient Found:\n";
            // Print the client details if found
            PrintClient(C);
            system("pause");
            return;
        }
    }

    // Display a message if the client is not found
    cout << "\nClient not found.\n";
    system("pause");
}

// Modify client data by account number
void ModifyClient() {
    system("cls");
    cout << "========== Modify Client ==========\n\n";

    // Prompt the user to enter the account number of the client to modify
    string AccNumber;
    cout << "Enter Account Number: ";
    cin >> AccNumber;

    // Load all client data from the file
    vector<Client> Clients = LoadClientsDataFromFile(ClientsFileName);
    bool Found = false;

    // Search for the client by account number
    for (Client& C : Clients) {
        if (C.AccountNumber == AccNumber) {
            cout << "\nClient Found. Enter new data:\n";
            // Read the new data and update the client record
            C = ReadClientData();
            Found = true;
            break;
        }
    }

    // If client is found, save the updated data to the file
    if (Found) {
        SaveAllClientsToFile(ClientsFileName, Clients);
        cout << "\nClient updated successfully.\n";
    }
    else {
        cout << "\nClient not found.\n";
    }

    system("pause");
}

// Delete a client by account number
void DeleteClient() {
    system("cls");
    cout << "========== Delete Client ==========\n\n";

    // Prompt the user to enter the account number of the client to delete
    string AccNumber;
    cout << "Enter Account Number: ";
    cin >> AccNumber;

    // Load all client data from the file
    vector<Client> Clients = LoadClientsDataFromFile(ClientsFileName);

    // Remove the client with the matching account number
    auto it = remove_if(Clients.begin(), Clients.end(), [&](Client C) {
        return C.AccountNumber == AccNumber;
        });

    // If the client was found, erase the record and save the updated data
    if (it != Clients.end()) {
        Clients.erase(it, Clients.end());
        SaveAllClientsToFile(ClientsFileName, Clients);
        cout << "\nClient deleted successfully.\n";
    }
    else {
        cout << "\nClient not found.\n";
    }

    system("pause");
}

// Logs transaction details (type, account number, amount, and optional target account) to a file.
void LogTransaction(const string& Type, const string& AccNum, double Amount, const string& TargetAcc = "") {

    ofstream LogFile("Transactions_Log.txt", ios::app);

    if (!LogFile.is_open()) {
        cerr << "Error: Could not open log file.\n";
        return;
    }

    // Get the current time and format it
    time_t now = time(0);
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &now);
    string timeStr = buffer;
    timeStr.pop_back();  // Remove the newline character

    // Write the transaction details to the log file
    LogFile << "[" << timeStr << "] "
        << "Type: " << Type
        << ", Account: " << AccNum;

    // If a target account is provided, include it in the log entry
    if (!TargetAcc.empty()) {
        LogFile << ", Target Account: " << TargetAcc;
    }

    // Log the transaction amount with two decimal places
    LogFile << ", Amount: " << fixed << setprecision(2) << Amount << "\n";

    LogFile.close();
}

// Performs a withdrawal from a client's account
void PerformWithdraw() {
    system("cls");
    cout << "========== Withdraw ==========\n\n";

    string AccNum;
    double Amount;
    vector<Client> Clients = LoadClientsDataFromFile(ClientsFileName);

    // Prompt the user to enter the account number for the withdrawal
    cout << "Enter Account Number: ";
    cin >> AccNum;

    // Search for the client by account number
    for (Client& C : Clients) {
        if (C.AccountNumber == AccNum) {
            cout << "Enter amount to withdraw: ";
            cin >> Amount;

            // Check if the withdrawal amount is valid
            if (Amount <= 0 || Amount > C.AccountBalance) {
                cout << "\nInvalid or insufficient balance.\n";
            }
            else {
                // Deduct the withdrawal amount from the account balance
                C.AccountBalance -= Amount;
                SaveAllClientsToFile(ClientsFileName, Clients);
                cout << "\nWithdrawal successful. New Balance: " << C.AccountBalance << "\n";
            }

            system("pause");

            // Log the transaction
            LogTransaction("Withdraw", C.AccountNumber, Amount);
            return;
        }
    }

    // If the account number is not found
    cout << "\nAccount not found.\n";
    system("pause");
}

// Performs a deposit into a client's account
void PerformDeposit() {
    system("cls");
    cout << "========== Deposit ==========\n\n";

    string AccNum;
    double Amount;
    vector<Client> Clients = LoadClientsDataFromFile(ClientsFileName);

    // Prompt the user to enter the account number for the deposit
    cout << "Enter Account Number: ";
    cin >> AccNum;

    // Search for the client by account number
    for (Client& C : Clients) {
        if (C.AccountNumber == AccNum) {
            cout << "Enter amount to deposit: ";
            cin >> Amount;

            // Check if the deposit amount is valid
            if (Amount <= 0) {
                cout << "\nInvalid amount.\n";
            }
            else {
                // Add the deposit amount to the account balance
                C.AccountBalance += Amount;
                SaveAllClientsToFile(ClientsFileName, Clients);
                cout << "\nDeposit successful. New Balance: " << C.AccountBalance << "\n";
            }

            system("pause");

            // Log the transaction
            LogTransaction("Deposit", C.AccountNumber, Amount);
            return;
        }
    }

    // If the account number is not found
    cout << "\nAccount not found.\n";
    system("pause");
}

// Performs a balance inquiry for a client's account
void PerformBalanceInquiry() {
    system("cls");
    cout << "========== Balance Inquiry ==========\n\n";

    string AccNum;
    vector<Client> Clients = LoadClientsDataFromFile(ClientsFileName);

    // Prompt the user to enter the account number for the inquiry
    cout << "Enter Account Number: ";
    cin >> AccNum;

    // Search for the client by account number
    for (Client& C : Clients) {
        if (C.AccountNumber == AccNum) {
            // Display the client's name and balance
            cout << "\nClient: " << C.Name << "\n";
            cout << "Balance: " << fixed << setprecision(2) << C.AccountBalance << "\n";
            system("pause");
            return;
        }
    }

    // Log the transaction for balance inquiry
    LogTransaction("Balance Inquiry", AccNum, 0.0);

    // If the account number is not found
    cout << "\nAccount not found.\n";
    system("pause");
}

// Performs a fund transfer between two client accounts
void PerformTransaction() {
    system("cls");
    cout << "========== Transfer Funds ==========\n\n";

    string FromAcc, ToAcc;
    double Amount;
    vector<Client> Clients = LoadClientsDataFromFile(ClientsFileName);

    // Prompt the user to enter the account numbers for transfer
    cout << "From Account Number: ";
    cin >> FromAcc;
    cout << "To Account Number  : ";
    cin >> ToAcc;

    // Check if both accounts are the same
    if (FromAcc == ToAcc) {
        cout << "\nCannot transfer to the same account.\n";
        system("pause");
        return;
    }

    Client* Sender = nullptr;
    Client* Receiver = nullptr;

    // Find the sender and receiver client records
    for (Client& C : Clients) {
        if (C.AccountNumber == FromAcc)
            Sender = &C;
        else if (C.AccountNumber == ToAcc)
            Receiver = &C;
    }

    // Check if sender account is found
    if (!Sender) {
        cout << "\nSender account not found.\n";
        system("pause");
        return;
    }

    // Check if receiver account is found
    if (!Receiver) {
        cout << "\nReceiver account not found.\n";
        system("pause");
        return;
    }

    // Prompt the user to enter the amount to transfer
    cout << "Enter Amount to Transfer: ";
    cin >> Amount;

    // Validate the transfer amount
    if (Amount <= 0) {
        cout << "\nInvalid amount.\n";
        system("pause");
        return;
    }

    // Check if sender has sufficient balance
    if (Sender->AccountBalance < Amount) {
        cout << "\nInsufficient balance in sender's account.\n";
        system("pause");
        return;
    }

    // Perform the transfer
    Sender->AccountBalance -= Amount;
    Receiver->AccountBalance += Amount;

    // Save the updated client data to the file and log the transaction
    SaveAllClientsToFile(ClientsFileName, Clients);
    LogTransaction("Transfer", FromAcc, Amount, ToAcc);

    // Display the transaction result
    cout << "\nTransaction successful!\n";
    cout << fixed << setprecision(2);
    cout << "New Balance - Sender: " << Sender->AccountBalance << "\n";
    cout << "New Balance - Receiver: " << Receiver->AccountBalance << "\n";

    system("pause");
}

// Displays transaction history for a specific account from the transaction log file
void DisplayTransactionsForAccount() {

    system("cls");
    cout << "========== Transaction History ==========\n\n";
    string AccNum;
    vector<Client> Clients = LoadClientsDataFromFile(ClientsFileName);

    // Prompt the user to enter the account number
    cout << "Enter Account Number: ";
    cin >> AccNum;

    // Open the transaction log file for reading
    ifstream LogFile("Transactions_Log.txt");
    if (!LogFile.is_open()) {
        cout << "Error: Cannot open transaction log file.\n";
        system("pause");
        return;
    }

    bool found = false;
    cout << "\nTransactions for Account: " << AccNum << "\n";
    string line;

    // Read through each line in the transaction log file and check for the account number
    while (getline(LogFile, line)) {
        if (line.find(AccNum) != string::npos) {
            cout << line << endl;  // Print the transaction line if the account number is found
            found = true;
        }
    }

    // Inform the user if no transactions were found for the entered account number
    if (!found) {
        cout << "No transactions found for account " << AccNum << ".\n";
    }

    // Close the log file
    LogFile.close();
    cout << "==============================\n";
    system("pause");
}

void TransactionsMenu() {
    int choice;
    do {
        system("cls");
        cout << "========== Transactions Menu ==========\n\n";
        cout << "[1]. Withdraw\n";
        cout << "[2]. Deposit\n";
        cout << "[3]. Transfer\n";
        cout << "[4]. Check Balance\n";
        cout << "[5]. View Transaction Log\n";
        cout << "[6]. Back to Main Menu\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: PerformWithdraw(); break;
        case 2: PerformDeposit(); break;
        case 3: PerformTransaction(); break;
        case 4: PerformBalanceInquiry(); break;
        case 5: DisplayTransactionsForAccount(); break;
        case 6: break;
        default: cout << "\nInvalid choice. Try again.\n"; system("pause"); break;
        }
    } while (choice != 5);
}

void ShowMenu() {
    system("cls");
    cout << "========== Client Management System ==========\n\n";
    cout << "[1]. Add New Client\n";
    cout << "[2]. View All Clients\n";
    cout << "[3]. Find Client\n";
    cout << "[4]. Modify Client\n";
    cout << "[5]. Delete Client\n";
    cout << "[6]. Transactions\n";
    cout << "[7]. Exit\n";
    cout << "\nEnter your choice: ";
}

void Start() {
    int Choice;
    do {
        ShowMenu();
        cin >> Choice;
        switch (Choice) {
        case 1: AddClient(); break;
        case 2: PrintAllClients(LoadClientsDataFromFile(ClientsFileName)); break;
        case 3: FindClient(); break;
        case 4: ModifyClient(); break;
        case 5: DeleteClient(); break;
        case 6: TransactionsMenu(); break;
        case 7: cout << "\nExiting...\n"; break;
        default: cout << "\nInvalid choice. Try again.\n"; system("pause"); break;
        }
    } while (Choice != 7);
}


int main() {
    Start();
    return 0;
}
