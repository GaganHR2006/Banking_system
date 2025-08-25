#include<stdio.h>
typedef struct 
{
    int account_no;
    char name[50];
    char address[100];
    int initial_deposit;
}customer;

int menu();
int open_new_account();
int list_of_accounts();
int see_individual_account();
int daily_transaction();
int edit_account();
void modify_account();
int close_account();
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
    new.account_no = __INT_MAX__ - 1;
    printf("Enter your name:\n");
    fgets(new.name, sizeof(new.name), stdin);
    printf("Enter your address \n");
    fgets(new.address, sizeof(new.address), stdin);
    printf("Enter initial deposit \n");
    scanf("%d",&new.initial_deposit);
    if(new.initial_deposit<500){
        printf("Initial deposit should not be less than 500 \n");
        return 1;
    }
    add_to_file(new,n,"initial.dat");

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
    printf("Your Account number:%d",new.account_no);

}
int list_of_accounts(){

}
int see_individual_account(){

}
int daily_transaction(){

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
void modify_account(){

}
int close_account(){
    int temp_acc;
    printf("Enter the account number\n");
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
