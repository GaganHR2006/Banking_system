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
    add_to_file(new,n,"initial.dat");
}
void add_to_file(customer new,int n, const char *filename){
    FILE *fp = fopen( filename, "wb");
    if (fp == NULL) {
        printf("Try again\n");
        return;
    }
    size_t written_count = fwrite(&new, sizeof(new), n, fp);
    if (written_count == n) {
        printf("Successfully created \n");
    }
    fclose(fp);

}
int list_of_accounts(){

}
int see_individual_account(){

}
int daily_transaction(){

}
int edit_account(){

}