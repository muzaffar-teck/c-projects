#include <stdio.h>            
#include <string.h>              
#include <stdlib.h>              
#include <time.h>              
              
#define RESET "\033[0m"              
#define RED "\033[1;31m"              
#define GREEN "\033[1;32m"              
#define YELLOW "\033[1;33m"              
#define BLUE "\033[1;34m"            
#define PINK    "\033[38;5;218m"            
              
#define ADMIN_PASSWORD "admin@123"              
              
#ifdef _WIN32              
#define CLEAR_SCREEN system("cls")              
#else              
#define CLEAR_SCREEN system("clear")              
#endif              
      
      


struct BankAccount              
{              
  char name[50];              
  long long acc_no;              
  int pin;              
  int balance;              
};              
            
struct LoanRequest
{
    long long acc_no;
    int amount;
    int status;
};
            
void printHeader()             
{            
    printf(YELLOW "************************************" RESET "\n");            
    printf(YELLOW "*" PINK "==================================" YELLOW "*" RESET "\n");            
    printf(YELLOW "*" GREEN "   #####   ###    #   #   #   #" YELLOW "   *" RESET "\n");            
    printf(YELLOW "*" GREEN "   #    # #   #   ##  #   #  # " YELLOW "   *" RESET "\n");            
    printf(YELLOW "*" GREEN "   #####  #####   # # #   ### " YELLOW "    *" RESET "\n");            
    printf(YELLOW "*" GREEN "   #    # #   #   #  ##   #  # " YELLOW "   *" RESET "\n");            
    printf(YELLOW "*" GREEN "   #####  #   #   #   #   #   #" YELLOW "   *" RESET "\n");            
    printf(YELLOW "*" PINK "==================================" YELLOW "*" RESET "\n");            
    printf(YELLOW "************************************" RESET "\n");            
    printf("\n\n\n");            
}            
            
            
void getFullName(char *name, int size)             
{              
    printf(YELLOW"ENTER FULL NAME:- "RESET);              
    getchar();              
    fgets(name, size, stdin);              
    name[strcspn(name, "\n")] = 0;              
}              



 void createAccount()                
{             
    struct BankAccount user;            
                    
    CLEAR_SCREEN;             
                   
    printHeader();             
                  
  getFullName(user.name, sizeof(user.name));                
            
while(1)
{
    printf(YELLOW"ENTER ACCOUNT NO (8 digits): "RESET);
    scanf("%lld", &user.acc_no);

    if(user.acc_no < 10000000 || user.acc_no > 99999999)
    {
        printf(RED "Account number must be exactly 8 digits!\n" RESET);
        continue;
    }

    char check_file[100];
    sprintf(check_file, "%lld.txt", user.acc_no);

    FILE *check = fopen(check_file, "r");
    if(check != NULL)
    {
        fclose(check);
        printf(RED "This account number is already used\n" RESET);
        printf(YELLOW "Choose another account number\n" RESET);
        continue;
    }

    break;
}          
            
 while(1)            
 {            
   printf(YELLOW"ENTER PIN (4 digits): "RESET);                
   scanf("%d", &user.pin);                
        if(user.pin >= 1000 && user.pin <= 9999)             
        break;            
   printf(RED "PIN must be exactly 4 digits!\n" RESET);            
 }            
    printf(YELLOW"INITIAL DEPOSIT: "RESET);                
    scanf("%d", &user.balance);                
            
    char muzaffar[100];             
                   
    sprintf(muzaffar, "%lld.txt", user.acc_no);                
    FILE *fp = fopen(muzaffar, "w");                
            
 if (fp != NULL)               
 {                
  fprintf(fp, "%s\n%lld\n%d\n%d\n", user.name, user.acc_no, user.pin, user.balance);                
   fclose(fp);                
 printf(GREEN "\nACCOUNT CREATED SUCCESSFULLY\n" RESET);                
            
  FILE *idx = fopen("accounts_index.txt", "a");                
  if(idx != NULL)                
   {                
      fprintf(idx, "%lld\n", user.acc_no);                
        fclose(idx);                
   }                
 }               
    else               
    {                
        printf(RED "ERROR SAVING ACCOUNT\n" RESET);                
    }                
}            

void addTransaction(long long acc_no, char type[], int amount, long long receiver_acc)              
{              
  char file[100];              
              
   sprintf(file, "%lld_transactions.txt", acc_no);              
               
    FILE *fp = fopen(file, "a");              
                
  if (fp == NULL)            
        return;              
    time_t t = time(NULL);              
    struct tm tm = *localtime(&t);              
    int hour12 = tm.tm_hour % 12;            
    if(hour12 == 0) hour12 = 12;              
    char *ampm = (tm.tm_hour >= 12) ? "PM" : "AM";            
    if (strcmp(type, "Deposit") == 0 || strcmp(type, "Withdraw") == 0)              
        fprintf(fp, "%d %s on %02d-%02d-%04d %02d:%02d:%02d %s\n", amount, type, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, hour12, tm.tm_min, tm.tm_sec, ampm);              
    else if (strcmp(type, "Transfer") == 0)              
        fprintf(fp, "%d sent to %lld on %02d-%02d-%04d %02d:%02d:%02d %s\n", amount, receiver_acc, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, hour12, tm.tm_min, tm.tm_sec, ampm);              
    fclose(fp);              
}            
 
 void applyLoan(long long acc_no)
{
    int choice, amount;
    FILE *fp = fopen("loan_requests.txt", "a");

    printf("\nSelect Loan Amount:\n");
    printf("1. 50000\n2. 100000\n3. 200000\n>>> ");
    scanf("%d", &choice);

    if(choice == 1) amount = 50000;
    else if(choice == 2) amount = 100000;
    else if(choice == 3) amount = 200000;
    else
    {
        printf("Invalid Choice\n");
        return;
    }

    fprintf(fp, "%lld %d 0\n", acc_no, amount);
    fclose(fp);

    printf(GREEN "\nLoan Request Sent to Admin\n" RESET);
}
void showNotifications(long long acc_no)
{
    FILE *fp = fopen("notifications.txt", "r");
    long long a;
    char msg[200];

    if(fp == NULL)
    {
        printf("No Notifications\n");
        return;
    }

    while(fscanf(fp, "%lld %[^\n]\n", &a, msg) != EOF)
    {
        if(a == acc_no)
            printf("🔔 %s\n", msg);
    }

    fclose(fp);
}

void repayLoan(long long acc_no)
{
    FILE *fp = fopen("loan_requests.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    struct LoanRequest lr;
    int foundLoan = 0;
    int remainingAmount = 0;

    
   
  while(fscanf(fp, "%lld %d %d", &lr.acc_no, &lr.amount, &lr.status) != EOF)
  {
   if(lr.acc_no == acc_no && lr.status == 1)
   {
       foundLoan = 1;
       remainingAmount = lr.amount;
   }
        fprintf(temp, "%lld %d %d\n", lr.acc_no, lr.amount, lr.status);
  }

    fclose(fp);
    fclose(temp);

    if(!foundLoan)
    {
        printf(RED "\n⚠️  No approved loan to repay\n" RESET);
        printf(YELLOW "Press Enter to return..." RESET);
        getchar(); getchar();
        return;
    }

    
    char userFile[100];
    sprintf(userFile, "%lld.txt", acc_no);
    FILE *ufp = fopen(userFile, "r+");
    struct BankAccount u;
    fscanf(ufp, "%[^\n]\n%lld\n%d\n%d", u.name, &u.acc_no, &u.pin, &u.balance);

    
    int choice;
    CLEAR_SCREEN;
    printHeader();
    printf(YELLOW "\n💰 Loan Amount Remaining: %d\n\n" RESET, remainingAmount);
    printf(PINK "1. Partial Repayment\n2. Full Repayment\n>>> " RESET);
    scanf("%d", &choice);

    int repayAmount = 0;

    if(choice == 1) 
    {
      printf(YELLOW "\nEnter Amount to Repay: " RESET);
      scanf("%d", &repayAmount);

    if(repayAmount > u.balance)
    {
      printf(RED "\n❌ Insufficient balance to repay this amount\n" RESET);
      fclose(ufp);
      printf(YELLOW "Press Enter to return..." RESET);
      getchar(); getchar();
      return;
    }
     else if(repayAmount > remainingAmount)
      {
        repayAmount = remainingAmount;  
      }

        u.balance -= repayAmount;
        remainingAmount -= repayAmount;

        printf(GREEN "\n✅ Partial repayment successful\n" RESET);
        printf(YELLOW "Remaining Loan: " PINK "%d\n" RESET, remainingAmount);
    }
    else if(choice == 2) 
    {
        if(u.balance < remainingAmount)
        {
            printf(RED "\n❌ Insufficient balance to repay full loan\n" RESET);
            fclose(ufp);
            printf(YELLOW "Press Enter to return..." RESET);
            getchar(); getchar();
            return;
        }
        else
        {
            repayAmount = remainingAmount;
            u.balance -= repayAmount;
            remainingAmount = 0;
            printf(GREEN "\n✅ Full repayment successful\n" RESET);
        }
    }
    else
    {
        printf(RED "\n⚠️  Invalid option\n" RESET);
        fclose(ufp);
        getchar(); getchar();
        return;
    }

    
    rewind(ufp);
    fprintf(ufp, "%s\n%lld\n%d\n%d", u.name, u.acc_no, u.pin, u.balance);
    fclose(ufp);

    
    char tfile[100];
    sprintf(tfile, "%lld_transactions.txt", acc_no);
    FILE *tfp = fopen(tfile, "a");
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    if(choice == 1)
        fprintf(tfp, "%d partial repayment on %02d-%02d-%04d\n", repayAmount, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    else
        fprintf(tfp, "%d full repayment on %02d-%02d-%04d\n", repayAmount, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    fclose(tfp);

    
    fp = fopen("loan_requests.txt", "r");
    temp = fopen("temp.txt", "w");

    while(fscanf(fp, "%lld %d %d", &lr.acc_no, &lr.amount, &lr.status) != EOF)
    {
        if(lr.acc_no == acc_no && lr.status == 1)
        {
            lr.amount = remainingAmount;
            if(remainingAmount == 0) lr.status = 2; // 2 = fully repaid
        }
        fprintf(temp, "%lld %d %d\n", lr.acc_no, lr.amount, lr.status);
    }
    fclose(fp);
    fclose(temp);
    remove("loan_requests.txt");
    rename("temp.txt", "loan_requests.txt");

    
    FILE *nfp = fopen("notifications.txt", "a");
    if(choice == 1)
        fprintf(nfp, "%lld Partial repayment of %d done. Remaining loan: %d\n", acc_no, repayAmount, remainingAmount);
    else
        fprintf(nfp, "%lld Full repayment of %d done.\n", acc_no, repayAmount);
    fclose(nfp);

    printf(YELLOW "\nPress Enter to continue..." RESET);
    getchar(); getchar();
}
           
void loginAccount()              
{              
    long long enter_acc;              
    int enter_pin;             
                
    CLEAR_SCREEN;             
                
    printHeader();             
                
    printf(RED"+++++++++++++++++++++++++++++++\n"RESET);            
    printf("+" YELLOW "       Enter Account No:     " RESET "+\n        ");            
    scanf("%lld", &enter_acc);             
	            
   char muzaffar[100];              
               
    sprintf(muzaffar, "%lld.txt", enter_acc);              
    
    char blocked_file[100];
sprintf(blocked_file, "%lld_blocked.txt", enter_acc);

FILE *bfp = fopen(blocked_file, "r");
if (bfp != NULL)
{
    fclose(bfp);
    printf(RED "\nACCOUNT IS BLOCKED\nContact Admin\n" RESET);
    return;
}
    FILE *fp = fopen(muzaffar, "r");              
      
      int pin_attempts = 0;
                
    if (fp == NULL)             
    {              
        printf(RED "\nNO ACCOUNT FOUND\n" RESET);              
        return;              
    }              
    struct BankAccount user;              
    fscanf(fp, "%[^\n]\n%lld\n%d\n%d", user.name, &user.acc_no, &user.pin, &user.balance);              
    fclose(fp);              
          
          
    while(1)
{
  printf(YELLOW"        Enter Pin: \n        "RESET);
  scanf("%d", &enter_pin);
  printf(RED"+++++++++++++++++++++++++++++++\n"RESET);

  if(enter_pin == user.pin)
    {
        break;
    }
  else
    {
      pin_attempts++;
      printf(RED "Wrong PIN\n" RESET);

      if(pin_attempts == 3)
       {
         char blocked_name[100];
         sprintf(blocked_name, "%lld_blocked.txt", user.acc_no);

         rename(muzaffar, blocked_name);

            printf(RED "\n\tACCOUNT BLOCKED DUE \n" RESET);
            printf(RED "Contact Manager to Recover\n" RESET);
            return;
       }
    }
}
    CLEAR_SCREEN;             
                
    printHeader();            
                
    printf(GREEN "\nLogin Successful\n" RESET);              
            
    while(1)              
    {              
        int op;              
        CLEAR_SCREEN;              
        printHeader();             
        printf(RED "\n\t    +============+\n");            
        printf("\t    | OPERATIONS |\n" RESET);            
        printf(PINK "    +<<<<<<<<<<<<<<>>>>>>>>>>>>>+\n" RESET);            
        printf(PINK "    |" YELLOW "  1. Change Account No     " PINK "|\n" RESET);            
        printf(PINK "    |" YELLOW "  2. Change PIN            " PINK "|\n" RESET);            
        printf(PINK "    |" YELLOW "  3. Deposit               " PINK "|\n" RESET);            
        printf(PINK "    |" YELLOW "  4. Withdraw              " PINK "|\n" RESET);            
        printf(PINK "    |" YELLOW "  5. Account Detail        " PINK "|\n" RESET);            
        printf(PINK "    |" YELLOW "  6. Delete Account        " PINK "|\n" RESET);            
        printf(PINK "    |" YELLOW "  7. Send Money            " PINK "|\n" RESET);            
        printf(PINK "    |" YELLOW "  8. Transactions          " PINK "|\n" RESET);     
        printf(PINK "    |" YELLOW "  9. Apply for Loan        " PINK "|\n" RESET);
        printf(PINK "    |" YELLOW " 10. Notifications         " PINK "|\n" RESET);       
        printf(PINK "    |" YELLOW " 11. Re Pay loan           " PINK "|\n" RESET);
        printf(PINK "    +<<<<<<<<<<<<<<>>>>>>>>>>>>>+\n" RESET);            
        printf(RED "\t    |  12. Exit   |");            
        printf( "\n\t    +============+\n" RESET);            
        printf(YELLOW "\n>>> " RESET);            
        scanf("%d", &op);            
            
 if (op == 1)             
  {              
     long long new_acc;            
       char new_file[100];            
                        
   printf(YELLOW"Enter New Account: "RESET"");              
   scanf("%lld", &new_acc);              
                    
      sprintf(new_file, "%lld.txt", new_acc);            
                     
    if (rename(muzaffar, new_file) == 0)            
     {            
       user.acc_no = new_acc;            
     strcpy(muzaffar, new_file);            
      printf(GREEN "\nAccount Number Updated Successfully\n" RESET);            
    }            
  else            
   {            
      printf(RED "\nError Changing Account Number\n" RESET);            
   }            
  }            
  else if (op == 2)            
  {              
   printf(YELLOW"Enter New Pin: "RESET"");              
   scanf("%d", &user.pin);              
 }              
  else if (op == 3)            
  {            
   char choice;            
   int amount = 0;            
               
            CLEAR_SCREEN;            
                        
      printHeader();            
                  
            printf(RED"\n\t+-------------------------------+\n"RESET);            
            printf(RED"\t|"YELLOW"        DEPOSIT AMOUNT         "RED"|\n"RESET);            
            printf(RED"\t+-------------------------------+\n");            
            printf("\t|"RESET" A."GREEN" 1000       "RESET" B."GREEN" 5000       "RED" |\n");            
            printf("\t|"RESET" C."GREEN" 10000    "RESET"   D. "GREEN"15000      "RED" |\n");            
            printf("\t|"RESET" E."GREEN" 20000     "RESET"  F. "GREEN"25000      "RED" |\n");            
            printf("\t|"RESET" G."GREEN" 40000     "RESET"  H. "GREEN"CUSTOM      "RED"|\n");            
            printf("\t+-------------------------------+\n"RESET);            
            printf("\t>>> ");            
                        
            scanf(" %c", &choice);            
                        
  switch(choice)            
     {            
                case 'A': case 'a':            
                 amount = 1000;             
                 break;            
                case 'B': case 'b':            
                 amount = 5000;             
                 break;            
                case 'C': case 'c':            
                 amount = 10000;             
                 break;            
                case 'D': case 'd':             
                 amount = 15000;             
                break;            
                case 'E': case 'e':             
                 amount = 20000;            
                break;            
                case 'F': case 'f':            
                 amount = 25000;            
                break;            
                case 'G': case 'g':             
                 amount = 40000;             
                break;            
                case 'H': case 'h':            
                            
                    printf("\n\tEnter Custom Amount: ");            
                    scanf("%d", &amount);            
                    break;            
                default:            
                    printf(RED "\nInvalid Choice\n" RESET);            
                    break;            
     }            
       if (amount <= 0)            
       {            
        printf(RED "\nInvalid Amount\n" RESET);            
       }            
       else            
       {            
                user.balance += amount;            
                            
                addTransaction(user.acc_no, "Deposit", amount, 0);            
                            
       printf(GREEN "\nDeposit Successful\n" RESET);            
       printf("Amount Deposited : %d\n", amount);            
       printf("Current Balance  : %d\n", user.balance);            
       }            
      }            
   else if (op == 4)            
     {            
       char choice;            
       int amount = 0;            
                   
            CLEAR_SCREEN;            
                        
        printHeader();            
            printf(RED"\n\t+-------------------------------+\n"RESET);            
            printf(RED"\t|"YELLOW"           WITHDRAW          "RED"|\n"RESET);            
            printf(RED"\t+-------------------------------+\n");            
            printf("\t|"RESET" A."GREEN" 1000       "RESET" B."GREEN" 5000       "RED" |\n");            
            printf("\t|"RESET" C."GREEN" 10000    "RESET"   D. "GREEN"15000      "RED" |\n");            
            printf("\t|"RESET" E."GREEN" 20000     "RESET"  F. "GREEN"25000      "RED" |\n");            
            printf("\t|"RESET" G."GREEN" 40000     "RESET"  H. "GREEN"CUSTOM      "RED"|\n");            
            printf("\t+-------------------------------+\n"RESET);            
            printf("\t>>> ");            
                        
     scanf(" %c", &choice);            
   switch(choice)            
    {            
                case 'A': case 'a':            
                 amount = 1000;             
                 break;            
                case 'B': case 'b':            
                 amount = 5000;             
                 break;            
                case 'C': case 'c':            
                 amount = 10000;             
                 break;            
                case 'D': case 'd':             
                 amount = 15000;             
                break;            
                case 'E': case 'e':             
                 amount = 20000;            
                break;            
                case 'F': case 'f':            
                 amount = 25000;            
                break;            
                case 'G': case 'g':             
                 amount = 40000;             
                break;            
                case 'H': case 'h':            
                            
                    printf("\n\tEnter Custom Amount: ");            
                    scanf("%d", &amount);            
                    break;            
                default:            
                    printf(RED "\nInvalid Choice\n" RESET);            
                    break;            
    }            
   if (amount <= 0)            
   {            
   printf(RED "\nInvalid Amount\n" RESET);            
   }            
      else if (amount > user.balance)            
       {            
         printf(RED "\nInsufficient Balance\n" RESET);            
       }            
        else            
         {            
                user.balance -= amount;            
                            
                addTransaction(user.acc_no, "Withdraw", amount, 0);            
                            
          printf(YELLOW "\n>>>>"GREEN"Withdrawal Successful"YELLOW"<<<<\n" RESET);            
          printf("Amount Withdrawn : %d\n", amount);            
          printf("Remaining Balance: %d\n", user.balance);            
         }            
        }            
                    
  else if (op == 5)             
  {              
    printf(GREEN"<---->"RESET""PINK"DETAILS"RESET""GREEN"<---->\n"RESET);              
    printf(YELLOW"Name:"RESET" %s\n", user.name);              
    printf(YELLOW"Account No: "RESET"%lld\n", user.acc_no);              
    printf(YELLOW"Pin:"RESET" %d\n", user.pin);              
    printf(YELLOW"Balance:"RESET" %d\n", user.balance);              
  }              
  else if (op == 6)             
  {              
    if(remove(muzaffar) == 0)              
     {              
        printf(GREEN "\nAccount Deleted Successfully" RESET);              
        printf(RED" \n   >>>>>>>>>>>>><<<<<<<<<<<<< \n"RESET);            
     }              
     else printf(RED "\nError Deleting Account\n" RESET);              
            break;              
  }             
              
  else if (op == 7)             
  {            
     long long recv_acc;            
      int send_amount;            
                  
   printf(YELLOW "\n\t°°°°°°°°°°°°°°°°°°°°°°°\n" RESET);            
printf(YELLOW "\t°" RESET " Receiver Account No " YELLOW "°\n    " RESET);            
printf(YELLOW "\t°°°°°°°°°°°°°°°°°°°°°°°\n      " RESET);            
printf("\t");            
scanf("%lld", &recv_acc);            
            
printf(YELLOW "\t°°°°°°°°°°°°°°°°°°°°°°°\n" RESET);            
printf(YELLOW "\t°" RESET " Enter Amount        " YELLOW "°\n    " RESET);            
printf(YELLOW "\t°°°°°°°°°°°°°°°°°°°°°°°\n     " RESET);            
printf("\t");            
scanf("%d", &send_amount);            
            
     if(send_amount > user.balance)            
      {            
        printf(RED "Insufficient Balance\n" RESET);            
        printf("      <<<<<<<<<>>>>>>>>\n");            
      }            
   else            
   {            
     user.balance -= send_amount;            
     addTransaction(user.acc_no, "Transfer", send_amount, recv_acc);            
            
       char recv_file[100];            
                   
     sprintf(recv_file, "%lld.txt", recv_acc);            
                 
       FILE *rfp = fopen(recv_file, "r");             
      if(rfp == NULL)            
      {            
       printf(RED "Receiver Account Not found\n" RESET);            
        user.balance += send_amount;            
      }            
                  
   else            
   { struct BankAccount recv_user;            
               
      fscanf(rfp, "%[^\n]\n%lld\n%d\n%d", recv_user.name, &recv_user.acc_no, &recv_user.pin, &recv_user.balance);            
       fclose(rfp);            
                   
            recv_user.balance += send_amount;            
            
      rfp = fopen(recv_file, "w");            
     fprintf(rfp, "%s\n%lld\n%d\n%d\n", recv_user.name, recv_user.acc_no, recv_user.pin, recv_user.balance);            
      fclose(rfp);            
            
      printf(GREEN "Money Sent Successfully\n" RESET);            
                }            
            }             
        }            
  else if (op == 8)             
    {            
      char tfile[100];            
                  
     sprintf(tfile, "%lld_transactions.txt", user.acc_no);            
      FILE *tfp = fopen(tfile, "r");            
                  
        if (tfp == NULL)            
         {            
           printf(RED "No Transaction Found\n" RESET);            
         }            
        else            
        { char line[200];            
                    
         printf("\n Transaction History\n");            
         printf("<><><><><><><><><><><><><><><><><><><><><><><><>\n");            
                        
           while(fgets(line, sizeof(line), tfp))            
         {            
          printf("%s", line);            
         }            
           fclose(tfp);            
        }            
    }            
                
   else if(op == 9)
{
    applyLoan(user.acc_no);
}
   else if(op == 10)
{
    showNotifications(user.acc_no);
}
   else if(op == 11)
{
    repayLoan(user.acc_no);
}
   else if(op == 12)
{
    printf("\nExiting...");
    break;
}
    if (op != 6)             
    {              
       FILE *fp = fopen(muzaffar, "w");              
                   
     if (fp != NULL)             
      {              
       fprintf(fp, "%s\n%lld\n%d\n%d", user.name, user.acc_no, user.pin, user.balance);              
           fclose(fp);              
        printf(GREEN "\nAccount Updated Successfully\n" RESET);              
      }             
      else             
      printf(RED "Error Updating Account\n" RESET);              
    }              
            
    if(op != 6)             
    {              
     printf(YELLOW "\nPress Enter to Continue...." RESET);              
     getchar(); getchar();              
    }              
 }              
}             
            
            
void adminViewAllAccounts()              
{              
    FILE *fp;             
    struct BankAccount user;              
    char filename[100];             
    long long acc;              
    long long printed[1000];             
    int printed_count = 0;            
            
    fp = fopen("accounts_index.txt", "r");              
    if(fp == NULL)             
 {             
  printf(RED "No Account Found\n" RESET);             
   return;             
 }              
            
    printf(GREEN "\nAll Active Accounts\n" RESET);              
    printf(RED"<><><><><><><><><><><><><><><><><><><><><>\n\n"RESET);            
            
    while(fscanf(fp, "%lld", &acc) == 1)              
    {              
                    
        int duplicate = 0;            
        int i;            
     for( i = 0; i < printed_count; i++)            
     {            
      if(printed[i] == acc)             
       {            
         duplicate = 1;            
         break;            
       }            
     }            
        if(duplicate) continue;            
            
        sprintf(filename, "%lld.txt", acc);              
        FILE *afp = fopen(filename, "r");              
        if(afp == NULL)             
            continue;              
            
        if(fscanf(afp, "%[^\n]\n%lld\n%d\n%d", user.name, &user.acc_no, &user.pin, &user.balance) == 4)              
        {            
            printf("Name: %s | Acc: %lld | Balance: %d\n", user.name, user.acc_no, user.balance);              
            printed[printed_count++] = acc;            
        }            
            
        fclose(afp);              
    }              
    fclose(fp);              
}            
void adminBlockAccount()              
{              
    long long acc;             
    char file[100], blocked[120];              
    printf(" to Block:");              
    scanf("%lld", &acc);              
    sprintf(file, "%lld.txt", acc);              
    sprintf(blocked, "%lld_blocked.txt", acc);              
    if(rename(file, blocked) == 0)             
        printf(GREEN "Account Blocked Successfully\n×<><><><><><>×" RESET);              
    else             
        printf(RED "Account Not found\n ^^^^^^^^^^" RESET);              
}              
            
void adminActivateAccount()              
{              
    long long acc;             
    char file[100], active[120];              
    printf(" to Activate:");              
    scanf("%lld", &acc);              
    sprintf(file, "%lld_blocked.txt", acc);              
    sprintf(active, "%lld.txt", acc);              
    if(rename(file, active) == 0)             
        printf(GREEN "Account Activated Successfully\n×<><><><><><>×" RESET);              
    else             
        printf(RED "Blocked Account Not found\n ^^^^^^^^^^" RESET);              
}              
            
void adminViewTransactions()              
{              
   long long acc;              
   char file[100], line[200];              
   printf("Enter Account_No: ");              
   scanf("%lld", &acc);              
   sprintf(file, "%lld_transactions.txt", acc);              
   FILE *fp = fopen(file, "r");              
   if(fp == NULL)             
   {             
        printf(RED "No Transaction Found\n   ×<><><><><><>×" RESET);             
        return;             
   }              
   printf(GREEN "\nTransaction History:\n" RESET);              
   while(fgets(line, sizeof(line), fp))             
        printf("%s", line);              
   fclose(fp);              
}              
void adminTotalBankBalance()            
{            
    FILE *fp;            
    long long acc;            
    struct BankAccount user;            
    char filename[100];            
    long long total_balance = 0;            
    int total_accounts = 0;            
            
    fp = fopen("accounts_index.txt", "r");            
    if(fp == NULL)            
    {            
        printf(RED "No Account Found\n" RESET);            
        return;            
    }            
            
    while(fscanf(fp, "%lld", &acc) == 1)            
    {            
        sprintf(filename, "%lld.txt", acc);            
        FILE *afp = fopen(filename, "r");            
        if(afp == NULL) continue;            
            
        if(fscanf(afp, "%[^\n]\n%lld\n%d\n%d", user.name, &user.acc_no, &user.pin, &user.balance) == 4)            
        {            
            total_balance += user.balance;            
            total_accounts++;            
        }            
        fclose(afp);            
    }            
            
    fclose(fp);            
            
    printf(GREEN "\nTotal Accounts: %d\n" RESET, total_accounts);            
    printf(GREEN "Total Bank Balance: %lld\n" RESET, total_balance);            
}           

void adminViewLoans()
{
    FILE *fp = fopen("loan_requests.txt", "r");
    struct LoanRequest lr;

    if(fp == NULL)
    {
        printf("No Loan Requests\n");
        return;
    }

    while(fscanf(fp, "%lld %d %d", &lr.acc_no, &lr.amount, &lr.status) != EOF)
    {
        if(lr.status == 0)
            printf("Acc: %lld | Amount: %d | Pending\n", lr.acc_no, lr.amount);
    }

    fclose(fp);
}

void adminApproveLoan()
{
    long long acc;
    int foundRequest;

  while(1)  
  {
    CLEAR_SCREEN;
    printHeader();

    printf(YELLOW "Enter Account No to Approve Loan: " RESET);
    scanf("%lld", &acc);

        
     char accFile[100];
     sprintf(accFile, "%lld.txt", acc);

     FILE *ufp = fopen(accFile, "r");
     if(ufp == NULL)
     {
       printf(RED "\nNo account exists\n" RESET);
       printf(YELLOW "Press Enter to try again..." RESET);
       getchar(); getchar();
        continue;   
     }
        fclose(ufp);
        
      FILE *fp = fopen("loan_requests.txt", "r");
      FILE *temp = fopen("temp.txt", "w");

    struct LoanRequest lr;
    foundRequest = 0;

  while(fscanf(fp, "%lld %d %d", &lr.acc_no, &lr.amount, &lr.status) != EOF)
   {
    if(lr.acc_no == acc && lr.status == 0)
    {
      foundRequest = 1;
      lr.status = 1;

       struct BankAccount u;
       ufp = fopen(accFile, "r+");
       fscanf(ufp, "%[^\n]\n%lld\n%d\n%d", u.name, &u.acc_no, &u.pin, &u.balance);
       u.balance += lr.amount;

        rewind(ufp);
        fprintf(ufp, "%s\n%lld\n%d\n%d", u.name, u.acc_no, u.pin, u.balance);
       fclose(ufp);
                FILE *nfp = fopen("notifications.txt", "a");
                fprintf(nfp, "%lld Your loan of %d has been approved and credited\n",
                        acc, lr.amount);
                fclose(nfp);
            }
            fprintf(temp, "%lld %d %d\n", lr.acc_no, lr.amount, lr.status);
        }

        fclose(fp);
        fclose(temp);
        remove("loan_requests.txt");
        rename("temp.txt", "loan_requests.txt");

        
        if(!foundRequest)
        {
            printf(RED "\nThis account has not applied for any loan\n" RESET);
            printf(YELLOW "Press Enter to try again..." RESET);
            getchar(); getchar();
            continue;
        }

        printf(GREEN "\nLoan Approved Successfully\n" RESET);
        printf(YELLOW "Press Enter to return to Admin Panel..." RESET);
        getchar(); getchar();
        break;   
    }
}

void adminPanel()      
{      
      
      int choice;
    char pass[50];

  while(1)
  {
    CLEAR_SCREEN;
    printHeader();

    printf(YELLOW "\t\tEnter Admin Password: \n                      " RESET);
    scanf("%s", pass);

    if(strcmp(pass, ADMIN_PASSWORD) == 0)
    {
        printf(GREEN "\nAdmin Login Successful\n" RESET);
        break;
    }
    else
    {
        printf(RED "Wrong Admin Password! Try Again\n" RESET);
        getchar(); getchar();
    }
  }
 while(1)              
 {              
        CLEAR_SCREEN;              
        printHeader();             
        printf(BLUE "\n      ===== "RED"ADMIN PANEL"BLUE" =====\n\n" RESET);             
        printf(BLUE    "     °°°°°°°°°°°°°°"RED"°°°°°°°°°°°°°°\n"RESET);             
        printf(PINK"     \t1. View All Accounts\n");              
        printf("     \t2. Block Any Account\n");              
        printf("     \t3. Activate Blocked Acc\n");            
        printf("     \t4. Account Transactions\n");              
        printf("     \t5. Total Balance in Bank\n");            
        printf("     \t6. View Loan Requests\n ");            
        printf("     \t7. Approve loan\n ");
        printf(BLUE    "     °°°°°°°°°°°°°°"RED"°°°°°°°°°°°°°°\n"RESET);             
        printf(BLUE "\n      ===== "RED"8. EXIT PANEL"BLUE" =====\n\n" RESET);             
        printf(YELLOW"      \n\n>>> "RESET);            
        scanf("%d", &choice);              
                    
   if(choice == 1)             
    {            
     adminViewAllAccounts();              
    }            
    else if(choice == 2)            
     { adminBlockAccount();  }            
     else if(choice == 3)            
    {            
      adminActivateAccount();              
     }            
     else if(choice == 4)            
    {            
     adminViewTransactions();              
     }            
     else if(choice == 5)            
     {            
      adminTotalBankBalance();            
      }            
      else if(choice == 6)
      {
      adminViewLoans();
      }
      else if(choice == 7)
      {
      adminApproveLoan();
      }
    else if(choice == 8)             
    {            
      printf(GREEN "\nExiting Admin Panel....\n" RESET);            
      break;             
    }              
        else printf(RED "Invalid option\n" RESET);              
        printf(YELLOW "\nPress Enter to return to Admin Menu..." RESET);              
        getchar(); getchar();              
 }              
}              
            

int main()
{
  int choice;

  while(1)
 {
        CLEAR_SCREEN;
        printHeader();

    printf(GREEN"\tø"RED"Ø"PINK"ø"BLUE"Ø"YELLOW"ø"GREEN"ø"RED"Ø"PINK"ø"BLUE"Ø"YELLOW"ø"GREEN"ø"RED"Ø"PINK"ø"BLUE"Ø"YELLOW"ø"GREEN"ø"RED"Ø"PINK"ø"BLUE"Ø"YELLOW"ø"GREEN"ø"RED"Ø"PINK"ø\n"RESET);
    printf(RED "            1. Create Account         \n");
   printf("            2. Login                  \n");
    printf("            3. Admin Panel            \n");
    printf("            4. Exit                   \n"RESET);
   printf(GREEN"\tø"RED"Ø"PINK"ø"BLUE"Ø"YELLOW"ø"GREEN"ø"RED"Ø"PINK"ø"BLUE"Ø"YELLOW"ø"GREEN"ø"RED"Ø"PINK"ø"BLUE"Ø"YELLOW"ø"GREEN"ø"RED"Ø"PINK"ø"BLUE"Ø"YELLOW"ø"GREEN"ø"RED"Ø"PINK"ø\n"RESET);
  printf(YELLOW "\n>>> " RESET);
        scanf("%d", &choice);

   if(choice == 1)
   {
     createAccount();
   }
   else if(choice == 2)
   {
    loginAccount();  
   }
   else if(choice == 3)
   {
    adminPanel();   
   }
   else if(choice == 4)
   {
    printf(GREEN "\nExiting Program...\n" RESET);
    break;          
   }
   else
   {
      printf(RED "Invalid choice\n" RESET);
      getchar(); getchar();
   }
 }
    return 0;
}