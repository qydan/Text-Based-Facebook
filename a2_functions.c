/*****************
    Student Name 	= Aydan Eng
    Student Number	= 101298520
*/

#include <stdlib.h>
#include <ctype.h> //tolower function
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include "a2_nodes.h"
#include "a2_functions.h"

// Your solution goes here



/*
   ******** DONT MODIFY THIS FUNCTION ********
   Function that reads users from the text file.
   IMPORTANT: This function shouldn't be modified and used as is
   ******** DONT MODIFY THIS FUNCTION ********
*/

user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        fgets(buffer, sizeof(buffer), file);
        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        char *token2 = strtok(NULL, ",");
        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        // token = strtok(NULL, ",");
        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}


/*
Function to add user to program, inserts user in linked list in ascending order (lexicographically)
*/
user_t *add_user(user_t *users, const char *username, const char *password){
    assert(username != NULL);
    assert(password != NULL);

    //Dedicate memory to new user
    user_t *new_user = malloc(sizeof(user_t));
    assert(new_user != NULL);

    //Copy given username and password to newly created user struct
    strcpy(new_user->username, username);
    assert(new_user->username != NULL);

    strcpy(new_user->password, password);
    assert(new_user->password != NULL);

    //Initalize user with no posts and friends
    new_user->posts = NULL;
    new_user->friends = NULL;

    //Insert into the linked list in ascending order
    if (users == NULL || strcmp(username, users->username) < 0) {
        //Insert at the beginning
        new_user->next = users;
        return new_user;
    }

    //Traverse list until finds correct spot to be inserted in linked list (alphabetically)
    user_t *current = users;
    while (current->next != NULL && strcmp(username, current->next->username) > 0) { //if username comes before the current username keep moving through linked list
        current = current->next;
    }

    //Insert after the current node
    new_user->next = current->next;
    current->next = new_user;

    //Print successful addition of user to linked list and return linked list of users
    return users;
}


/*
Function to iterate through a linked list of users and find the user with the matching given username.
*/
user_t *find_user(user_t *users, const char *username){
    user_t *current = users;

    //Iterate through linked list
    while(current != NULL){

        //Compare username of users in linked list to entered username, return node where user is found
        if(strcmp(current->username, username) == 0){
            return current;
        }
        current = current->next;
    }
    //Return NULL if user not found
    return NULL;
}


/*
Function to iterate through a linked list of friends and find the friend with the matching given username.
*/
friend_t *find_friend(user_t *users, const char *username){
    friend_t *current = users->friends;

    if (current == NULL) {
        printf("No friends in the list to delete.\n");
        return NULL;
    }

    //iterate through linked list
    while(current != NULL){
        //compare username of users in linked list to entered username
        if(strcmp(current->username, username) == 0){
            //return user with matching username if found
            return current;
        }
        current = current->next;
    }
    printf("\n-----------------------------------------------\nFriend not found.\n-----------------------------------------------\n\n");
    //return NULL if not found
    return NULL;
}


/*
Function to print main menu wlecome message.
*/
void print_menu(){
    printf("\n***********************************************\nMAIN MENU:\n*********************************************** \n");
    printf("1. Register a new user\n");
    printf("2. Manage a user's profile (change password)\n");
    printf("3. Manage a user's post (add/remove)\n");
    printf("4. Manage a user's friends (add/remove)\n");
    printf("5. Display a user's post\n");
    printf("6. Exit\n\n");
    printf("Enter your choice: ");
}


/*
Function to print manage post menu.
*/
void print_post_menu(user_t *user){
    printf("\n-----------------------------------------------\nManaging %s's posts\n-----------------------------------------------\n", user->username);
    if (user->posts == NULL){
        printf("Note: No posts available for %s\n", user->username);
    }
    printf("1. Add new post\n");
    printf("2. Remove a post\n");
    printf("3. Return to main menu\n");
    printf("Enter your choice: ");
}


/*
Function to print the manage friends menu.
*/
void print_friend_menu(user_t *user){
    printf("\n-----------------------------------------------\nManaging %s's friends\n-----------------------------------------------\n", user->username);
    printf("1. Add new friend\n");
    printf("2. Remove a friend\n");
    printf("3. Return to main menu\n");
    printf("Enter your choice: ");
}


/*
Function to receive unsigned short integer input from user.
*/
unsigned short int get_input_usi(void){
	unsigned short int user_input;
	do{
		scanf("%hu", &user_input);
		if (user_input < 0){
			printf("Invalid Input. Please enter a valid positive integer: ");
		} 
	}while(user_input < 0);
	return user_input;
}


/*
Function to receive unsigned short integer input from user.
*/
char get_input_char(void){
	char user_input;
	do{
		scanf(" %c", &user_input);
        tolower(user_input);
		if ((user_input !=  'y') && (user_input !=  'n')){
			printf("Invalid Input. Please enter a valid choice (y or n): ");
		} 
	}while((user_input !=  'y') && (user_input !=  'n'));
	return user_input;
}


/*
Function to add post for a user
*/
void add_post(user_t *user, const char *text){
    assert(user != NULL);
    assert(text != NULL);

    //create a new post using create_post
    post_t *new_post = create_post(text);

    //add the new post to the top of the stack (at the head of the list)
    new_post->next = user->posts;
    user->posts = new_post;
}


/*
Function to add friend for user, inserts friend in linked list of friends in ascending order (lexicographically)
*/
void add_friend(user_t *user, const char *friend){
    assert(user != NULL);
    assert(friend != NULL);

    //Create a new friend node
    friend_t *new_friend = create_friend(friend);
    assert(new_friend != NULL);

    new_friend->posts = NULL;

    //If the friends list is empty, add the new friend as the head
    if (user->friends == NULL){
        user->friends = new_friend;
        return;
    }

    //Traverse the list to find the correct position (ascending order)
    friend_t *current = user->friends;
    friend_t *previous = NULL;

    while(current != NULL && strcmp(current->username, new_friend->username) < 0){
        previous = current;
        current = current->next;
    }

    //Insert the new friend into the list
    if(previous == NULL){
        //Insert at the beginning if no previous node comes before new friend name
        new_friend->next = user->friends;
        user->friends = new_friend;
    } 
    else {
        //Insert in the middle or at the end
        new_friend->next = current;
        previous->next = new_friend;
    }
}



/*
Function to display all the post for a user.
*/
void display_all_user_posts(user_t *user){
    assert(user != NULL);

    printf("-----------------------------------------------\n%s's posts\n", user->username);
    post_t *current = user->posts;

    if (current == NULL) {
        printf("No posts available.\n");
    }

    //iterate through linked list
    int post_number = 1;
    while(current != NULL){
        printf("%d - %s\n", post_number, current->content);
        post_number++;
        current = current->next;
    }
    printf("-----------------------------------------------\n");
}


/*
Function to create a poster for a given user.
*/
post_t *create_post(const char *text){
    assert(text != NULL);

    //allocate memory for the post
    post_t *new_post = malloc(sizeof(post_t));
    assert(new_post != NULL);
    strcpy(new_post->content, text);

    //Make next = NULL
    new_post->next = NULL;
    return new_post;
}


/*
Function to create a friend for a given user.
*/
friend_t *create_friend(const char *username){
    assert(username != NULL);

    friend_t *new_friend = malloc(sizeof(friend_t));
    assert(new_friend != NULL);
    strcpy(new_friend->username, username);

    //Make next and linked posts = NULL
    new_friend->posts = NULL;
    new_friend->next = NULL;
    return new_friend;
}


/*
Function to delete (pop) the latest post made by a user.
*/
_Bool delete_post(user_t *user){
    assert(user != NULL);

    if (user->posts == NULL){
        printf("\n-----------------------------------------------\nNo post to delete.\n-----------------------------------------------\n");
        return false;
    }

    //store head in temp post_t
    post_t *temp = user->posts;

    //move next posts to head
    user->posts = user->posts->next;

    //free old head stored in temp post_t
    free(temp);
    return true;
}   


/*
Function to display n number of post by the user.
*/
void display_posts_by_n(user_t *users, int number){
    assert(users != NULL);
    post_t *current = users->posts;

    //iterate through linked list
    int post_number = 1;
    char user_choice;
    do{
        printf("-----------------------------------------------\n%s's posts\n", users->username);
        if (current == NULL) {
            printf("No posts available.\n");
            printf("-----------------------------------------------\n");
            return;
        }

        for (int i = 1; i <= number; i++){
            printf("%d - %s\n", post_number, current->content);
            post_number++;
            current = current->next;
            if (current == NULL){
                printf("All post by the %s displayed\n", users->username);
               printf("-----------------------------------------------\n");
               return;
            }
        }

        printf("-----------------------------------------------\n");

        if(current != NULL){
            printf("Would you like to continue displaying post (y or n): ");
            user_choice = get_input_char();
        }
    }while(user_choice != 'n');
}


/*
Function to display all the friends of a given user.
*/
void  display_user_friends(user_t *users){
    assert(users != NULL);
    printf("-----------------------------------------------\n%s's friends\n", users->username);

    friend_t *current = users->friends;

    if (current == NULL) {
        printf("No friends. loner lmao\n");
    }

    //iterate through linked list
    int friend_number = 1;
    while(current != NULL){
        printf("%d - %s\n", friend_number, current->username);
        friend_number++;
        current = current->next;
    }
    printf("-----------------------------------------------\n");
}


/*
Function to delete friend of a given name.
*/
_Bool delete_friend(user_t *user, char *friend_name){
    friend_t *friend_found = find_friend(user, friend_name);
    if(friend_found == NULL){
        return false;
    }

    if(user->friends == friend_found){
        user->friends = friend_found->next;
    } 
    else{
        // Traverse to find the node before friend_found
        friend_t *current = user->friends;
        while (current->next != friend_found) {
            current = current->next;
        }
        // Update the next pointer to skip friend_found
        current->next = friend_found->next;
    }

    // Free the memory of the deleted friend
    free(friend_found);
    printf("Friend successfully deleted.\n");
    return true; // Deletion successful
}


/*
Function to free all the memory of the users
*/
void teardown(user_t *users) {
    // Iterate through the user list
    while (users != NULL) {
        
        // Free posts linked list
        post_t *current_post = users->posts;
        while (current_post != NULL) {
            post_t *temp_post = current_post;
            current_post = current_post->next;
            free(temp_post); // Free each post
        }

        // Free friends linked list
        friend_t *current_friend = users->friends;
        while (current_friend != NULL) {
            friend_t *temp_friend = current_friend;
            current_friend = current_friend->next;
            free(temp_friend); // Free each friend
        }

        // Move to the next user and free the current user
        user_t *temp_user = users;
        users = users->next;
        free(temp_user); // Free the user node
    }
}
