/********* main.c ********
    Student Name 	= Aydan Eng
    Student Number	= 101298520
*/

// Includes go here
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h> //tolower
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
    
    printf("***********************************************\nWelcome To Text-Based Facebook\n***********************************************\n\n");
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
                printf("Enter a password (min of 8 characters): ");
                scanf("%s", &password);
                while (strlen(password) < 8){
                    printf("Invalid input, enter a password (min of 8 characters): ");
                    scanf("%s", &password);
                }
                
                //add user to linked list of users

                to_lowercase(username, sizeof(username));

                if (find_user(users, username) != NULL){
                    printf("\n****User Already Registered****\n\n");
                    break;
                }
                else{
                    users = add_user(users, username, password);
                    printf("\n****User Added****\n\n");
                }

                break;

            case 2:
                printf("Enter username: ");
                scanf("%s", &username);

                //get input for users password
                printf("Enter password: ");
                scanf("%s", &password);

                user_t *logged_in_user = login(users, username, password);
                if(logged_in_user != NULL){
                    do{
                        print_sub_main_menu(logged_in_user);
                        user_choice = get_input_usi();
                        switch(user_choice){
                            case 1:
                                //Manage user profile

                                //Check if entered password matches current passowrd stored for that user
                                printf("Enter the current password: ");
                                scanf(" %s", &password);
                                if (strcmp(logged_in_user->password, password) != 0){
                                    printf("-----------------------------------------------\nIncorrect Password/Password Error.\n-----------------------------------------------\n\n");
                                    break;
                                }

                                //prompt user to enter new password
                                printf("Enter the new password: ");
                                scanf("%s", logged_in_user->password);
                                printf("\n****Password Changed!****\n\n");
                                break;

                            case 2:
                                //Manage user post

                                //Print the post managin menu using the inputted username 
                                do{
                                    print_post_menu(logged_in_user);
                                    user_choice = get_input_usi();
                                    switch (user_choice){
                                        case 1:
                                            //Add new post

                                            //init char array for post content and scanf input into that array
                                            char post_content[250];
                                            printf("Enter your post content: ");
                                            scanf(" %[^\n]s", &post_content);

                                            //add post for user using add_post function
                                            add_post(logged_in_user, post_content);
                                            display_all_user_posts(logged_in_user);
                                            break;
                                        case 2:
                                            //Delete latest post
                                            if(delete_post(logged_in_user)){
                                                printf("\n-----------------------------------------------\nLatest post deleted successfully.\n-----------------------------------------------\n");
                                                display_all_user_posts(logged_in_user);
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

                            case 3:
                                do{
                                    print_friend_menu(logged_in_user);
                                    user_choice = get_input_usi();
                                    switch (user_choice){
                                        case 1:
                                            //add friend
                                            printf("Enter a new friend's name: ");
                                            scanf("%s", username);
                                            user_t *user_found_friend = find_user(users, username);
                                            if (user_found_friend == NULL){
                                                printf("%s is not a registered user of facebook.\n", username);
                                                break;
                                            }
                                            add_friend(logged_in_user, username);
                                            printf("\n****Friend Added****\n\n");
                                            break;
                                        case 2:
                                            //delete friend
                                            display_user_friends(logged_in_user);
                                            printf("Enter a friend's name to delete: ");
                                            scanf("%s", username);
                                            delete_friend(logged_in_user, username);
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

                            case 4:
                                //find username post
                                printf("Enter a friend's username to display their post: ");
                                scanf("%s", &username);

                                user_t *user_found_post_n = find_user(users, username);
                                if (user_found_post_n == NULL){
                                    printf("%s is not a registered user of facebook.\n", username);
                                    break;
                                }
                                friend_t *friend_found_post_n = find_friend(logged_in_user, username);
                                if (friend_found_post_n == NULL){
                                    break;
                                }
                                display_posts_by_n(user_found_post_n, 3);
                                break;

                            case 5:
                                break;

                            default:
                                //Incase of invalid entry
                                printf("Invalid Entry. Please Try Again.\n");
                                break;
                        }
                    }while(user_choice != 5);
                }
                else{
                    printf("Username or password is incorrect, try again.\n");
                }
                break;
            case 3:
                //Exit program
                printf("Thanks for choosing Facebook, Goodbye!");
                break;
            default:
            //Incase of invalid entry
            printf("Invalid Entry. Please Try Again.\n");
            break;
        }
    }while(user_choice != 3);

    teardown(users);
    return EXIT_SUCCESS;
}