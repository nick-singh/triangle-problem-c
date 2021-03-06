/*******************************************************************

	Name	: Nicholas Chamansingh	


	Problem : Triangle Problem...
	
	Given a triangular matrix, with the number of rows calculate 
	the best sum that can be obtained from the values in the 
	triangle.


	Methods Implemented:
	Brute Force
	Recursion
	Memorized Recursion
	Dynamic Programming


********************************************************************/

#include <stdlib.h>
#include <stdio.h>

/*******************************************************************
	
	Initialization function

*******************************************************************/
int **initTrinagle(FILE*, int);
int **initMemTriangle(int,int);
void initBinArray(int [],int);




/*******************************************************************
	
	Utility Functions

*******************************************************************/
int pow2(int,int);
void numToBinary(int [], int, int);
void printPath(FILE*,int**, int[], int, int);
void printMem(FILE*,int **, int);





/*******************************************************************
	
	Functions which compute the best sum given any triangle

*******************************************************************/
int bruteForceTriangle(int **,int,int[]);
int recursiveTriangle(int **, int, int, int);
int recursiveMemTriangle(int **, int**, int, int, int);
int dpTriangle(int **, int**, int);


int main()// start main
{

	//reads input and stores in a FILE pointer in
	FILE*in = fopen("input.txt","r");
	FILE*out = fopen("output.txt","w");
	if(in == NULL) printf("error reaind file\n"); // if there is an error reading the file
	
	// variables used to store solutions from differnt methods
	int n = 0, bruteForceSol, recursionSol, memRecursionSol, dpSol;

	

	fscanf(in,"%d",&n); // reads the first line containing the number of rows
	int bruteForcePath[n-1],recursionPath[n-1]; // arrarys which will be used to store the optimual path
	int **arr = initTrinagle(in,n); // stores numbers from file into triangle
	int **memArray = initMemTriangle(n,-1); // initializes the array to be used for the memorized recursion
	int **memArrayDp = initMemTriangle(n,0); // initializes the array to be used for the memorized dp


	// Function to compute the best sum using the brute force method
	bruteForceSol = bruteForceTriangle(arr,n-1,bruteForcePath);	
	fprintf(out,"brute froce method %d\n", bruteForceSol);
	// printPath(arr,bruteForcePath,bruteForceSol,n-1);


	// Function to compute the best sum using simple recursion
	recursionSol = recursiveTriangle(arr,n-1,0,0);
	fprintf(out,"using recursion %d\n", recursionSol);

	// Function to compute the best sum using memorized recursion
	memRecursionSol = recursiveMemTriangle(arr, memArray, n-1, 0,0);
	fprintf(out,"Using memoized recursion best sum = %d, with path: \n", memRecursionSol);
	printMem(out,memArray, n-1);

	// Function to compute the best sum using dynamic programming
	dpSol =	dpTriangle(arr,memArrayDp, n-1);
	fprintf(out,"Using dynamic programming best sum = %d, with path: \n", dpSol);
	printMem(out, arr, n-1);

	fclose(in);// close in
	fclose(out);// close out
	return 0;
}// end main




int bruteForceTriangle(int **triangle, int num, int path[]){
	int i = 0,j;
	int n = pow2(2,num); // if there are n rows then the number of paths are 2^n-1
	int m = -999, sum = triangle[0][0], array[num];
	
	while(i < n){ // while we have not visited all paths

		initBinArray(array, num); // initialize binary array all values are set to 0
		numToBinary(array,num,i); // converts the current number to its binary equ
		int a = 0, b = 0; // starting points 

		for (j = 1; j <=num; j++) // for each path in the binary arrar 
		{						// 1 left, 1 right
			if (array[j] == 0) // go left
			{
				a++;								
				sum+=triangle[a][b];					
			}else{ // go right
				a++;
				b++;				
				sum+=triangle[a][b];					
			}			
		}
		if(sum> m) { // stroes the max sum in m
			m = sum;			
			path = array;			
		}
		sum = triangle[0][0]; // re initialize the sum to the first value in the trinagle
		i++;
	}
	return m;
}// end brute force


int recursiveTriangle(int **triangle, int n, int a, int b){
	if(a == n)return triangle[a][b]; //if we are at the bottom of the triangle
	int left = recursiveTriangle(triangle, n, a+1, b); // getting value for left
	int right = recursiveTriangle(triangle, n, a+1, b+1); // getting value for right
	if(left>right){
		// fprintf(out,"%d\n", 0);
		return triangle[a][b]+left; // returns the sum of the current position in the triangle and left	
	}else{
		// fprintf(out,"%d\n", 0);
		return triangle[a][b]+right; // returns the sum of the current position in the triangle and right	
	} 
	
} //end recursion


int recursiveMemTriangle(int **triangle, int **memArray, int n, int a, int b){
	if(a == n)return memArray[a][b]  = triangle[a][b]; // we are at the bottom of the triangle
	if(memArray[a][b] != -1)return memArray[a][b]; // if we have already visited this position
	int left = recursiveMemTriangle(triangle,memArray,n, a+1, b); // getting value for left
	int right = recursiveMemTriangle(triangle,memArray, n, a+1, b+1); // getting value for right
	if(left>right) return memArray[a][b] = triangle[a][b]+left;	// returns the sum of the current position in the triangle and left
	return memArray[a][b] = triangle[a][b]+right; // returns the sum of the current position in the triangle and right
}


int dpTriangle(int **triangle, int **mem, int num){
	int i, j;
	int left, right;
	for(i=num; i>=0;i--){ //starting at the bottom of the triangle
		for(j=i;j>=0;j--){ // starting from the right
			if(i<num){ // if we are at the second to last row and above
				left = triangle[i+1][j] + triangle[i][j]; // get left
				right = triangle[i+1][j+1] + triangle[i][j]; // get right
				if (left>right) // get larger between left and right
				{
					triangle[i][j] = left; 
				}else{
					triangle[i][j] = right;
				}				
			}				
		}
    }

	return triangle[0][0]; // return the top of the triangle, which now holds the best sume...
}


int **initTrinagle(FILE *in, int num){
	int i, j;
	int ** arr = malloc(num * sizeof (int*));

	for (i = 0; i < num; i++)
	{
		arr[i] = malloc(num * sizeof (int*));
		for (j = 0; j <= i; j++)
		{			
			fscanf(in,"%d",&arr[i][j]);						
		}
	}

	return arr;
}


int **initMemTriangle(int num, int init){
	int i, j;
	int ** arr = malloc(num * sizeof (int*));

	for (i = 0; i < num; i++)
	{
		arr[i] = malloc(num * sizeof (int*));
		for (j = 0; j <= i; j++)
		{			
			arr[i][j] = init;						
		}
	}

	return arr;
}


int pow2(int x, int n){
	int y = 0;
	if (n == 0)return 1;
	y = pow2(x, n/2);
	y = y * y;
	if(n%2!=0) return x * y;
	return y;
}

void initBinArray(int arr[],int size){
	int i;
	for (i = size; i > 0; i--)
	{
		arr[i] = 0;		
	}
}


void numToBinary(int arr[], int size,int num){
	int i = size;		
	while(num > 0){
			
		arr[i] = num%2;			
		i--;
		num/=2;
	}	
}



void printPath(FILE *out,int **tri, int path[], int sol, int size){
	int i = 0,j, a=0, b=0;
	
	for(j = 0; j < size; j++){
		if (path[j] == 0)
		{
			fprintf(out,"Left ");
		}else{
			fprintf(out,"Right ");
		}
	}	
	fprintf(out,"\n");
}


void printMem(FILE *out, int **mem, int size){
	int a = 0, i = 0, j = 0;	
	while(a<size){
		if(mem[i+1][j] > mem[i+1][j+1]){
			if(a<size-1){
				fprintf(out,"Left ->");
			}else{
				fprintf(out,"Left\n");
			}
			i++;
		}else{
			if(a<size-1){
				fprintf(out,"Right ->");
			}else{
				fprintf(out,"Right\n");
			}
			j++;
			i++;
		}
		a++;
	}
}

