#include<stdio.h>
#include<string.h>
#include<time.h>
typedef struct 
{
    int account_no;
    char name[50];
    char address[100];
    int initial_deposit;
}customer;
typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    int account_no;
    char tra[10];         
    char tra_type[10];    
    int amount;
    int balance;
    int d_amount;
    int w_amount;
    Date date;
} info;



int menu();
int open_new_account();
int last_accno();
int list_of_accounts();
int see_individual_account();
int monthly_report(int temp_acc);
int daily_transaction();
int deposit(int temp_acc,char tra[10]);
int withdraw(int temp_acc,char tra[10],int balance);
int transaction(int temp_acc,char tra[10],char tra_type[10],int amount);
int balance(int temp_acc,char tra[10],int amount);
int update_balance(int temp_acc,int balance);
Date get_current_date();
int edit_account();
int modify_account();
int close_account();
int close_transactions(int temp_acc);
int name_modify(const char *filename,int temp_acc);
int address_modify(const char *filename,int temp_acc);
int found_account(const char *filename,int temp_acc);
void add_to_file(customer new,int n, const char *filename);

int main(){
    int choice;
    menu();
    scanf("%d",&choice);
    getchar();
    switch (choice)
    {
    case 1:
        open_new_account();
        break;
    case 2:
        list_of_accounts();
        break;
    case 3:
        see_individual_account();
        break;
    case 4:
        daily_transaction();
        break;
    case 5:
        edit_account();
        break;
    
    default:
        printf("Invaild Choice \n");
        break;
    }
}
int menu(){
    printf("1.Open New Account \n");
    printf("2.List of Accounts \n");
    printf("3.See Individual Account \n");
    printf("4.Daily transaction \n");
    printf("5.Edit account \n");
}
int open_new_account(){
    customer new;
 int n=1;
    int last = last_accno();
    new.account_no= last+1;-
    printf("Enter your name:\n");

    fgets(new.name, sizeof(new.name), stdin);
    
    new.name[strcspn(new.name, "\n")] = '\0';
    if (strlen(new.name) == 0 || new.name[0] == ' ') {
        printf("Invalid name\n");
        return 0;
    }

    printf("Enter your address \n");
   
    fgets(new.address, sizeof(new.address), stdin);

    new.address[strcspn(new.address, "\n")] = '\0';
    if (strlen(new.address) == 0 || new.address[0] == ' ') {
        printf("Invalid address\n");
        return 0;
    }

    printf("Enter initial deposit \n");
    
    scanf("%d",&new.initial_deposit);
    getchar();
    if(new.initial_deposit<500){
        printf("Initial deposit should not be less than 500 \n");
        return 1;
    }
    printf("Your Account number:%d\n",new.account_no);
    add_to_file(new,n,"initial.dat");

}
int last_accno(){
    FILE *fp = fopen("initial.dat","rb");
    if(fp==NULL){
        printf("Try again\n");
        return 0;
    }
    customer temp;
    int last_acc =0;
    while(fread(&temp,sizeof(customer),1,fp)==1){
        if(temp.account_no>last_acc){
            last_acc=temp.account_no;
        }
    }
    fclose(fp);
    return last_acc;
    

}
void add_to_file(customer new,int n, const char *filename){
    FILE *fp = fopen( filename, "ab");
    if (fp == NULL) {
        printf("Try again\n");
        return;
    }
    size_t written_count = fwrite(&new, sizeof(new), n, fp);
    if (written_count == n) {
        printf("Successfully created \n");
    }
    fclose(fp);
    Date today;
    today=get_current_date();
    info temp;
    temp.account_no=new.account_no;
    temp.d_amount=new.initial_deposit;
    temp.balance=new.initial_deposit;
    strcpy(temp.tra_type,"initial");
    strcpy(temp.tra,"D");
    
    temp.amount=0;
    temp.w_amount=0;
    temp.date.day=today.day;
    temp.date.month=today.month;
    temp.date.year=today.year;
    FILE *fpp =fopen("banking.dat","ab");
    if(fpp==NULL){
        printf("try again\n");
        return;
    }
    size_t count = fwrite(&temp, sizeof(temp), n, fpp);
}
int list_of_accounts(){
    FILE *fp = fopen("initial.dat","rb");
    if(fp==NULL){
        printf("try again\n");
        return 0;
    }
    int total=0;
    customer temp;
    printf("Account Number    Name      Address          Balance\n");
    while(fread(&temp,sizeof(customer),1,fp)==1){
         total += temp.initial_deposit;
        printf("%-17d %-9s %-16s %-10d\n",temp.account_no,temp.name,temp.address,temp.initial_deposit);
    }
    fclose(fp);
    printf("\n");
    printf("                    Total balance in bank is:%d",total);
    return 1;
    
}
int see_individual_account(){
    int temp_acc;
    int choice;
    printf("Enter the account number\n");
   
    scanf("%d",&temp_acc);
    getchar();
    
    int value=found_account("initial.dat",temp_acc);
    if(value==0){
        return 0;
    }
    printf("Enter 1 for Global Report\nEnter 2 for Monthly Report\n");
    scanf("%d",&choice);
    switch(choice){
        case 1:
            break;
        case 2:
            monthly_report(temp_acc);
            return 0;
            break;
        default:
            printf("inavlid choice\n");
            break;
    }
    FILE *fp = fopen("initial.dat","rb");
    if(fp==NULL){
        printf("Try again\n");
        return 0;
    }
    customer temp;
    info tempp;
    int count =0;
    Date today;
    today=get_current_date();
    while(fread(&temp,sizeof(customer),1,fp)==1){
        if(temp.account_no==temp_acc){
            printf("Account Number:%d          %s          Date:%02d/%02d/%04d\n",temp.account_no,temp.name,today.day,today.month,today.year);
            printf("Global Report of Account\n\n");
            fclose(fp);
        }
    }
    int d_total=0;
    int w_total=0;

    FILE *fpp = fopen("banking.dat","rb");
    if(fpp==NULL){
        printf("Try again\n");
        return 0;
    }
    printf("Date        Particular Deposit   Withdraw   Balance\n");
    while(fread(&tempp,sizeof(info),1,fpp)==1){
        if(tempp.account_no==temp_acc){
            printf("%02d/%02d/%04d  %-10s %-10d %-10d %-10d\n",tempp.date.day,tempp.date.month,tempp.date.year,tempp.tra_type,tempp.d_amount,tempp.w_amount,tempp.balance);
            d_total+=tempp.d_amount;
            w_total+=tempp.w_amount;
        }
    } fclose(fpp);
    printf("\n");
    printf("Total ->               %d       %d          %d",d_total,w_total,tempp.balance);
    return 1;
}
int monthly_report(int temp_acc){
    int month;
    Date c_month = get_current_date();
    printf("Enter the Month\n");
    scanf("%d",&month);

        FILE *fp = fopen("initial.dat","rb");
            if(fp==NULL){
            printf("Try again\n");
            return 0;
    }
    customer temp;
    info tempp;
    int count =0;
    Date today;
    today=get_current_date();
    while(fread(&temp,sizeof(customer),1,fp)==1){
        if(temp.account_no==temp_acc){
            printf("Account Number:%d          %s          Date:%02d/%02d/%04d\n",temp.account_no,temp.name,today.day,today.month,today.year);
            printf("Monthly Report of Account\n\n");
            fclose(fp);
        }
    }
    int d_total=0;
    int w_total=0;

    FILE *fpp = fopen("banking.dat","rb");
    if(fpp==NULL){
        printf("Try again\n");
        return 0;
    }
    printf("Date        Particular Deposit   Withdraw   Balance\n");
    while(fread(&tempp,sizeof(info),1,fpp)==1){
        if((tempp.account_no==temp_acc)&&(tempp.date.month==month)){
            printf("%02d/%02d/%04d  %-10s %-10d %-10d %-10d\n",tempp.date.day,tempp.date.month,tempp.date.year,tempp.tra_type,tempp.d_amount,tempp.w_amount,tempp.balance);
            d_total+=tempp.d_amount;
            w_total+=tempp.w_amount;
        }
    } fclose(fpp);
    printf("\n");
    printf("Total ->               %d       %d          %d\n",d_total,w_total,tempp.balance);
    printf("Current Balance:%d\n",tempp.balance);
    return 0;
    }

int daily_transaction(){
    info temp;
    customer old;
    int temp_acc;
    printf("Enter the account number\n");
    scanf("%d",&temp_acc);
    int value=found_account("initial.dat",temp_acc);
    if (value==0){
        return 0;
    }
    FILE *fp = fopen("initial.dat", "rb");
    while (fread(&old, sizeof(customer), 1, fp) == 1) {
        if (old.account_no == temp_acc) {
        break;
        }      
    }
    fclose(fp);
    printf("Balance Amount:%d\n",old.initial_deposit);
    printf("Enter D for Deposit\nEnter W for Withdrawal\n");
    getchar();
    fgets(temp.tra,sizeof(temp.tra),stdin);
    temp.tra[strcspn(temp.tra, "\n")] = '\0';
    if (strcmp(temp.tra, "D") == 0) {
        deposit(temp_acc,temp.tra);
    } else if (strcmp(temp.tra, "W") == 0) {
       withdraw(temp_acc,temp.tra,old.initial_deposit);
    } else {
        printf("Invalid action\n");
    }
return 1;
}
int deposit(int temp_acc,char tra[10]){
    info temp;
    printf("Enter the amount\n");
    scanf("%d",&temp.amount);
    printf("Enter the mode of transaction\n");
    printf("Enter 'cash' or 'cheque'\n");
    getchar();
    fgets(temp.tra_type,sizeof(temp.tra_type),stdin);
    temp.tra_type[strcspn(temp.tra_type, "\n")] = '\0';

    if (strcmp(temp.tra_type, "cash") == 0) {
        transaction(temp_acc,tra,temp.tra_type,temp.amount);
    } else if (strcmp(temp.tra_type, "cheque") == 0) {
        transaction(temp_acc,tra,temp.tra_type,temp.amount);
    } else {
        printf("Invalid action\n");
    }
return 1;   
}
int withdraw(int temp_acc,char tra[10],int balance){
    info temp;
    printf("Enter the amount\n");
    scanf("%d",&temp.amount);
    if(temp.amount>balance){
        printf("insufficient balance\n");
        return 0;
    }
    printf("Enter the mode of transaction\n");
    printf("Enter 'cash' or 'cheque'\n");
    getchar();
    fgets(temp.tra_type,sizeof(temp.tra_type),stdin);
    temp.tra_type[strcspn(temp.tra_type, "\n")] = '\0';

   if (strcmp(temp.tra_type, "cash") == 0) {
        transaction(temp_acc,tra,temp.tra_type,temp.amount);
    } else if (strcmp(temp.tra_type, "cheque") == 0) {
        transaction(temp_acc,tra,temp.tra_type,temp.amount);
    } else {
        printf("Invalid action\n");
    }
    return 1;
}
int transaction(int temp_acc,char tra[10],char tra_type[10],int amount){
    info temp = {0};
    temp.account_no = temp_acc;
    strcpy(temp.tra, tra);
    strcpy(temp.tra_type, tra_type);
    if(strcmp(temp.tra,"D")==0){
        temp.d_amount = amount;
        temp.w_amount = 0;
    }else{
        temp.w_amount=amount;
        temp.d_amount=0;
    }
    
    temp.date = get_current_date();
    temp.balance = balance(temp_acc, tra, amount);
    FILE *fp = fopen("banking.dat","ab");
    if(fp==NULL){
        printf("Try again");
        return 0;
    }

    fwrite(&temp,sizeof(info),1,fp);
    fclose(fp);
    return 1;
}
int balance(int temp_acc,char tra[10],int amount){
    customer temp;
    info tempp;
    FILE *fp = fopen("initial.dat", "rb");
    while (fread(&temp, sizeof(customer), 1, fp) == 1) {
        if (temp.account_no == temp_acc) {
        break;
        }      
    }
    fclose(fp);
   
    int balance=0;
    if(strcmp(tra, "D") == 0){
        balance=amount+temp.initial_deposit;
    }
    else{
        balance=temp.initial_deposit-amount;
    }
    update_balance(temp_acc,balance);
    return balance;
}
int update_balance(int temp_acc,int balance){
   int value = found_account("initial.dat",temp_acc);
    if(value==0){
        printf("Unable to updated\n");
        return 0;
    }
    FILE *fp = fopen("initial.dat","rb+");
    customer temp;
    while(fread(&temp,sizeof(customer),1,fp)==1){
        if(temp_acc==temp.account_no){
            temp.initial_deposit=balance;
            fseek(fp,-sizeof(customer),SEEK_CUR);
            fwrite(&temp,sizeof(customer),1,fp);
            break;
        }
    }
    fclose(fp);
    printf("Balance Updated\n");
    return 1;
}
Date get_current_date() {
    time_t now = time(NULL);
    struct tm *local = localtime(&now);

    Date today;
    today.day = local->tm_mday;
    today.month = local->tm_mon + 1;
    today.year = local->tm_year + 1900;

    return today;
}

int edit_account(){
    int choice;
    printf("1.Modify Account \n");
    printf("2.Close Account \n");
    printf("3.Quit\n");
    scanf("%d",&choice);
    switch (choice)
    {
    case 1:
        modify_account();
        break;
    case 2:
        close_account();
        break;
    case 3:
        return 0;;
    default:
        printf("invaild entry \n");
    }
    
}
int modify_account(){
    int temp_acc;
    int choice;
    printf("Enter the account number\n");
    scanf("%d",&temp_acc);
    
    int value=found_account("initial.dat",temp_acc);
    if(value==0){
        return 0;
    }
    printf("What do you want to modify\n1.Name\n2.Address\n");
    scanf("%d",&choice);
   
    switch (choice)
    {
    case 1:
        name_modify("initial.dat",temp_acc);
        break;
    case 2:
        address_modify("initial.dat",temp_acc);
        break;
    default:
        printf("Invalid choice\n");
        break;
    }
}
int name_modify(const char *filename,int temp_acc){
    customer updated;
    printf("Enter the new name\n");
    getchar();
    fgets(updated.name,sizeof(updated.name),stdin);
    updated.name[strcspn(updated.name, "\n")] = '\0';

    FILE *fp =fopen("initial.dat","rb+");
    if(fp==NULL){
        printf("cannot edit\n");
        return 0;
    }
    customer temp;
    while(fread(&temp,sizeof(customer),1,fp)==1){
        if(temp.account_no==temp_acc){
            strcpy(temp.name,updated.name);
            fseek(fp,-sizeof(customer),SEEK_CUR);
            fwrite(&temp,sizeof(customer),1,fp);
            break;
        }
    }
    fclose(fp);
    printf("Name successfully changed\n");
}
int address_modify(const char *filename,int temp_acc){
    customer updated;
    printf("Enter the new Address\n");
    getchar();
    fgets(updated.address,sizeof(updated.address),stdin);
    updated.address[strcspn(updated.address, "\n")] = '\0';

    FILE *fp =fopen("initial.dat","rb+");
    if(fp==NULL){
        printf("cannot edit\n");
        return 0;
    }
    customer temp;
    while(fread(&temp,sizeof(customer),1,fp)==1){
        if(temp.account_no==temp_acc){
            strcpy(temp.address,updated.address);
            fseek(fp,-sizeof(customer),SEEK_CUR);
            fwrite(&temp,sizeof(customer),1,fp);
            break;
        }
    }
    fclose(fp);
    printf("Address successfully changed\n");
}
int close_account(){
    int temp_acc;
    printf("Enter the account number\n");
    getchar();
    scanf("%d",&temp_acc);
    found_account("initial.dat",temp_acc);
    FILE *fp = fopen("initial.dat","rb");
    if(fp==NULL){
        printf("File 1 not found \n");
        return 0;
    }
    FILE *temp_fp = fopen("temp.dat","wb");
    if(temp_fp==NULL){
        printf("File 2 not found \n");
        fclose(fp);
        return 0;
    }
    customer temp;
    int found = 0;
    while (fread(&temp,sizeof(customer),1,fp)==1){
        if(temp.account_no!=temp_acc){
            fwrite(&temp,sizeof(customer),1,temp_fp);
        }else{
            found = 1;
        }
    }
    fclose(fp);
    fclose(temp_fp);
   if(found){
    remove("initial.dat");
    rename("temp.dat","initial.dat");
    printf("Account deleted successfully\n");
   } else{
    remove("temp.dat");
    printf("Account 2 number not found.\n");
   }
   close_transactions(temp_acc);
}
int close_transactions(int temp_acc){
    
    FILE *fp = fopen("banking.dat","rb");
    if(fp==NULL){
        printf("File 1 not found \n");
        return 0;
    }
    FILE *temp_fp = fopen("temp.dat","wb");
    if(temp_fp==NULL){
        printf("File 2 not found \n");
        fclose(fp);
        return 0;
    }
    info temp;
    int found = 0;
    while (fread(&temp,sizeof(info),1,fp)==1){
        if(temp.account_no!=temp_acc){
            fwrite(&temp,sizeof(info),1,temp_fp);
        }else{
            found = 1;
        }
    }
    fclose(fp);
    fclose(temp_fp);
   if(found){
    remove("banking.dat");
    rename("temp.dat","banking.dat");
    printf("Transactions deleted successfully\n");
   } else{
    remove("temp.dat");
    printf("Account 2 number not found.\n");
   }
}
int found_account(const char *filename,int temp_acc){
    FILE *fp = fopen( filename, "rb");
    if (fp == NULL) {
        printf("Try again\n");
        return 0;
    }
    customer temp;
    while (fread(&temp,sizeof(customer),1,fp)==1){
        if(temp.account_no==temp_acc){
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    printf("Account 1 number not found\n");
    return 0;  
}
