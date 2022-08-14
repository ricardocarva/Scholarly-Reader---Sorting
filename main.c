/* COP 3502C Assignment 4 
This program is written by: Ricardo Carvalheira */ 

#include <stdio.h>
#include <stdlib.h>
#include "leak_detector_c.h"

FILE *inFile, *outFile;

#define PAGESCEILINGLIMIT 100000000000000

typedef struct testCase
{
	int books;
	long long pageLimit;
	int *arr;

}testArray;

void merge(int arr[], int left, int mid, int right)
{
	int i, j, k; //counters for different loops
	
	int n1=mid-left+1;//logically dividing the array in the left. This is the size of this array
	int n2=right-mid;  //logically dividing the array in the left. This is the size of this array
	
	int *L=(int *) malloc(sizeof(int)*n1); //allocates memory for a temp array to store the left size
	int *R=(int *) malloc(sizeof(int)*n2); //allocates memory for a temp array to store the right size
	
	for( i=0; i<n1;i++)
	{
		L[i]=arr[left+i];//copy data from the original array to the left array
	}
	for( j=0; j<n2;j++)
	{
		R[j]=arr[mid+1+j]; //copy data from the original array to the right array
	}
	
	i=0; //resets counter of the left side
	j=0; //resets counter of the right side
	k=left;

	while(i<n1&&j<n2)
	{//while both sides compared to their counters are not finished, keep checking if left and right are sorted
  //Depending on the scenario, we store the value from the L or from the R to the array
		if(L[i]<R[j])
		{
		  arr[k]=L[i]; 
		  i++;
		  k++;
		}
		else
		{
		  arr[k]=R[j];
		  j++;
		  k++;
		}
	}

	while(i<n1)
	{//If we got out of the loop because of the other array, we copy all the values from the left side to the array
	    arr[k]=L[i];
	    i++;
	    k++;
	}
	while(j<n2)
	{ //If we got out of the loop because of the other array, we copy all the values from the right side to the array
	    arr[k]=R[j];
	    j++;
	    k++;
	}

  //free memory for both left and right temporary arrays
	free(L);
	free(R);
}

void mergeSort(int arr[], int left, int right)
{
	if(right>left)
	{
	int mid=(left+right)/2;
	mergeSort(arr,  left, mid); //recursive call to sort array on the left side
	mergeSort(arr,  mid+1, right); //recursive call to sort array on the right side
	merge(arr, left, mid, right); //merge both sides
	}
}

void maximumBooks(int arr[], int totalOfBooks, long long limitOfPages)
{//Count the maximum of books possible to read in the test case, and print it to the screen
  long long maximumOfPages=0;//Maximum of pages read up to the moment
  int booksCounter=0;//Maximum of books read until the moment

  while((booksCounter<totalOfBooks)&&((maximumOfPages+arr[booksCounter]<=limitOfPages)))
  {
	maximumOfPages+=arr[booksCounter];//Increases the number of pages read up to the moment
	booksCounter++;//Increases the number of books read
    
   
  }
  
	printf("%d\n",booksCounter);
	fprintf(outFile,"%d\n",booksCounter);
}

void processArray(int totalBooks, long long pageLimit)
{//Alocates memory for the structure and array, stores the values, sort the array, displays maximum of books, and frees memory
	testArray* arrTest;
	arrTest=(testArray*)malloc(sizeof(testArray)); //Allocates memory for the structure
    arrTest->arr=(int*)malloc(totalBooks*sizeof(int)); //Allocates memory for the array in the structure
    arrTest->books=totalBooks; //stores the number of books from test case to structure
    arrTest->pageLimit=pageLimit; //stores the limit of pages from test case one is willing to read in the structure
    
    int i; //Index to transverse array

    for( i=0; i<totalBooks;i++)
    { //stores array values
    	fscanf(inFile,"%d", &arrTest->arr[i]); //stores how many pages each book has
      if(arrTest->arr[i]<1||arrTest->arr[i]>1000000000)
      {
        printf("Number of pages is not valid. It should be a single positive integer between 1 and 10^(9), inclusive.\n");
        fprintf(outFile, "Number of pages is not valid. It should be a single positive integer between 1 and 10^(9), inclusive.\n");
        free(arrTest->arr); //free array from the structure
        free(arrTest); //free structure
        fclose(inFile); //Closes the files
        fclose(outFile); //Closes the files+
        exit(-1);
      }
    }

    mergeSort(arrTest->arr, 0, totalBooks-1); //Merge Sort function to sort the array input
   
    maximumBooks(arrTest->arr,totalBooks,pageLimit); //Function that returns the maximum number of books possible to read in test case
    free(arrTest->arr); //free array from the structure
    free(arrTest); //free structure
}

void processCase(int numTestCases)
{
  if(numTestCases==0) //When we processes all test cases, we return
    return;
  else
  { //If there are still test cases, process it
    int totalBooksInThisTest; //totalBooks for this test case
    long long pageLimitInThisTest; // PageLimit for this test case

    fscanf(inFile, "%d %lld", &totalBooksInThisTest, &pageLimitInThisTest);
		
	if(totalBooksInThisTest<0 ||totalBooksInThisTest>100000)
	{ //Test if the number of books in this test is a positive integer less or equal than 100000
		printf("The number of total books in this test case is not valid. It should be a single positive integer less than or equal to 100000.\n");
		fprintf(outFile, "The number of total books in this test case is not valid. It should be a single positive integer less than or equal to 100000.\n");
		// fprintf(outFile, "\n");
		fclose(inFile); //Closes the files
		fclose(outFile); //Closes the files
		exit(-1);
	}

  	if(pageLimitInThisTest<0 ||pageLimitInThisTest>PAGESCEILINGLIMIT)
		{ //Test if the number of pages you are willing to read in this test is a positive integer less or equal than 10^(14)
			printf("The number of total pages in this case is not valid. It should be a single positive integer less than or equal to 10^(14).\n");
			fprintf(outFile, "The number of total pages in this case is not valid. It should be a single positive integer less than or equal to 10^(14).\n");
			fclose(inFile); //Closes the files
			fclose(outFile); //Closes the files
        	exit(-1);
		}

	processArray(totalBooksInThisTest, pageLimitInThisTest);//Process the array by sorting and getting the max books
    processCase(--numTestCases); //Recursive call to process next test case
  }
}

int main(void) 
{	
	atexit(report_mem_leak);
 	int numTestCases; //Total of test cases

 	inFile = fopen("in.txt", "r"); //opens the file name
 	
  	if (inFile== NULL)
  	{
	    printf("Input file is missing!\n");
	    exit(-1);
	    fclose(inFile); //Closes the files
  	}

 	outFile = fopen("out.txt", "w"); //opens the output file

	fscanf(inFile,"%d", &numTestCases); //Scans from the input file the first digit and stores in  numTestCases
	
	if(numTestCases<0 ||numTestCases>25) //Test if the number of test caeses to process is a positive integer less or equal than 25
	{
		printf("The number of test cases is not valid. It should be a single positive integer less than or equal to 25.\n");
		fprintf(outFile, "The number of test cases is not valid. It should be a single positive integer less than or equal to 25.\n");
		fclose(inFile); //Closes the files
		fclose(outFile); //Closes the files
		exit(-1);
	}
	
  	processCase(numTestCases); //Processes all test cases
	fclose(inFile); //Closes the files
  	fclose(outFile); //Closes the files

  return 0;
}
