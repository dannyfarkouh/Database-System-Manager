#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "database.h"

//Student Name: Danny Farkouh
//Student ID: 261176369

int main_loop(Database * db){
	
	//If program was saved or not 
	int saved = 0;

	//Initializing the values to use getline
	char *line = NULL;
	size_t size;
	size_t nread;
	//Initializing the values to use strtok
	char *word1;
	char *word2;
	char *word3;
	char *word4;

	printf("Loaded %d records.\n", db->size);

	//While not end of file (ctrl+D) continue the loop
	while(!feof(stdin)){
		
		printf("> ");

		//getline to get the user input 
		nread = getline(&line, &size, stdin); // Now we have the line of user input
		//Replace the last value \n by \0  
		line[nread - 1] = '\0';
		
		//This is the value of the first user input, separated by whitespace 
		word1 = strtok(line, " ");

		

		//If the first value is just a space 
		if(word1 == NULL){
		
			printf("Error! You have not entered a command.\n");
			printf("You must enter one of the following:\n");
			printf("<list>, <add>, <update>, <save>, <exit>.\n");
		
		}
		
		//If the first value is <list> 
		else if(strcmp(word1, "list") == 0){
			
			//Value of the second word input 
			word2 = strtok(NULL, " ");
			//If <list> has an argumenet
			if(word2 != NULL){
				printf("Error! <list> cannot have any extra arguments.\n");
			
			}
			//If <list> is correctly called
			else if (!feof(stdin)){
				
				//ALL WORK FOR LIST IS DONE HERE
				printf("HANDLE              | FOLLOWERS | LAST MODIFIED    | COMMENT\n");
				printf("--------------------|-----------|------------------|--------------------------\n");
			
				for(int i = 0; i < db->size; i++){
					
					char human_readable[24];
					strftime(human_readable, sizeof(human_readable), "%Y-%m-%d %H:%M", localtime(&db->records[i].date_last_modified));
				
					printf("%-20.20s| %-10lu| %-17s| %-25.25s\n"
							,db->records[i].handle, db->records[i].follower_count
							,human_readable, db->records[i].comment);
				
				}
			} 
		}	

		//If the first value is <add>
		else if(strcmp(word1, "add") == 0){
			
			//Value of the second word input 
			word2 = strtok(NULL, " ");
			//If <add> doesn't have any extra arguments
			if(word2 == NULL && !feof(stdin)){

				printf("Error! <add> must have arguments such that:\n");
				printf("<add> <HANDLE> <FOLLOWERS>\n");
				
			
			//If the <add> has extra arguments
			}else if(word2 != NULL){
				
				//Value of the third word input
				word3 = strtok(NULL, " ");
				//If <add> doesn't have a third argument
				if(word3 == NULL){

					printf("Error! <add> must have arguments such that:\n");
					printf("<add> <HANDLE> <FOLLOWERS>\n");


				//If <add> is called correctly 
				}else{
				
					word4 = strtok(NULL, " ");
					//Value of the fourth argument
					if(word4 != NULL){
						
						printf("Error! <add> must have exactly two extra arguments such that:\n");
						printf("<add> <HANDLE> <FOLLOWERS>\n");

					
					
					}else{
					
						Record * is_handle = db_lookup(db, word2);
						
						//If the handle is not in the database, continue 
						if(is_handle == NULL){
							
							int is_valid = is_valid_handle(word2);
							
							if(is_valid == 0){
								
								//Max 9 digits (No instagram account has a billion followers)
								if(strlen(word3) > 9){
								
									printf("Error! <FOLLOWERS> is too high.\n");
								
								//If the number of followers is respected 
								}else{
										
									char * last_char;

									//Start with transforming the string of the followers to an integer 
									long int followers = strtol(word3, &last_char, 10);

									//If the argument is not all integers 
									if(*last_char != '\0'){
									
										printf("Error! <FOLLOWERS> must be all integers.\n");
								
									//If the FOLLOWERS is all integers
									}else{
										
										//Get the user input of the comment
										printf("Comment> ");
										
										//Initialize the variables to get the comment line 
										
										char *comment_line = NULL;
										size_t size_line;
										size_t ncomment;


										ncomment = getline(&comment_line, &size_line, stdin); // Now we have the line of user input
											
										int comment_valid = is_valid_comment(comment_line);
										
										comment_line[strlen(comment_line) - 1] = '\0';	
									
										if(comment_valid == 0){
											
											//Continue and add to record 

											Record rec;
											
											rec.follower_count = followers;
											rec.date_last_modified = time(NULL);
											strcpy(rec.handle, word2);
											strcpy(rec.comment, comment_line);

											db_append(db, &rec);	

											//Unsave 
											saved = 1;

											
										
										}else{
										
											printf("The comment is invalid\n");
											printf("Comment must respect the following:\n");	
											printf("Comment must not contain <,> <\\n> <\\0>\n");
											printf("Comment must also not be longer than 32 characters.\n");
										}
												

									}
								}
								
									
								
								
							}else{
							
								printf("The handle '%s' is invalid.\n", word2);
								printf("Handle must respect the following:\n");
								printf("Handle must start with '@'.\n");
								printf("Handle must not contain <,> <\\n> <\\0>\n");
								printf("Handle must not be longer than 32 characters.\n");
							
							}
							
						//If the handle is in the database, tell user that it is already there 
						}else{
						
							printf("Error! Handle '%s' already exists in the database.\n", word2);
						
						
						}	
						
						
						
					}
				}
			
			
			}			
		}

		
		//If the first value is <update>
		else if(strcmp(word1, "update") == 0){

			//Value of the second word input 
			word2 = strtok(NULL, " ");
			//If <update> doesn't have any extra arguments
			if(word2 == NULL && !feof(stdin)){
				
				printf("Error! <update> must have arguments such that:\n");
				printf("<update> <HANDLE> <FOLLOWERS>\n");
			
			//If the <update> has extra arguments
			}else if(word2 != NULL){
				
				//Value of the third word 
				word3 = strtok(NULL, " ");
				//If <update> doesn't have a third argument
				if(word3 == NULL){
				
					printf("Error! <update> must have arguments such that:\n");
					printf("<update> <HANDLE> <FOLLOWERS>\n");
				
				}else{
				
					word4 = strtok(NULL, " ");
					//Value of the fourth word 
					if(word4 != NULL){
						
						printf("Error! <update> must have exactly two extra arguments such that:\n");
						printf("<update> <HANDLE> <FOLLOWERS>\n");
					
					
					}else{
						
						Record * is_handle = db_lookup(db, word2);

						//If the handle is in database, continue 
						if(is_handle != NULL){
							
							//Check that the number of followers is less than 9 digits
							if(strlen(word3) > 9){
								
								printf("Error! <FOLLOWERS> is too high.\n");
							
							//If the number of followers is respected
							}else{
							
								char * last_char;

								//Start with transforming the string of the followers to an integer
								long int followers = strtol(word3, &last_char, 10);

								//If the argument is not all integers
								if(*last_char != '\0'){
								
									printf("Error! <FOLLOWERS> must be all integers.\n");
								
								//IF the followers is all integers
								}else{
								
									//Get the user input of the comment
									printf("Comment> ");

									char *comment_line = NULL;
									size_t size_line;
									size_t ncomment;
							
									ncomment = getline(&comment_line, &size_line, stdin);
									
									int comment_valid = is_valid_comment(comment_line);

									comment_line[strlen(comment_line) - 1] = '\0';	
									
									if(comment_valid == 0){
									
										//First call db_index to find the account that needs to be replaced
										
										is_handle->follower_count = followers;
										strcpy(is_handle->comment, comment_line);	
										is_handle->date_last_modified = time(NULL);

										//Unsave 
										saved = 1;
									
									//If the comment is invalid 
									}else{
									
										
										printf("The comment is invalid\n");
										printf("Comment must respect the following:\n");	
										printf("Comment must not contain <,> <\\n> <\\0>\n");
										printf("Comment must also not be longer than 32 characters.\n");
									
									}
								
								}
							
							}
								
						}else{
						
							printf("Error! Handle '%s' does not exist in the database.\n", word2);
						
						}
					}
				
				}
			
			
			}
		
		
		
		}
		
		//If the first value is <save>
		else if(strcmp(word1, "save") == 0){
			
			//Value of the second word input
			word2 = strtok(NULL, " ");
			//If <save> has an argument
			if(word2 != NULL){
				printf("Error! <save> cannot have any extra arguments.\n");
			
			}

			//If <save> is correctly called
			else{
				
				//Save this database to the file
				db_write_csv(db, "database.csv");
				//Save
				saved = 0;
				printf("Program has been saved.\n");
				
			}
		
		}

		//If the first value is <exit>
		else if(strcmp(word1, "exit") == 0){
			
			word2 = strtok(NULL, " ");

			//If <exit> has no arguments 
			if(word2 == NULL){
				
				//Check if the program has been saved
				if(saved == 0){
					
					printf("Wrote %d records.\n", db->size);
					printf("Exiting the program...\n");
					exit (0);
				
				//If hasn't been saved
				}else{
				
					printf("Error! You did not save your changes. Use 'exit fr' to force exiting anyway.\n");
				
				}

			//If <exit> has argument <fr> 
			}else if(strcmp(word2, "fr") == 0){
				
				printf("Exiting unsaved program...\n");
				exit (0);

			//If <exit> has any other arguments
			}else{
			
				printf("Exit must not have arguments.\n");
			
			}
			
		//If the first argument is anything else 
		}else if(!feof(stdin)){

			printf("Error! Invalid command.\n");
			printf("Command must be one out of the following:\n");
			printf("<list>, <add>, <update>, <save>, <exit>.\n");
			
		}
	} 
}


int main()
{
	Database db = db_create();
	db_load_csv(&db, "database.csv");
	return main_loop(&db);
	db_free(&db);
}
