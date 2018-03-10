#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define MAX_LINES 20000

const char* getfield(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
        	return tok;
    }
    return NULL;
}

int isValue(const char* name, const char* people[], int size)
{
	for(int i=0; i<size; i++)
	{
		if(strcmp(people[i],name) == 0) 
			return i;
	}
	return -1;
}

void sortPrintTweeters(const char* people[], int value[], int size)
{
	for (int i=0; i<size; i++) 
    {
	    for (int j=i+1; j<size; j++) 
	    {
			if (value[i] < value[j]) 
	        {
	            int a = value[i];
	            const char* old = people[i];

	            value[i] = value[j];
	            people[i] = people[j];

	            value[j] = a;
	            people[j] = old;
	        }
	    }
	}

	if(size>10){
		for(int i=0; i<10; i++)
		{
			printf("%s: ", people[i]);
			printf("%i\n", value[i]);
		}
	}else{
		for(int i=0; i<size; i++)
		{
			printf("%s: ", people[i]);
			printf("%i\n", value[i]);
		}
	}
	
}

int main(int argc,char* argv[])
{
	// Has less/more args
    if(argc!=2){
    	printf("Invalid Input Format\n");
    	return -1;
    }

	FILE* file = fopen(argv[1], "r");	//Open the file
	if(file == NULL){			// File not Found
		printf("Invalid Input Format\n");
		return -1;
	}

	// Success in Finding the CSV File

   	//Create a buffer to keep the line that we are reading
   	char buf[BUFFER_SIZE];

   	//Create an array to keep all th tweeters
   	const char* tweeters[MAX_LINES];

   	//Create an array to keep how many times each person tweets
   	int values[MAX_LINES];

   	//Read first line of the file
   	fgets(buf, sizeof(buf), file);
   	
   	//Create two variables to find the column number of the field name
   	int counter = 0;
   	int column_name = 0;

	//Read the whole line, separating fields by commas
	for (char *tok = strtok(buf, ",");
		tok && *tok;
		tok = strtok(NULL, ",\n"))
	{
		//Every field would be in another column
		counter++;
		
		//If we find the column name, keep the number of column in which it is
		//This name field could be inside "" o without
		if (strcmp(tok,"\"name\"") == 0 || strcmp(tok,"name") == 0)
		{
			column_name = counter;
		} 			
	}

	//If there is not a field call name  -- error
	if(column_name == 0)
	{
		printf("Invalid Input Format\n");
		return -1;
	}

	int i = 0;

	//We read one line already
	int lines = 1;

	//Read the rest of the file
	while(fgets(buf, sizeof(buf), file))
	{
		lines++;
		if(lines>MAX_LINES)
		{
			printf("Invalid Input Format\n");
			return -1;
		}

		//Keep the tweeters on an array
		const char* tweeter = getfield(buf, column_name);

		if(tweeter == NULL)
		{
			printf("Invalid Input Format\n");
			return -1;
		}

		//Check if the tweeter is already in the array
		//If it is, increment his value in the values array by 1
		int position = isValue(tweeter, tweeters, i);
		if(position != -1)
		{
			values[position]+=1;

		}else{
			//Add tweeter to the array
			tweeters[i] = strdup(tweeter);
			//Inicialize array of values
			values[i] = 1;
			i++;
		}
	}

	//Call function to order and print the top 10 tweeters
	sortPrintTweeters(tweeters, values, i); 

	//close the file
	fclose(file); 
	return 0;    			
}