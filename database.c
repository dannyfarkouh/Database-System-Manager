#include <stdio.h>
#include "database.h"
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <time.h>

//Student Name: Danny Farkouh
//Student ID: 261176369


Database db_create(){
// Create a database with initial size 0 and initial capacity 4

	Database db;
	db.size = 0;
	db.capacity = 4;
	db.records = (Record *)malloc(db.capacity * sizeof(Record));

	return db;

}

void db_append(Database * db, Record const * item){
// Copies the record pointed to by item to the end of the database. 
// This is where you will have to implement the resizing logic described above. 

	//If the size is smaller than the capacity
	if (db->size < db->capacity){
		
		db->records[db->size] = *item;
		db->size++;
		
		}
	
	//If the size has reached the same value as the capacity
	else if (db->size >= db->capacity){
		
		int new_capacity = db->capacity * 2;
		Record *new_records = (Record *)realloc(db->records, new_capacity * sizeof(Record));
		
		
		db->records = new_records;
		db->capacity = new_capacity;

		db->records[db->size] = *item;
		db->size++;
		
		}	
		
	
	
}


Record * db_index(Database * db, int index){
// Returns a pointer to the item in the database at the given index 
// You need to check the bounds of the index
	
	//If out of bounds  
	if (index >= db->size || index < 0){

		printf("Error! The index is out of bounds.\n");
		exit (3);
	
	}
	
	else{
		return &(db->records[index]);

		
	
	}

}


Record * db_lookup(Database * db, char const * handle){
// Returns a pointer to the first item in the database whose handle 
// field equals the given value 
	

	//Let us loop over the handles in array records 
	for (int i = 0; i < db->size; i++){
		if (strcmp(db->records[i].handle, handle) == 0 ){
			return &(db->records[i]);
		

		}

	}	

		return NULL; 
}
	
void db_free(Database * db){
// Releases the memory held by the underlying array 
// After calling this, the databse can no longer be used 

	free(db->records);
	db->size = 0;

}



Record parse_record(char * line){
// ^ parses a single line of CSV data into one Record 
	
	//Create record ri
	Record ri;

	//Let us create the pointer that will check through the four values in the line 	
	//This is the pointer to the first value in the line which is the handle
	char *value = strtok(line, ",");
	//string has to be copied into ri.handle
	strncpy(ri.handle, value, sizeof(ri.handle));
	//Ensure that the array ends with \0 since that is how strings end in C
	ri.handle[sizeof(ri.handle) - 1] = '\0';


	//second value of the line is follower count which is an int 
	value = strtok(NULL, ",");	
	//We do not need an endptr so NULL 
	//integers do not need to be copied we can just equal the value 
	ri.follower_count = strtol(value, NULL, 10);
	

	//third value of the line is comment which is a string 
	value = strtok(NULL, ",");
	strncpy(ri.comment, value, sizeof(ri.comment) - 1);
	ri.comment[sizeof(ri.comment) - 1] = '\0';

	//fourth value of the line is the date last modified which is an int
	value = strtok(NULL, ",");
	ri.date_last_modified = strtol(value, NULL, 10);

	return ri;
}


void db_load_csv(Database * db, char const * path){
// ^ Appends the records read from the file at 'path'
//  int the already initialized database 'db'
	
	//Opening the file 
	FILE *file = fopen(path, "r");
	
	//These are the args for getline
	char *line = NULL;
	size_t size;
	size_t nread;
	
	//While the end of the file hasn't been reached...
	while((nread = getline(&line, &size, file)) != -1){
		Record ri = parse_record(line);
		db_append(db, &ri);
	
	}
	
	free(line);
	fclose(file);
	
		
}


void db_write_csv(Database * db, char const * path){
//^ Overwrites the file located at 'path' with the 
//contents of the database, represented in CSV format.
	
	//Opening the file 
	FILE *file = fopen(path, "w");

	//Go through all records in the database 
	for(int i = 0; i < db->size; i++){
		
	//Write into the file in path 
	fprintf(file, "%s,%lu,%s,%lu\n" 
			, db->records[i].handle, db->records[i].follower_count 
			, db->records[i].comment, db->records[i].date_last_modified);	
		
	}
	
	fclose(file);
	
}

int is_valid_handle(char * handle){
//This function will validate if the handle given by the user 
//is a valid handle or not 

	int count = 1;

	//Let's check that the handle has less than 32 character
	//more than one character (not just @)
	//and that the handle starts with a '@'
	if(strlen(handle) <= 32 && strlen(handle) > 1 && handle[0] == '@'){
		
		//Create a for loop to go over each letter and 
		//check if there are bad characters
		for(int i = 0; count != strlen(handle); i++){
		
			//If comma, return 1
			if(handle[i] == ','){
				
				return 1;
			
			//If '\0' or '\n' return 1
			}else if(handle[i] == '\\'){
			
				if(handle[i+1] == 'n' || handle[i+1] == '0'){
				
					return 1;
				
				}
			}
			count++;
		}
		
		//If all is fine, return 0, the handle is valid
		return 0;

	//If handle is too long, too short or doesn't start with '@', return 1
	}else{
	
		return 1;
	
	}
}
	
int is_valid_comment(char * comment){
//This function will validate if the comment given by the user 
//is a valid comment or not 

	int count = 1;

	//Let's check that the comment has less than 32 characters
	//(I decided on 32 since anyway the comment is going to get truncated if too long)
	//Let's also check that there are no commas or weird characters 
	if(strlen(comment) <= 32){
		
		//Create a for loop to go over each letter and 
		//check if there are bad characters
		for(int i = 0; count != strlen(comment); i++){
		
			//If comma, return 1
			if(comment[i] == ','){
				
				return 1;
			
			//If '\0' or '\n' return 1
			}else if(comment[i] == '\\'){
			
				if(comment[i+1] == 'n' || comment[i+1] == '0'){
				
					return 1;
				
				}
			}
			count++;
		}
		
		//If all is fine, return 0, the comment is valid
		return 0;

	//If comment is too long, too short or doesn't start with '@', return 1
	}else{
			
		return 1;
	
	}

}

