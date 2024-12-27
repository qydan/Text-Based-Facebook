/********* main.c ********
    Student Name 	= Aydan Eng
    Student Number	= 101298520
*/

// Includes go here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a2_nodes.h"
#include "a2_functions.h"

int main()
{
    
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }
    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);

    fclose(csv_file);
    

    //user_t *users = NULL; Init a empty list of users for testing purposes
    int user_choice;
    do{
        //Print Main Menu
        print_menu();

        //Get user's choice for main menu
        user_choice = get_input_usi();
        switch (user_choice) {
            //Initalize char array for username and password to be entered by the user later in the program.
            char username[30];
            char password[15];

            case 1:
                //Register a new user

                //get input for users username
                printf("Enter a username: ");
                scanf("%s", &username);

                //get input for users password
                printf("Enter a password (max of 15 characters): ");
                scanf("%s", &password);
                
                //add user to linked list of users
                users = add_user(users, username, password);
                printf("\n****%s Added!****\n\n", username);
                break;

            case 2:
                //Manage user profile
                //Get username to update password
                printf("Enter a username to update the password: ");
                scanf("%s", &username);

                //Check if user is registered or not, return to main meanu if not found.
                user_t *user_found = find_user(users, username);
                if (user_found == NULL){
                    printf("\n-----------------------------------------------\nUser not found.\n-----------------------------------------------\n\n");
                    break;
                }

                //Check if entered password matches current passowrd stored for that user
                printf("Enter the current password: ");
                scanf(" %s", &password);
                if (strcmp(user_found->password, password) != 0){
                    printf("-----------------------------------------------\nIncorrectPassword/Password Error.\n-----------------------------------------------\n\n");
                    break;
                }

                //prompt user to enter new password
                printf("Enter the new password: ");
                scanf("%s", user_found->password);
                printf("\n****Password Changed!****\n\n");
                break;

            case 3:
                //Manage user post

                //Get username to manage post
                printf("Enter a username to manage their post: ");
                scanf("%s", &username);

                //Find and return user, if not found print message
                user_t *user_found_post = find_user(users, username);
                if (user_found_post == NULL){
                    printf("\n-----------------------------------------------\nUser not found.\n-----------------------------------------------\n\n");
                    break;
                }

                //Print the post managin menu using the inputted username 
                do{
                    print_post_menu(user_found_post);
                    user_choice = get_input_usi();
                    switch (user_choice){
                        case 1:
                            //Add new post

                            //init char array for post content and scanf input into that array
                            char post_content[250];
                            printf("Enter your post content: ");
                            scanf(" %[^\n]s", &post_content);

                            //add post for user using add_post function
                            add_post(user_found_post, post_content);
                            display_all_user_posts(user_found_post);
                            break;
                        case 2:
                            //Delete latest post
                            if(delete_post(user_found_post)){
                                printf("\n-----------------------------------------------\nLatest post deleted successfully.\n-----------------------------------------------\n");
                                display_all_user_posts(user_found_post);
                            }
                            break;
                        case 3:
                            //return to main menu
                            break;
                        default:
                            //Incase of invalid entry
                            printf("Invalid Entry. Please Try Again.\n");
                            break;
                    }
                }while (user_choice != 3);
                break;

            case 4:
                //Manage users friends
                printf("Enter a username to manage their friends: ");
                scanf("%s", &username);

                //Find and return user, if not found print message
                user_t *user_found_friend = find_user(users, username);
                if (user_found_friend == NULL){
                    printf("\n-----------------------------------------------\nUser not found.\n-----------------------------------------------\n\n");
                    break;
                }
                do{
                    print_friend_menu(user_found_friend);
                    user_choice = get_input_usi();
                    switch (user_choice){
                        case 1:
                            //add friend
                            printf("Enter a new friend's name: ");
                            scanf("%s", username);
                            add_friend(users, username);
                            break;
                        case 2:
                            //delete friend
                            display_user_friends(users);
                            printf("Enter a friend's name to delete: ");
                            scanf("%s", username);

                            delete_friend(users, username);
                            break;
                        case 3:
                            //return to main menu
                            break;
                        default:
                            //Incase of invalid entry
                            printf("Invalid Entry. Please Try Again.\n");
                            break;
                    }
                }while(user_choice != 3);
                break;
            case 5:
                //find username post
                printf("Enter a username to display their post: ");
                scanf("%s", &username);

                user_t *user_found_post_n = find_user(users, username);
                if (user_found_post_n == NULL){
                    printf("\n-----------------------------------------------\nUser not found.\n-----------------------------------------------\n\n");
                    break;
                }

                display_posts_by_n(user_found_post_n, 3);

                break;
            case 6:
                //Exit program
                printf("Thanks for choosing Facebook, Goodbye!");
                break;
            default:
            //Incase of invalid entry
            printf("Invalid Entry. Please Try Again.\n");
            break;
        }
    }while(user_choice != 6);

    teardown(users);
}