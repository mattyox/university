#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define booksnumberslenght 5 //lenght of book number + '\0'
#define readersnumberslenght 5 //lenght of reader number + '\0'
#define arraysize 80000 //number of characters in arrays containing files
#define stringarraysize 320 //numbers of characters in arrays containing single string
#define bookslistformat "Book number; Title; Author's name; Author's surname\n" //how the data in bookslist.txt is written
#define readerslistformat "Reader number; Reader's name; Reader's surname\n" //how the data in readerslist.txt is written
#define booksonloanlistformat "Book number; Title; Author's name; Author's surname; Reader number; Reader's name; Reader's surname; Date of return: YYYYMMDD\n" //how the data in booksonloanlist.txt is written
#define loanduration 1209600; //number of seconds in 2 weeks

void dump_line(FILE* fp) //correct read of input data
{
	int ch;
	while ((ch = fgetc(fp)) != EOF && ch != '\n');
}

int readfile(char* filename, char* filecontent, int sizeofarray) //reads a file and loads it to an array
{
	FILE* file;
	file = fopen(filename, "rt");
	if (file == NULL)
	{
		printf("Missing %s file\n", filename);
		return 0;
	} //if file can not be read
	fgets(filecontent, sizeofarray, file); 
	fclose(file);

	return 1;
}
void printfilecontent(char* filecontent) //prints an array created by readfile, character by character
{
	while (*filecontent != '\0')
	{
		while (*filecontent != '~') putchar(*filecontent++); //prints a single book string
		printf("\n");
		*filecontent++; //moves the pointer to next book string
	}
}
void writefile(char* filename, char* filecontent) //into a file writes an array
{
	FILE* file;
	file = fopen(filename, "wt");
	fprintf(file, filecontent);
	fclose(file);
}
void getbooknumber(char* booknumber) //reads a book number and stores it in a variable
{
	printf("Enter book number: ");
	fgets(booknumber, booksnumberslenght, stdin);
	dump_line(stdin);
}
void getreadernumber(char* readernumber) //reads a reader number and stores it in a variable
{
	printf("Enter reader number: ");
	fgets(readernumber, readersnumberslenght, stdin);
	dump_line(stdin);
}
int makedecision() //asks the user if want to continue with the previously written data
{
	char decision;
	printf("Y/N\n");
	decision = getc(stdin);
	dump_line(stdin);
	
	while (decision != 'Y' && decision != 'N')
	{
		printf("Enter a correct argument\n");
		printf("Y/N\n");
		decision = getc(stdin);
		dump_line(stdin);
	}
	
	if (decision == 'N') return 0;
	
	return 1;
}

int showallbooks()
{
	char filename[] = "bookslist.txt";
	char bookslistcontent[arraysize];

	if (readfile(filename, bookslistcontent, arraysize) == 0) return 0;

	printfilecontent(bookslistcontent);

	return 0;
}
int showallreaders()
{
	char filename[] = "readerslist.txt";
	char readerslistcontent[arraysize];

	if (readfile(filename, readerslistcontent, arraysize) == 0) return 0;

	printfilecontent(readerslistcontent);

	return 0;
}
int showallbooksonloan()
{
	char filename[] = "booksonloanlist.txt";
	char booksonloanlistcontent[2*arraysize];

	if (readfile(filename, booksonloanlistcontent, 2*arraysize) == 0) return 0;

	printfilecontent(booksonloanlistcontent);

	return 0;
}
int lendabook()
{
	char bookslist[] = "bookslist.txt";
	char bookslistcontent[arraysize];
	if (readfile(bookslist, bookslistcontent, arraysize) == 0) return 0;

	char readerslist[] = "readerslist.txt";
	char readerslistcontent[arraysize];
	if (readfile(readerslist, readerslistcontent, arraysize) == 0) return 0;

	char booknumber[booksnumberslenght];
	getbooknumber(booknumber);

	char* searchedbookstringbeging;
	searchedbookstringbeging = strstr(bookslistcontent, booknumber); //begining of searched book string
	if (searchedbookstringbeging == NULL)
	{
		printf("\n");
		printf("Invalid book number");
		printf("\n");
		return 0;
	} //if book number does not exist in bookslist.txt
	char* searchedbookstringending;
	searchedbookstringending = strchr(searchedbookstringbeging, '~'); //end of searched book string
	strncpy_s(bookslistcontent, bookslistcontent, searchedbookstringending - bookslistcontent); //string to the end of searched book string
	char* searchedbookstring;
	searchedbookstring = strstr(bookslistcontent, booknumber); //string from the begining to the end of searched book string

	char readernumber[readersnumberslenght];
	getreadernumber(readernumber);

	char* searchedreaderstringbeging;
	searchedreaderstringbeging = strstr(readerslistcontent, readernumber); //begining of searched reader string
	if (searchedreaderstringbeging == NULL)
	{
		printf("\n");
		printf("Invalid reader number");
		printf("\n");
		return 0; 
	} //if reader number does not exist in readerslist.txt
	char* searchedreaderstringending;
	searchedreaderstringending = strchr(searchedreaderstringbeging, '~'); //end of searched reader string
	strncpy_s(readerslistcontent, readerslistcontent, searchedreaderstringending - readerslistcontent); //string to the end of searched book string
	char* searchedreaderstring;
	searchedreaderstring = strstr(readerslistcontent, readernumber); //string from the begining to the end of searched book string

	printf("\n");
	printf("Do you want to lend this book to this reader?\n");
	printf("\n");
	printf(bookslistformat);
	printf("%s\n", searchedbookstring);
	printf("\n");
	printf(readerslistformat);
	printf("%s\n", searchedreaderstring);
	printf("\n");

	if (makedecision() == 0) return 0;

	char bookonloan[stringarraysize];
	strcpy(bookonloan, searchedbookstring);
	strcat_s(bookonloan, "; "); //seperates data in string
	strcat_s(bookonloan, searchedreaderstring); //creates a string containing both lend book data and loaner reader data

	time_t loantime;
	time(&loantime);
	time_t returntime;
	returntime = loantime + loanduration;
	struct tm* returndate;
	returndate = localtime(&returntime); //changes time_t to struct tm* variable
	char returndatestring[arraysize];
	strftime(returndatestring, arraysize, "%Y%m%d", returndate); //creates a string containing return date in YYYYMMDD format
	strcat_s(bookonloan, "; DoR: "); //adds prefix of return date to book on loan string
	strcat_s(bookonloan, returndatestring); //adds return date to book on loan string

	char booksonloanlist[] = "booksonloanlist.txt";
	char booksonloanlistcontent[2*arraysize];
	if (readfile(booksonloanlist, booksonloanlistcontent, 2*arraysize) == 0) return 0;

	strcat_s(booksonloanlistcontent, bookonloan); //adds new book on loan string to rest books on loan
	strcat_s(booksonloanlistcontent, "~"); //adds string ending character

	writefile(booksonloanlist, booksonloanlistcontent);

	printf("\n");
	printf("Book succesfully lend\n");

	return 0;
}
int returnabook()
{
	char booksonloanlist[] = "booksonloanlist.txt";
	char booksonloanlistcontent[2*arraysize];
	if (readfile(booksonloanlist, booksonloanlistcontent, 2*arraysize) == 0) return 0;

	char booknumber[booksnumberslenght];
	getbooknumber(booknumber);

	char* searchedbooksonloantringbeging;
	searchedbooksonloantringbeging = strstr(booksonloanlistcontent, booknumber); //begining of searched book on loan string
	if (searchedbooksonloantringbeging == NULL)
	{
		printf("\n");
		printf("Book not on loan or invalid book number");
		printf("\n");
		return 0;
	} //if book number does not exist in booksonloanlist.txt
	char* searchedbooksonloantringending;
	searchedbooksonloantringending = strchr(searchedbooksonloantringbeging, '~'); //end of searched book on loan string
	char buffer[arraysize];
	strncpy_s(buffer, booksonloanlistcontent, searchedbooksonloantringending - booksonloanlistcontent); //string to the end of searched book on loan string
	char* searchedbookonloanstring;
	searchedbookonloanstring = strstr(buffer, booknumber); //string from the begining to the end of searched book on loan string

	printf("\n");
	printf("Do you want to return this book by this reader?\n");
	printf("\n");
	printf(booksonloanlistformat);
	printf("%s\n", searchedbookonloanstring);
	printf("\n");
	
	if (makedecision() == 0) return 0;

	char restbooksonloan[2*arraysize];
	strncpy_s(restbooksonloan, booksonloanlistcontent, searchedbooksonloantringbeging - booksonloanlistcontent); //creates a string containing books on loan to the begining of returned book string
	strcat_s(restbooksonloan, searchedbooksonloantringending + 1); //adds string containing rest books on loan

	writefile(booksonloanlist, restbooksonloan);

	printf("\n");
	printf("Book succesfully returned\n");

	return 0;
}
int whohasthebooktoolong()
{
	time_t currenttime;
	time(&currenttime);
	struct tm* currentdate;
	currentdate = localtime(&currenttime); //changes time_t to struct tm* variable
	char currentdatestring[9];
	strftime(currentdatestring, 9, "%Y%m%d", currentdate); //creates a string containing current date in YYYYMMDD format
	int currentdateint = atoi(currentdatestring); //changes current date string to integer

	char booksonloanlist[] = "booksonloanlist.txt";
	char booksonloanlistcontent[2 * arraysize];
	if (readfile(booksonloanlist, booksonloanlistcontent, 2 * arraysize) == 0) return 0;

	char* bookstringbeging;
	bookstringbeging = strchr(booksonloanlistcontent, '~');
	bookstringbeging++; //begining of book string
	char* bookstringending;
	bookstringending = strchr(bookstringbeging, '~'); //end of book string
	if (bookstringending == NULL)
	{
		printf("No book on loan\n");
		return 0;
	} //if there is no book outside library
	char buffer[2 * arraysize];
	strncpy_s(buffer, booksonloanlistcontent, bookstringending - booksonloanlistcontent); //string to the end of book string
	char* bookstring;
	bookstring = strchr(buffer, '~');
	bookstring++; //string from the begining to the end of book string

	char* returndatestring;
	returndatestring = strstr(bookstring, "DoR: ");
	returndatestring = returndatestring + 5; //string of return date

	int returndateint = atoi(returndatestring); //changes return date string to integer

	printf("\n");
	printf(booksonloanlistformat);

	if ((currentdateint - returndateint) > 0) printf("%s\n", bookstring); //if the current date is greater then return date prints book string

	while (bookstringbeging != NULL) //loop searching through rest books on loan and printing detained books
	{
		bookstringbeging = strchr(bookstringbeging, '~');
		bookstringbeging++;
		bookstringending = strchr(bookstringbeging, '~');
		strncpy_s(buffer, bookstringbeging, bookstringending - bookstringbeging);

		returndatestring = strstr(buffer, "DoR: ");
		returndatestring = returndatestring + 5;

		returndateint = atoi(returndatestring);

		if ((currentdateint - returndateint) > 0) printf("%s\n", buffer);
	}

	return 0;
}
int whohasthebook()
{
	char booksonloanlist[] = "booksonloanlist.txt";
	char booksonloanlistcontent[2*arraysize];
	if (readfile(booksonloanlist, booksonloanlistcontent, 2*arraysize) == 0) return 0;

	char booknumber[booksnumberslenght];
	getbooknumber(booknumber);

	char* searchedbookstringbeging;
	searchedbookstringbeging = strstr(booksonloanlistcontent, booknumber); //begining of searched book on loan string
	if (searchedbookstringbeging == NULL)
	{
		printf("\n");
		printf("Book not on loan or invalid book number");
		printf("\n");
		return 0;
	} //if book number does not exist in bookslist.txt
	char* searchedbookstringending;
	searchedbookstringending = strchr(searchedbookstringbeging, '~'); //end of searched book on loan string
	strncpy_s(booksonloanlistcontent, booksonloanlistcontent, searchedbookstringending - booksonloanlistcontent); //string to the end of searched book on loan string
	char* searchedbookstring;
	searchedbookstring = strstr(booksonloanlistcontent, booknumber); //string from the begining to the end of searched book on loan string

	printf("\n");
	printf(booksonloanlistformat);
	printf("%s\n", searchedbookstring);
	
	return 0;
}

int main()
{
	int functionnumber;

	while(1)
	{
		printf("Choose function:\n");
		printf("1. Show all books\n");
		printf("2. Show all readers\n");
		printf("3. Show all books on loan\n");
		printf("4. Lend a book\n");
		printf("5. Return a book\n");
		printf("6. Who has the book too long?\n");
		printf("7. Who has the book?\n");
		printf("\n");
		scanf_s("%d", &functionnumber); //input data is the number of choosen function
		dump_line(stdin);
		printf("\n");

		switch (functionnumber) //selects the choosen function
		{
			case 1 :
				showallbooks();
				printf("\n");
				break;
			case 2 :
				showallreaders();
				printf("\n");
				break;
			case 3 :
				showallbooksonloan();
				printf("\n");
				break;
			case 4:
				lendabook();
				printf("\n");
				break;
			case 5 :
				returnabook();
				printf("\n");
				break;
			case 6 :
				whohasthebooktoolong();
				printf("\n");
				break;
			case 7 :
				whohasthebook();
				printf("\n");
				break;
			default : //if the argument written by user is incorrect
				printf("Invalid function\n");
				printf("\n");
		}
		printf("\n");
	}
}