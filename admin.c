#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include "admin.h"
extern char *foodsTxt;
extern char *ordersTxt;
extern char *closedOrdersTxt;
extern char *takenOrdersTxt;
extern char *logTtxt;

// This method creates a new table.
void createTable(char tableID[])
{
    if(isExistTable(tableID)==false)
    {

        printf("Table %s is created successfully....\n",tableID);
        mkdir(tableID,S_IRUSR |S_IWUSR |S_IXUSR);
        strcat(tableID,"//");
        strcat(tableID,ordersTxt);
        FILE *file = fopen(tableID,"ab+");
        fclose(file);
        char text[200];
        sprintf(text,"Table %s is created successfully....\n",tableID);
        writeToLogFile(text);

    }
    else
    {
        printf("This table already exist!!!\n");
    }
}
void deleteTable(char tableID[])
{
    if(isExistTable(tableID)==true)
    {
        char temp[250];
        strcpy(temp,tableID);
        strcat(temp,"//");
        strcat(temp,ordersTxt);
        remove(temp); // orders.txt is deleted...
        rmdir(tableID); // table is deleted...
        printf("Table %s is deleted successfully....\n",tableID);
        char text[200];
		sprintf(text,"Album %s is deleted successfully....\n",tableID);
		writeToLogFile(text);
    }
    else
    {
        printf("There is no table with the given id!!!\n");
    }
}
void checkNewOrder()
{
    FILE *file = fopen(takenOrdersTxt,"rb+");
    takenOrders current;
    fread(&current,sizeof(current),1,file);
    printf("size %d:  \n",sizeof(current));
    showOrderTable(current);
    if(sizeof(current)==0)
    {
        printf("No new orders...\n");
    }
    else
    {
        printf("Confirm Order?\nYes : 1\nNo  : 0\n");
        printf("Warning: Unconfirmed orders will be canceled!!!\nWarning: Approved orders are assigned to the relevant table!!!\n");
        int selection=-1;
        scanf("%d",selection);
        if(selection==1)
        {
        char temp[100];
        strcat(temp,current.tableID);
        strcat(temp,"//");
        strcat(temp,ordersTxt);
        FILE *file = fopen(temp,"ab+");
        fprintf(file,"%s","sdfsd");
        fclose(file);
        }
    }
}

void showAllInvocies()
{
    FILE *file = fopen(closedOrdersTxt, "r");
    float current;
    printf("All payment information:\n");
    // Dosyadan float de�erleri oku ve ekrana bast�r
    while (fscanf(file, "%f", &current) == 1) {
        printf("%.2f\n", current);
    }

    fclose(file);
}

void updateFood(int foodID,float fee)
{
    if(isExistFood(foodID)==true)
    {
        FILE *file = fopen(foodsTxt, "rb+");
        food current;
        fread(&current, sizeof(current),1,file);
        bool isUpdated = false;
        while( !feof(file))
        {
            if(foodID == current.foodID)
            {
                current.foodPrice = fee;
                fseek(file, -sizeof(current), SEEK_CUR);
                fwrite(&current,sizeof(current),1,file);
                isUpdated = true;
            }
            fread(&current, sizeof(current),1,file);
        }
        fclose(file);
        if(isUpdated == true)
		{
			printf("Food is updated......\n");
			char text[200];
			sprintf(text,"Fee of the food %d is updated as %f\n",foodID,fee);
			writeToLogFile(text);
		}
    }else
    {
         printf("There is no food with the given food ID!!!\n");
    }
}


