#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<monetary.h>
#include<locale.h>
#include<ctype.h>
#include "cs402.h"
#include "my402list.h"



typedef struct My402ListStructureObject {
	char transaction_type;
	int transaction_time;
	int transaction_amount;
	char *transaction_desc;
}My402ListObject;

void trim(char *);
void My402ListPrint(My402List *);
void sort_transaction(My402List *);

static
    void Process(FILE *fp, My402List *transaction_list)
    {
	
        char buf[1026];
		char *str=NULL, *str2=NULL, *str3 = NULL, *trans_desc=NULL;
		char *a[4];
		int j = 0, count = 0;
		int amt_dollar = 0, amt_cents = 0, trans_amt= 0 , trans_time = 0;
		while(fgets(buf, sizeof(buf), fp) != NULL) 
		{
			int i = 0;
			
			if(strlen(buf)>1024)
			{
				fprintf(stderr,"file invalid > than 1024");
				exit(0);
			}
			//printf("%s",buf);
		
            /* buf may contain '\n' */
			str = strtok( buf , "\t" );
			//a[i] = str1;
			while(str!=NULL)
			{
				a[i] = str;
				i++;
				//printf("%s\n",str);
				str = strtok(NULL, "\t");
				
			}
			 
			char trans_type = a[0][0];
			char *t_time = a[1];
			//char *trans_time = malloc(25*sizeof(char));
			//strcpy(trans_time,a[1]);
			char *t_amt = a[2];
		    trans_desc = a[3];
			//char *t_desc = malloc(25*sizeof(char));
			//strcpy(t_desc,a[3]);
			
			
			if(trans_type != '+' && trans_type != '-')
			{
				fprintf(stderr,"transaction type invalid\n");
				exit(0);
			}
			else if(strlen(t_time) >= 11) //also check for digits
			{
				fprintf(stderr,"transaction timestamp is bad\n");
				exit(0);
			}
			
			trans_time = atoi(t_time);
			//printf("%d\n",trans_time);
			if(trans_time > time(NULL))
			{
				fprintf(stderr,"Timestamp not valid: ahead of current time\n");
				exit(0);
			}
			for( j = 0 , count = 0 ; t_amt[j] ; j++)
			{	
				if(t_amt[j] == '.')
				{	
					count = count+1;
				}
			}
			//printf("%d\n", count);
			if(count != 1)
			{
				fprintf(stderr,"Amount Invalid(1)\n");
				exit(0);
			}
			
			else
			{
				str2 = strtok(t_amt,".");
				/*if(!isdigit(str2))
				{
					fprintf(stderr,"Amount is invalid(2)\n");
				}*/
				
				if(strlen(str2) > 7)
				{
					fprintf(stderr,"Amount is invalid(2)\n");
					exit(0);
				}
				amt_dollar = atoi(str2);
				str3 = strtok(NULL,"\0");
				/*if(!isdigit(str3))
				{
					fprintf(stderr,"Amount is invalid(3)\n");
				}*/
				if(strlen(str3) != 2)
				{
					fprintf(stderr,"Amount is invalid(4)\n");
					exit(0);
				}
				amt_cents = atoi(str3);
				trans_amt = ((amt_dollar * 100) + amt_cents);
				//char *total_amt = (char*)((amt_dollar * 100) + amt_cents);
				//printf("%d\n", trans_amt);
				
			}		
			trim(trans_desc);
			if(strlen(trans_desc) == 0)
			{
				fprintf(stderr,"Description invalid");
				exit(0);
			}
			//printf("%s\n",t_desc);

			My402ListObject *MyListObject;
			MyListObject = (My402ListObject*)malloc(sizeof(My402ListObject));
			
			MyListObject->transaction_type = trans_type;
			//printf("%c\n",MyListObject->transaction_type);
			MyListObject->transaction_time = trans_time;
			//printf("%s\n",MyListObject->transaction_time);
			MyListObject->transaction_amount = trans_amt;
			//printf("%d\n",MyListObject->transaction_amount);
			MyListObject->transaction_desc = (char*)malloc(strlen(trans_desc)+1);
			strcpy(MyListObject->transaction_desc,trans_desc);
			//printf("%s\n",MyListObject->transaction_desc);
			
			
			My402ListAppend(transaction_list,(void*)MyListObject);
			
        }
	
		//My402ListPrint(transaction_list);
		sort_transaction(transaction_list);
		My402ListPrint(transaction_list);
	//printf("%s", a[0]);
    }
	
	
void sort_transaction(My402List *list)
{
	My402ListElem *elem1 = NULL, *elem2 = NULL, *elem3 = NULL;
	My402ListObject *temp1 = NULL, *temp2 = NULL, *obj = NULL;
	int swapped = 1;
	for(elem1 = My402ListLast(list);  elem1 != NULL && swapped; elem1 = My402ListPrev(list,elem1))
	{
		swapped = 0;
		for(elem2 = My402ListFirst(list); elem2 != NULL; elem2 = My402ListNext(list,elem2))
		{
			elem3 = My402ListNext(list,elem2);
			
			if(elem3 != NULL)
			{
						
			temp1 =(My402ListObject*)elem2->obj;
			temp2 = (My402ListObject*)elem3->obj;
			
			//printf("%d\n", temp1->transaction_time);
			//printf("%d\n", temp2->transaction_time);
			if(temp1->transaction_time == temp2->transaction_time)
			{
				fprintf(stderr,"Duplicate timestamp\n");
				exit(0);
			}
			if(temp1->transaction_time > temp2->transaction_time)
			{
				///printf("Swapping\n");
				swapped = 1;
				obj = elem2->obj;
				elem2->obj = elem3->obj;
				elem3->obj = obj;
			}
			//getchar();
			}
		}
	}
}

/*void sort_transaction(My402List *list)
{
	My402ListElem *elem1 = NULL, *elem2 = NULL, *elem3 = NULL;
	My402ListObject *temp = NULL , *temp1 = NULL, *temp2 = NULL, *obj = NULL;
	
	for(elem1 = My402ListFirst(list);  elem1 != NULL; elem1 = My402ListNext(list,elem1))
	{
		for(elem2 = My402ListFirst(list); elem2 != My402ListPrev(list,My402ListLast(list)); elem2 = My402ListNext(list,elem2))
		{
			elem3 = My402ListNext(list,elem2);
			
			//obj = (My402ListObject*)elem3->obj;			
			temp1 =(My402ListObject*)elem2->obj;
			temp2 = (My402ListObject*)elem3->obj;
			
			if(temp1->transaction_time > temp2->transaction_time)
			{
				My402ListInsertBefore(list,temp2,elem2);
				My402ListUnlink(list,elem3);
			}			
		}	
	}
}	*/
	
void My402ListPrint(My402List *list)
{
	My402ListElem *elem=NULL;
	My402ListObject *temp = NULL;
	char Description[24];
	char strpadding[23] = "                       ";
	char *Balance_str = NULL, *Amount_str = NULL;
	 
	double Balance_dollar = 0;
	int Amount = 0;
	
	setlocale(LC_ALL , "en_CA.UTF-8");
	fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");
	fprintf(stdout,"|       Date      | Description              |         Amount |        Balance |\n");
	fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");

    for (elem=My402ListFirst(list); elem != NULL; elem=My402ListNext(list, elem)) {
        temp = (My402ListObject*)elem->obj;
		//temp->transaction_type;
		time_t sec = temp->transaction_time;
		//printf("%d",sec);
		struct tm *ptm;
		ptm=localtime(&sec);
		char time_formatted[20];
		
		//printf("%d\n",time(NULL));
		Amount = temp->transaction_amount;
		//printf("%d\n",Amount);
		//printf("%d\n",temp->transaction_amount);
		double Amount_dollar = (double)Amount/100;
		//printf("%f\n",Amount_dollar);
		//Amount_cents = Amount%100;
		
		strftime(time_formatted, 17, " %a %b %e %Y ", ptm);
		
		if(strlen(temp->transaction_desc) > 24)
		{
			strncpy(Description, temp->transaction_desc , 24);
	
		}
		
		else if(strlen(temp->transaction_desc) < 24)
		{	
		
			strncpy(Description,temp->transaction_desc,24);
			strncat(Description, strpadding , 24-strlen(Description));
			
			
		}
		Description[strlen(Description)] = '\0';
		//Description = string_pad(temp->transaction_desc);
		char type = temp->transaction_type;
		
		//Description = strpad( temp->transaction_desc, ' ' , 23);
		
		if(Amount > 9999999)
		{
			printf("print ???"); ;//???????.??;
		}
		
	
			//printf("%s\n", Balance_str);
		if(type == '+')
		{
			
			Balance_dollar = Balance_dollar + Amount_dollar;
			
			Amount_str = malloc(25);
			Balance_str = malloc(25);
			
			strfmon(Amount_str , 25 , "%(!14#7.2n", Amount_dollar);
			strfmon(Balance_str , 25 , "%(!14#7.2n", Balance_dollar);
			
			//Balance_cents = Balance%100;
			
			fprintf(stdout,"|%s | %s | %s | %s |\n",time_formatted,Description,Amount_str,Balance_str);
			
		}
		else if(type == '-')
		{
			Amount_dollar = -(Amount_dollar);
			Balance_dollar = Balance_dollar + Amount_dollar;
			//double Balance_dollar = (double)Balance/100;
			//Balance_cents = Balance%100;
			Amount_str = malloc(25);
			Balance_str = malloc(25);
			
			strfmon(Amount_str , 25 , "%(!14#7.2n", Amount_dollar);
			strfmon(Balance_str , 25 , "%(!14#7.2n", Balance_dollar);
			
			fprintf(stdout,"|%s | %s | %s | %s |\n",time_formatted,Description,Amount_str,Balance_str);
			
		}
		
    }
	fprintf(stdout,"+-----------------+--------------------------+----------------+----------------+\n");
    fprintf(stdout, "\n");
}

 
 /* i did not write this code. Derieved from 
 http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way */
 
void trim(char *str)
{
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace(str[begin]))
        begin++;

    while (isspace(str[end]) && (end >= begin))
        end--;

    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0'; // Null terminate string.
}
 
 
 
int main(int argc, char *argv[])
{
	My402List mytransaction_list;
	My402ListInit(&mytransaction_list);
	int reading_from_file = 0;
	if (argc == 3 && !strcmp(argv[1],"sort")) 
	{
		reading_from_file = 1;
	}	
		FILE *fp = NULL;
 
		if (reading_from_file == 1) 
		{
			fp = fopen(argv[2], "r");
			if (fp == NULL) 
			{
				fprintf(stderr,"Cannot open for reading.%s \n",argv[2]);
				return 0;
			}
			
			Process(fp,&mytransaction_list);
			fclose(fp);
        } 
	   else 
	   {
			if(argc == 2 &&  !strcmp(argv[1],"sort"))
			{
				Process(stdin,&mytransaction_list);
			}
			else 
			{
				fprintf(stderr,"Invalid Command. Usage: ./warmup1 sort [filename]");
				exit(0);
			}
       }   
	return 0;
}	







