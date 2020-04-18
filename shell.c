#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 256
#define MAX_DIRECTORY_LENGTH 1024

#define CLEAR_SIZE 128
#define DEBUG_DEFAULT 0
#define TOTAL_USERS 10

typedef struct User
{
	char user[MAX_INPUT_LENGTH];
	char pass[MAX_INPUT_LENGTH];
} User;

typedef enum Command
{
	CLEAR, TEXTPALETTE, BACKGROUNDPALETTE, USERCOLOR, DIRECTORYCOLOR,
	MENUCOLOR, MENUCOLOR1, MENUCOLOR2, MENUCOLOR3, MENUCOLOR4,
	DEBUG, USERNAME, LOGIN, USER,

	HELP, EXIT,
	NUM_COMMANDS
} Command;

User registry[TOTAL_USERS];

int numUsers = 0;
int debug = DEBUG_DEFAULT;
User currentUser;
char directory[MAX_DIRECTORY_LENGTH];
char separator;
int userColor = 15;
int directoryColor = 15;
int menuColor1 = 15;
int menuColor2 = 15;
int menuColor3 = 15;
int menuColor4 = 15;

char command[MAX_INPUT_LENGTH];
char throwaway;

const char* commandString(int index)
{
	char result[MAX_INPUT_LENGTH];
	switch (index)
	{
		case CLEAR: strcpy(result, "clear"); break;
		case TEXTPALETTE: strcpy(result, "textpalette"); break;
		case BACKGROUNDPALETTE: strcpy(result, "backgroundpalette"); break;
		case USERCOLOR: strcpy(result, "usercolor"); break;
		case DIRECTORYCOLOR: strcpy(result, "dircolor"); break;
		case MENUCOLOR: strcpy(result, "menucolor"); break;
		case MENUCOLOR1: strcpy(result, "menucolor 1"); break;
		case MENUCOLOR2: strcpy(result, "menucolor 2"); break;
		case MENUCOLOR3: strcpy(result, "menucolor 3"); break;
		case MENUCOLOR4: strcpy(result, "menucolor 4"); break;
		case DEBUG: strcpy(result, "debug"); break;
		case USERNAME: strcpy(result, "username"); break;
		case LOGIN: strcpy(result, "login"); break;
		case USER: strcpy(result, "user"); break;


		case HELP: strcpy(result, "help"); break;
		case EXIT: strcpy(result, "exit"); break;
	}
}

void getNextCommand()
{
	fgets(command, MAX_INPUT_LENGTH, stdin);
	fflush(stdin);

	int length = strlen(command);

	for (int i = 0; i < length; i++)
	{
		command[i] = tolower(command[i]);

		if (command[i] == '\n')
			command[i] = '\0';
	}

	if (debug)
	{
		printf("Debug: %s\n", command);
	}
}

void clearScreen()
{
	for (int i = 0; i < CLEAR_SIZE; i++)
	{
		printf("\n");
	}
}

void endColor()
{
	printf("\033[0m");
}

void printTxtPalette()
{
	int counter = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			printf("\033[38;5;%im", counter);
			printf(" %3i ", counter++);
		}

		printf("\n");
	}

	endColor();
}

void printBkgPalette()
{
	int counter = 0;
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			printf("\033[48;5;%im", counter);
			printf(" %3i ", counter++);
		}

		printf("\n");
	}

	endColor();
}

void printUserLine()
{
	printf("\033[38;5;%im", userColor);
	printf("%s", currentUser.user);
	endColor();
	printf("%c", separator);
	printf("\033[38;5;%im", directoryColor);
	printf("%s: ", directory);
	endColor();
}

void setUserColor()
{
	printf("Enter the user color code: ");
	char input[4];
	fgets(input, 4, stdin);

	int temp = 0;
	for (int i = 0 ; i < 2; i++)
	{
		temp *= 10;
		temp += (int)(input[i] - '0');
	}

	if (debug)
		printf("Debug: num %i\n", temp);

	userColor = temp;
}

void setDirColor()
{
	printf("Enter the user color code: ");
	char input[4];
	fgets(input, 4, stdin);

	int temp = 0;
	for (int i = 0 ; i < 2; i++)
	{
		temp *= 10;
		temp += (int)(input[i] - '0');
	}

	if (debug)
		printf("Debug: num %i\n", temp);

	directoryColor = temp;
}

void setMenuColor(int index)
{
	printf("Enter the menu color code: ");
	char input[4];
	fgets(input, 4, stdin);

	int temp = 0;
	for (int i = 0 ; i < 2; i++)
	{
		temp *= 10;
		temp += (int)(input[i] - '0');
	}

	if (debug)
		printf("Debug: num %i\n", temp);

	switch (index)
	{
		case 0:
			menuColor1 = temp;
			menuColor2 = temp;
			menuColor3 = temp;
			menuColor4 = temp;
			break;
		case 1: menuColor1 = temp; break;
		case 2: menuColor2 = temp; break;
		case 3: menuColor3 = temp; break;
		case 4: menuColor4 = temp; break;
	}
}

void setUsername()
{
	printf("Enter the new username: ");
	char buffer[MAX_INPUT_LENGTH];
	fgets(buffer, MAX_INPUT_LENGTH, stdin);

	for (int i = 0; i < strlen(buffer); i++)
	{
		if (buffer[i] == '\n')
			buffer[i] = '\0';
	}

	if (debug)
		printf("Debug: %s\n", buffer);

	// Clear out old username
	for (int i = 0; i < strlen(currentUser.user); i++)
	{
		currentUser.user[i] = '\0';
	}

	strcpy(currentUser.user, buffer);
}

void toggleDebug()
{
	debug = !debug;
}

void selectionPrompt()
{
	printf("\033[38;5;%im", menuColor4);
	printf("    Selection: ");
	endColor();
}

void printMenuTitle(char title[])
{
	printf("\033[38;5;%im", menuColor2);
	printf("    === ");
	printf("\033[38;5;%im", menuColor1);
	printf("%s", title);
	printf("\033[38;5;%im", menuColor2);
	printf(" ===\n");
	endColor();
}

void printMenuOption(char option[])
{
	printf("\033[38;5;%im", menuColor3);
	printf("    %s\n", option);
	endColor();
}

void loginMenu()
{
	printf("\n");
	printMenuTitle("Log-In Menu");
	printMenuOption("[E]xisting User");
	printMenuOption("[N]ew User");
	printMenuOption("[R]egistry");
	printMenuOption("[Q]uit");
	printf("\n");
}

void userMenu()
{
	printf("\n");

	printf("\033[38;5;%im", menuColor2);
	printf("    === ");
	printf("\033[38;5;%im", menuColor1);
	printf("%s's Menu", currentUser.user);
	printf("\033[38;5;%im", menuColor2);
	printf(" ===\n");
	endColor();

	printMenuOption("[C]hange User Details");
	printMenuOption("[L]ogout");
	printMenuOption("[S]witch User");
	printMenuOption("[Q]uit");
	printf("\n");
}

void userDetailsMenu()
{
	printf("\n");
	printf("\033[38;5;%im", menuColor2);
	printf("    === ");
	printf("\033[38;5;%im", menuColor1);
	printf("%s's Details", currentUser.user);
	printf("\033[38;5;%im", menuColor2);
	printf(" ===\n");
	endColor();

	printMenuOption("[P]assword Change");
	printMenuOption("[U]sername Change");
	printMenuOption("[A]ccess Permissions");
	printMenuOption("[B]ack");
	printf("\n");
}

void userPermissionsMenu()
{
	printf("\n");
	printf("    === Permissions Menu ===\n");
	printf("    [U]ser Level Permissions *\n");
	printf("    [S]ystem Level Permissions\n");
	printf("    [B]ack\n");
	printf("\n");
}

User createUser(char name[], char pass[])
{
	User u;
	strcpy(u.user, name);
	strcpy(u.pass, pass);
	return u;
}

int newUser()
{
	char tUser[MAX_INPUT_LENGTH];
	char tPass[MAX_INPUT_LENGTH];
	char cPass[MAX_INPUT_LENGTH];
	char select;

	// Clear out stdin
	if (!feof(stdin))
	{
		fgetc(stdin); // Get rid of newline
		fflush(stdin);
	}

	// Get Username
	printf("\n");
	printf("    Enter your username: ");
	fgets(tUser, MAX_INPUT_LENGTH, stdin);
	for (int i = 0; i < strlen(tUser); i++)
	{
		if (tUser[i] == '\n')
			tUser[i] = '\0';
	}
	fflush(stdin);

	// Get Password
	printf("    Enter your password: ");
	printf("\033[38;5;0m");
	fgets(tPass, MAX_INPUT_LENGTH, stdin);
	for (int i = 0; i < strlen(tPass); i++)
	{
		if (tPass[i] == '\n')
			tPass[i] = '\0';
	}
	endColor();

	// Confirm Password
	printf("    Confirm your password: ");
	printf("\033[38;5;0m");
	fgets(cPass, MAX_INPUT_LENGTH, stdin);
	for (int i = 0; i < strlen(cPass); i++)
	{
		if (cPass[i] == '\n')
			cPass[i] = '\0';
	}
	endColor();

	// Compare the passwords here
	if (strcmp(tPass, cPass) == 0)
	{
		registry[numUsers++] = createUser(tUser, tPass);
		currentUser = registry[numUsers-1];
		printf("    New user '%s' created!\n\n", tUser);
		return 1;
	}
	else
	{
		printf("    Passwords do not match. Retry password? [Y/N]\n");
		selectionPrompt();

		do
		{
			select = tolower(fgetc(stdin));
			fflush(stdin);

			switch (select)
			{
				case 'y':
					fgetc(stdin);
					fflush(stdin);

					// Get Password
					printf("\n");
					printf("    Enter your password: ");
					printf("\033[38;5;0m");
					fgets(tPass, MAX_INPUT_LENGTH, stdin);
					for (int i = 0; i < strlen(tPass); i++)
					{
						if (tPass[i] == '\n')
							tPass[i] = '\0';
					}
					endColor();

					// Confirm Password
					printf("    Confirm your password: ");
					printf("\033[38;5;0m");
					fgets(cPass, MAX_INPUT_LENGTH, stdin);
					for (int i = 0; i < strlen(cPass); i++)
					{
						if (cPass[i] == '\n')
							cPass[i] = '\0';
					}
					endColor();

					if (strcmp(tPass, cPass) == 0)
					{
						registry[numUsers++] = createUser(tUser, tPass);
						currentUser = registry[numUsers-1];
						printf("    New user '%s' created!\n\n", tUser);
						return 1;
					}
					else
					{
						printf("    Password does not match. Failed to create user '%s'.\n", tUser);
						return 0;
					}
					break;
				case 'n':
					fgetc(stdin);
					fflush(stdin);

					break;
			}
		}
		while (select != 'y' && select != 'n');
	}

	if (debug)
	{
		printf("Debug: %s\n", tUser);
		printf("Debug: %s\n", tPass);
		printf("Debug: %s\n", cPass);
	}

	return 0;
}

int existingUser()
{
	char tUser[MAX_INPUT_LENGTH];
	char tPass[MAX_INPUT_LENGTH];
	char select;

	// Clear out stdin
	fgetc(stdin); // This clears stdin, throws away newline
	fflush(stdin);

	// Get Username
	printf("\n");
	printf("    Enter username: ");
	fgets(tUser, MAX_INPUT_LENGTH, stdin);
	for (int i = 0; i < strlen(tUser); i++)
	{
		if (tUser[i] == '\n')
			tUser[i] = '\0';
	}
	fflush(stdin);

	User t;
	int foundUser = 0;
	for (int i = 0; i < TOTAL_USERS; i++)
	{
		if (strcmp(registry[i].user, tUser) == 0)
		{
			t = registry[i];
			foundUser = 1;
		}
	}

	if (!foundUser)
	{
		printf("    User '%s' does not exist. Would you like to create this user? [Y/N]\n", tUser);
		selectionPrompt();
		do
		{
			select = tolower(fgetc(stdin));
			fgetc(stdin); // Remove newline
			fflush(stdin);

			switch (select)
			{
				case 'y':
					return newUser();
					break;
				case 'n':
					printf("    User '%s' not created.\n\n", tUser);
					break;
			}
		}
		while (select != 'y' && select != 'n');
	}
	else
	{
		// Get Password
		printf("    Enter password: ");
		printf("\033[38;5;0m");
		fgets(tPass, MAX_INPUT_LENGTH, stdin);
		for (int i = 0; i < strlen(tPass); i++)
		{
			if (tPass[i] == '\n')
				tPass[i] = '\0';
		}
		endColor();

		// Compare the passwords here
		if (strcmp(t.pass, tPass) == 0)
		{
			currentUser = t;
			printf("    User '%s' logged in successfully!\n\n", tUser);
			return 1;
		}
		else
		{
			printf("    Password does not match. Log-in attempt failed.\n\n");
		}
	}

	if (debug)
	{
		printf("Debug: %s\n", tUser);
		printf("Debug: %s\n", tPass);
	}

	return 0;
}

void printRegistry()
{
	fgetc(stdin);
	fflush(stdin);

	printf("\n");
	printMenuTitle("User Registry");

	for (int i = 0; i < numUsers; i++)
	{
		printMenuOption(registry[i].user);

		if (i >= numUsers-1)
			printf("\n");
	}
}

void login()
{
	char select;
	char tUser[MAX_INPUT_LENGTH];
	char tPass[MAX_INPUT_LENGTH];
	char cPass[MAX_INPUT_LENGTH];
	int exitFlag = 0;

	do
	{
		loginMenu();
		selectionPrompt();
		select = tolower(fgetc(stdin));

		switch (select)
		{
			case 'e': // Existing User
				exitFlag = existingUser();
				break;
			case 'n': // New User
				exitFlag = newUser();
				break;
			case 'r': // Registry
				printRegistry();
				break;
			case 'q': // Quit
				fgetc(stdin);
				fflush(stdin);
				break;
			default:
				fgetc(stdin);
				fflush(stdin);
				printf("\n");
				printMenuOption("Invalid selection. Please choose a valid selection.");
				break;
		}
	}
	while (select != 'q' && !exitFlag);
}

void passChange()
{
	char oPass[MAX_INPUT_LENGTH];
	char nPass[MAX_INPUT_LENGTH];
	char cPass[MAX_INPUT_LENGTH];

	printf("    Enter old password: ");
	printf("\033[38;5;0m");
	fgets(oPass, MAX_INPUT_LENGTH, stdin);
	for (int i = 0; i < strlen(oPass); i++)
	{
		if (oPass[i] == '\n')
			oPass[i] = '\0';
	}
	endColor();

	if (debug)
	{
		printf("Debug: old -> %s\n", oPass);
		printf("Debug: cPass -> %s\n", currentUser.pass);
	}

	// Check password with old password
	if (strcmp(oPass, currentUser.pass) == 0)
	{
		// Now get the new password
		printf("    Enter new password: ");
		printf("\033[38;5;0m");
		fgets(nPass, MAX_INPUT_LENGTH, stdin);
		for (int i = 0; i < strlen(nPass); i++)
		{
			if (nPass[i] == '\n')
				nPass[i] = '\0';
		}
		endColor();

		printf("    Confirm new password: ");
		printf("\033[38;5;0m");
		fgets(cPass, MAX_INPUT_LENGTH, stdin);
		for (int i = 0; i < strlen(cPass); i++)
		{
			if (cPass[i] == '\n')
				cPass[i] = '\0';
		}
		endColor();

		if (strcmp(nPass, cPass) == 0)
		{
			printf("    New password set!\n");

			strcpy(currentUser.pass, nPass);

			// Update the registry file
			for (int i = 0; i < numUsers; i++)
			{
				if (strcmp(registry[i].user, currentUser.user) == 0)
				{
					strcpy(registry[i].pass, nPass);
				}
			}
		}
		else
		{
			printf("    Passwords do not match. New password not set.\n");
		}
	}
	else
	{
		printf("    Incorrect password entered. Please try again.\n");
	}
}

void usernameChange()
{
	char oPass[MAX_INPUT_LENGTH];
	char nUser[MAX_INPUT_LENGTH];
	char cUser[MAX_INPUT_LENGTH];

	printf("    Enter password: ");
	printf("\033[38;5;0m");
	fgets(oPass, MAX_INPUT_LENGTH, stdin);
	for (int i = 0; i < strlen(oPass); i++)
	{
		if (oPass[i] == '\n')
			oPass[i] = '\0';
	}
	endColor();

	if (debug)
	{
		printf("Debug: old -> %s\n", oPass);
		printf("Debug: cPass -> %s\n", currentUser.pass);
	}

	// Check password with old password
	if (strcmp(oPass, currentUser.pass) == 0)
	{
		// Now get the new password
		printf("    Enter new username: ");
		fgets(nUser, MAX_INPUT_LENGTH, stdin);
		for (int i = 0; i < strlen(nUser); i++)
		{
			if (nUser[i] == '\n')
				nUser[i] = '\0';
		}

		printf("    Confirm new username: ");
		fgets(cUser, MAX_INPUT_LENGTH, stdin);
		for (int i = 0; i < strlen(cUser); i++)
		{
			if (cUser[i] == '\n')
				cUser[i] = '\0';
		}

		if (strcmp(nUser, cUser) == 0)
		{
			printf("    New username set!\n");

			// Update the registry file
			for (int i = 0; i < numUsers; i++)
			{
				if (strcmp(registry[i].user, currentUser.user) == 0)
				{
					strcpy(registry[i].user, nUser);
					currentUser = registry[i];
				}
			}
		}
		else
		{
			printf("    Usernames do not match. New username not set.\n");
		}
	}
	else
	{
		printf("    Incorrect password entered. Please try again.\n");
	}
}

void accessPermissions()
{

}

void userDetails()
{
	char select;
	int exitFlag = 0;

	do
	{
		userDetailsMenu();
		selectionPrompt();
		select = tolower(fgetc(stdin));

		switch (select)
		{
			case 'p':
				fgetc(stdin);
				fflush(stdin);
				passChange();
				break;
			case 'u':
				fgetc(stdin);
				fflush(stdin);
				usernameChange();
				break;
			case 'a':
				fgetc(stdin);
				fflush(stdin);
				accessPermissions();
				break;
			case 'b':
				fgetc(stdin);
				fflush(stdin);
				exitFlag = 1;
				break;
			default:
				printMenuOption("Invalid selection. Please choose a valid selection.");
				fgetc(stdin);
				fflush(stdin);
				break;
		}
	}
	while (select != 'b' || !exitFlag);
}

void user()
{
	char select;
	int exitFlag = 0;

	do
	{
		userMenu();
		selectionPrompt();
		select = tolower(fgetc(stdin));

		switch (select)
		{
			case 'c':
				fgetc(stdin);
				fflush(stdin);
				userDetails();
				break;
			case 'l':
				fgetc(stdin);
				fflush(stdin);

				// Logout current user and login guest
				currentUser = registry[1];
				exitFlag = 1;
				break;
			case 's':
				// Existing user
				exitFlag = existingUser();
				break;
			case 'q':
				fgetc(stdin);
				fflush(stdin);
				break;
			default:
				printMenuOption("Invalid selection. Please choose a valid selection.");
				fgetc(stdin);
				fflush(stdin);
				break;
		}
	}
	while (select != 'q' && !exitFlag);
}

void help()
{
	char buffer[MAX_INPUT_LENGTH];
	printf("\n");
	printMenuTitle("Command List");

	for (int i = 0; i < NUM_COMMANDS; i++)
	{
		strcpy(buffer, commandString(i));
		printMenuOption(buffer);

		if (i >= NUM_COMMANDS-1)
			printf("\n");
	}
}

int main(void)
{
	// strcpy(user, "john_smith");
	registry[numUsers++] = createUser("master", "pass");
	registry[numUsers++] = createUser("guest", "");
	currentUser = registry[1];
	separator = '@';
	strcpy(directory, "root/home/desktop/");

	clearScreen();

	int exitFlag = 0;

	do
	{
		printUserLine();
		getNextCommand();

		if (strcmp(command, "hello") == 0)
		{
			printf("Hello there, %s! Welcome to PegasOS!\n", currentUser.user);
		}
		else if (strcmp(command, commandString(CLEAR)) == 0)
		{
			clearScreen();
		}
		else if (strcmp(command, commandString(TEXTPALETTE)) == 0)
		{
			printTxtPalette();
		}
		else if (strcmp(command, commandString(BACKGROUNDPALETTE)) == 0)
		{
			printBkgPalette();
		}
		else if (strcmp(command, commandString(USERCOLOR)) == 0)
		{
			setUserColor();
		}
		else if (strcmp(command, commandString(DIRECTORYCOLOR)) == 0)
		{
			setDirColor();
		}
		else if (strcmp(command, commandString(MENUCOLOR)) == 0)
		{
			setMenuColor(0);
		}
		else if (strcmp(command, commandString(MENUCOLOR1)) == 0)
		{
			setMenuColor(1);
		}
		else if (strcmp(command, commandString(MENUCOLOR2)) == 0)
		{
			setMenuColor(2);
		}
		else if (strcmp(command, commandString(MENUCOLOR3)) == 0)
		{
			setMenuColor(3);
		}
		else if (strcmp(command, commandString(MENUCOLOR4)) == 0)
		{
			setMenuColor(4);
		}
		else if (strcmp(command, commandString(DEBUG)) == 0)
		{
			toggleDebug();
		}
		else if (strcmp(command, commandString(USERNAME)) == 0)
		{
			setUsername();
		}
		else if (strcmp(command, commandString(LOGIN)) == 0)
		{
			login();
		}
		else if (strcmp(command, commandString(USER)) == 0)
		{
			user();
		}
		else if (strcmp(command, commandString(HELP)) == 0)
		{
			help();
		}
		else if (strcmp(command, commandString(EXIT)) == 0)
		{
			exitFlag = 1;
		}

		// Clear out stdin for next round
		if (feof(stdin))
		{
			while (!feof(stdin))
				throwaway = fgetc(stdin);
		}
	}
	while (!exitFlag);

	return 0;
}