
# Bank Simulation

## Overview
This Client Management System, developed in C++, enables efficient management of client information. It supports various operations including adding, modifying, deleting clients, and handling financial transactions such as deposits, withdrawals, balance inquiries, and fund transfers. Client data is securely stored in text files with basic encryption applied to PIN codes for added security.

## Features

- **Add New Client:** Allows users to add a new client by providing account details such as account number, PIN, name, phone number, and account balance.
- **Modify Client:** Enables modification of an existing client’s details.
- **Delete Client:** Lets users delete clients by their account number.
- **Find Client:** Search and display a client’s details by their account number.
- **Withdraw:** Facilitates withdrawal of money from a client’s account.
- **Deposit:** Allows deposit of money into a client’s account.
- **Balance Inquiry:** Enables users to check the current balance of a client’s account.
- **Fund Transfer:** Supports transferring funds between two client accounts.
- **Transaction Logging:** Records every financial transaction (withdrawals, deposits, transfers) along with timestamps for security and auditing purposes.

## File Storage

- **Client Data**: All client information (account number, PIN, name, phone number, balance) is stored in `Clients.txt`. PIN codes are encrypted for security purposes.
- **Transaction Log**: A log of all transactions (withdrawals, deposits, transfers, inquiries) is stored in `Transactions_Log.txt` with timestamps.

## Data Encryption

The system employs a simple encryption mechanism for PIN codes. Each character in the PIN is shifted by 3 positions as a demonstration of basic encryption principles.

## File Structure

### Clients Data (Clients.txt)

Each line in the `Clients.txt` file contains information about a client in the following format:

```
AccountNumber#//#EncryptedPinCode#//#Name#//#Phone#//#AccountBalance
```

### Transaction Log (Transactions_Log.txt)

Each line in the `Transactions_Log.txt` file logs a transaction with the following details:

```
[Timestamp] Type: TransactionType, Account: AccountNumber, Amount: TransactionAmount
```

## How to Run

1. Compile the code using a C++ compiler. For example, using g++:

   ```
   g++ -o BankSimulation BankSimulation.cpp
   ```

2. Run the program:

   ```
   ./BankSimulation
   ```

   A menu will appear, allowing you to perform various operations on client data and financial transactions.

## Example Usage

### Add Client
When prompted, enter the client’s details, and they will be added to the system.

### Withdraw
Enter the account number and withdrawal amount. The system will verify the withdrawal conditions.

### Fund Transfer
Provide the source and destination account numbers along with the transfer amount. The system checks account validity and ensures the source account has sufficient funds.

## Possible Improvements

- Implement a more secure encryption algorithm for storing sensitive information.
- Improve error handling for file operations.
- Add user authentication to restrict system access.
