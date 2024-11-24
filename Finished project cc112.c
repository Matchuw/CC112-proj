#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define USERS 100 // max user
#define TRANSACTIONSLIMIT 100 // max transaction limit
#define USERNAMELENGTH 50 // max username length
#define PASSWORDLENGTH 50 // max password length
#define ACCNO 20 // max account
#define TRANSACTIOND 100 // transaction description 
#define BALANCELIMIT 1000000 // 
#define INTERESTRATE 0.05 // ong random rate 
#define PINLENGTH 6  // standard length for the ATM PIN
#define MAXATTEMPTS 3  // Max failed attempts for PIN entry

// Structure to store user information
typedef struct {
    char MathewAlarcon[USERNAMELENGTH]; // for user username
    char LorrelynPaga[PASSWORDLENGTH]; // for user password
    char LeighCleofas[ACCNO]; // for user ID bank id eya
    float KrishnaSeno; // foor user account balance 
    int transactioncount; // Transaction count
    char LorrainePEPITO[TRANSACTIONSLIMIT][TRANSACTIOND]; //transaction History
    char atmpin[PINLENGTH];  // Store the ATM PIN!!!!!
    int failedattempts;  // for tracking of failed ATM PIN attempts
} User;

// Global array to store users
User users[USERS];
int usercount = 0;

// Function prototypes
void registeruser();
int loginuser();
void deposit(User* user);
void withdraw(User* user);
void viewbalance(User* user);
void calinterest(User* user); // interest calculator ye
void VIEWtransactionhistory(User* user);
void generatereceipt(User* user, const char* transDESCRIPTION, float amount);
void ATMtry(User* user); // an atm inside a bank?? innovation lets go!
int authenticateatm(User* user);  // Function to authenticate user for ATM
void RESETattemptFailed(User* user); // Reset the failed attempts after successful login

// Utility functions
void clearbuffer(); //removes leftover data input/output for unexpected shit happen
void PRINTmenu(); // FOR MENU OPTION
void printATMmenu(); // AS THE VARIABLE NAME SUGGEST YEA
void LOGactivity(const char* message);  // Log system activity to a file
void welcumMESSAGE();  // printed welcome message 

// Main function
int main() {
    int loggedin = 0;
    int choice;
    int userID = -1; // Nag error remove pag di pa naayos bukas
    

    // login register menu
    while (1) {
        welcumMESSAGE();

        printf("1. Register\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearbuffer();

        switch (choice) {
            case 1:
                registeruser();
                break;
            case 2:
                userID = loginuser();
                if (userID != -1) {
                    loggedin = 1;
                    break;
                }
                break;
            case 3:
                printf("ERM what a sigma.\n");
                LOGactivity("System Exit"); // Log system exit 
                exit(0);
            default:
                printf("OOPS! wrong choice. Please try again.\n");
        }
        // 
        
        if (loggedin) {
            while (loggedin) {
                PRINTmenu();
                printf("Enter your choice: ");
                scanf("%d", &choice);
                clearbuffer();

                switch (choice) {
                    case 1:
                        deposit(&users[userID]);
                        break;
                    case 2:
                        withdraw(&users[userID]);
                        break;
                    case 3:
                        viewbalance(&users[userID]);
                        break;
                    case 4:
                        calinterest(&users[userID]);
                        break;
                    case 5:
                        VIEWtransactionhistory(&users[userID]);
                        break;
                    case 6:
                        ATMtry(&users[userID]);  // Enter ATM  mode
                        break;
                    case 7:
                        loggedin = 0;
                        RESETattemptFailed(&users[userID]);  // Reset failed attempts on logout
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            }
        }
    }

    return 0;
}

// Function to clear the input buffer
void clearbuffer() {
    while (getchar() != '\n');
}

// Function to print the main menu for logged in users
void PRINTmenu() {
    printf("\n--- Main Menu ---\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. View Balance\n");
    printf("4. Calculate Interest\n");
    printf("5. View Transaction History\n");
    printf("6. Access ATM\n");  // Option to go to ATM 
    printf("7. Logout\n");
}

// Function to print the ATM menu
void printATMmenu() {
    printf("\n--- ATM Menu ---\n");
    printf("1. Deposit\n");
    printf("2. Withdraw\n");
    printf("3. View Balance\n");
    printf("4. View Transaction History\n");
    printf("5. Exit ATM\n");  // Exit from ATM mode and return to the main menu
}

// Function to register a new user
void registeruser() {
    if (usercount >= USERS) {
        printf("Registration limit reached. Cannot register new users.\n");
        return;
    }

    User NEWuser;
    printf("Enter Username: ");
    fgets(NEWuser.MathewAlarcon, USERNAMELENGTH, stdin);
    NEWuser.MathewAlarcon[strcspn(NEWuser.MathewAlarcon, "\n")] = 0; // Remove newline character

    printf("Enter password: ");
    fgets(NEWuser.LorrelynPaga, PASSWORDLENGTH, stdin);
    NEWuser.LorrelynPaga[strcspn(NEWuser.LorrelynPaga, "\n")] = 0; // Remove newline character

    // Set up ATM PIN
    printf("Set your ATM PIN (6 digits): ");
    fgets(NEWuser.atmpin, PINLENGTH, stdin);
    NEWuser.atmpin[strcspn(NEWuser.atmpin, "\n")] = 0; // Remove newline character

    snprintf(NEWuser.LeighCleofas, ACCNO, "ACC%04d", usercount + 1);
    NEWuser.KrishnaSeno = 0.0f;
    NEWuser.transactioncount = 0;
    NEWuser.failedattempts = 0; // Initialize failed attempts

    users[usercount] = NEWuser;
    usercount++;
    printf("User registered successfully. Your account number is %s.\n", NEWuser.LeighCleofas);
    LOGactivity("New user registered.");
}

// Function to log in a user
int loginuser() {
    char MathewAlarcon[USERNAMELENGTH];
    char LorrelynPaga[PASSWORDLENGTH];
    
    printf("Enter username: ");
    fgets(MathewAlarcon, USERNAMELENGTH, stdin);
    MathewAlarcon[strcspn(MathewAlarcon, "\n")] = 0; // Remove newline character

    printf("Enter password: ");
    fgets(LorrelynPaga, PASSWORDLENGTH, stdin);
    LorrelynPaga[strcspn(LorrelynPaga, "\n")] = 0; // Remove newline character

    for (int i = 0; i < usercount; i++) {
        if (strcmp(users[i].MathewAlarcon, MathewAlarcon) == 0 && strcmp(users[i].LorrelynPaga, LorrelynPaga) == 0) {
            printf("Login successful! Welcome, %s.\n", users[i].MathewAlarcon);
            LOGactivity("User logged in.");
            return i;
        }
    }

    printf("Invalid username or password. Please try again.\n");
    return -1;
}

// eto na for depositing money lets go its working!!
void deposit(User* user) {
    float amount;
    printf("Enter amount to deposit: ");
    scanf("%f", &amount);
    clearbuffer();

    if (amount <= 0) {
        printf("Invalid amount. Deposit must be greater than zero.\n");
        return;
    }

    if (user->KrishnaSeno + amount > BALANCELIMIT) {
        printf("Deposit exceeds maximum balance limit of %.2f. Cannot deposit.\n", BALANCELIMIT);
        return;
    }

    user->KrishnaSeno += amount;
    snprintf(user->LorrainePEPITO[user->transactioncount], TRANSACTIOND, "Deposited: %.2f", amount);
    user->transactioncount++;

    printf("Deposited %.2f successfully. New balance: %.2f.\n", amount, user->KrishnaSeno);
    LOGactivity("Deposit made.");
}

// Function to withdraw money
void withdraw(User* user) {
    float amount;
    printf("Enter amount to withdraw: ");
    scanf("%f", &amount);
    clearbuffer();

    if (amount <= 0) {
        printf("Invalid amount. Withdrawal must be greater than zero.\n");
        return;
    }

    if (amount > user->KrishnaSeno) {
        printf("Insufficient balance. Your current balance is %.2f.\n", user->KrishnaSeno);
        return;
    }

    user->KrishnaSeno -= amount;
    snprintf(user->LorrainePEPITO[user->transactioncount], TRANSACTIOND, "Withdrew: %.2f", amount); // snprintf print the specified string 
    user->transactioncount++;

    
    generatereceipt(user, "Withdrawal", amount);

    printf("Withdrawal of %.2f successful. New balance: %.2f.\n", amount, user->KrishnaSeno);
    LOGactivity("Withdrawal made.");
}

// Function to view account balance
void viewbalance(User* user) {
    printf("Your current balance is: %.2f\n", user->KrishnaSeno);
}

// Function for interest calculator
void calinterest(User* user) {
    float interest = user->KrishnaSeno * INTERESTRATE;
    printf("Interest calculated at %.2f%%: %.2f\n", INTERESTRATE * 100, interest);
    user->KrishnaSeno += interest;
    snprintf(user->LorrainePEPITO[user->transactioncount], TRANSACTIOND, "Interest added: %.2f", interest);
    user->transactioncount++;
    printf("New balance after interest: %.2f\n", user->KrishnaSeno);
}

// Function to view transaction history
void VIEWtransactionhistory(User* user) {
    if (user->transactioncount == 0) {
        printf("No transactions yet.\n");
    } else {
        printf("\n--- Transaction History ---\n");
        for (int i = 0; i < user->transactioncount; i++) {
            printf("%s\n", user->LorrainePEPITO[i]);
        }
    }
}
// GAWA resibo every withdraw cool shit credit to:  random overflow user // every withdrew generate in text file receipt
void generatereceipt(User* user, const char* transDESCRIPTION, float amount) {
    char filename[100];
    snprintf(filename, sizeof(filename), "%s_receipt.txt", user->LeighCleofas);
    
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Error opening file for receipt generation.\n");
        return;
    }

    time_t t; // TIMEEEEEEE
    time(&t);
    fprintf(file, "----- Receipt -----\n");
    fprintf(file, "Account Number: %s\n", user->LeighCleofas);
    fprintf(file, "Transaction: %s %.2f\n", transDESCRIPTION, amount);
    fprintf(file, "Date and Time: %s", ctime(&t));
    fprintf(file, "New Balance: %.2f\n", user->KrishnaSeno);
    fprintf(file, "-------------------\n\n");

    fclose(file);
    printf("Receipt generated for withdrawal: %s_receipt.txt\n", user->LeighCleofas);
}

// Function  para sa authentication 
int authenticateatm(User* user) {
    char pin[PINLENGTH];
    
    if (user->failedattempts >= MAXATTEMPTS) {
        printf("Account locked due to too many failed attempts.\n");
        return 0;  // Lock account after too many failed attempts
    }
    
    printf("Enter your ATM PIN: ");
    fgets(pin, PINLENGTH, stdin);
    pin[strcspn(pin, "\n")] = 0; // Remove newline character

    if (strcmp(user->atmpin, pin) == 0) {
        return 1; // PIN is correct
    } else {
        user->failedattempts++;
        printf("Invalid ATM PIN. Access denied.\n");
        return 0; // PIN is incorrect
    }
}

// ATM simulation mode
void ATMtry(User* user) {
    // Authenticate ATM PIN before entering ATM mode
    if (!authenticateatm(user)) {
        return; // Exit if authentication fails
    }

    int atm_choice;
    while (1) {
        printATMmenu();
        printf("Enter your choice: ");
        scanf("%d", &atm_choice);
        clearbuffer();

        switch (atm_choice) {
            case 1:
                deposit(user);
                break;
            case 2:
                withdraw(user);
                break;
            case 3:
                viewbalance(user);
                break;
            case 4:
                VIEWtransactionhistory(user);
                break;
            case 5:
                return; // Exit ATM mode and return to the main menu
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
// ERROR PUTA BUKAS AYUSIN then add function pang reset ng log in attemp NOTE!!!!! TULOY YUNG sa ↑↑ time_t  ↓↓

// Function to reset failed attempts after successful login
void RESETattemptFailed(User* user) {
    user->failedattempts = 0; // Reset on successful logout
}

// Function to log system activity to a file
void LOGactivity(const char* message) {
    FILE *logfile = fopen("system_log.txt", "a");
    if (logfile != NULL) {
        time_t now;
        time(&now);
        fprintf(logfile, "[%s] %s\n", ctime(&now), message);
        fclose(logfile);
    }
}

// as the variables names suggest ofcourse it's welcome messages yay! 
void welcumMESSAGE() {
    printf("Welcome to the Pancit canton SECURE BANK promise!\n");
    printf("Your trusted partner for all financial needs!\n\n");
    // to sum it up all these codes is for basic BANKING system yeah!! that can handle multiple user or account and have user authentication and ATM simulation with PIN ofc
// tho it only last until runtime afterward the account is byebye 
}
