#include "headers/headers.h"
#include "headers/GameManager.h"

using namespace std;

int main()
{
	GameManager GameManager;
	GameManager.setReserveSize();

	int choice;

	do
	{
		cout << "Menu:" << endl;
		cout << "1. Add Game" << endl;
		cout << "2. View All Games" << endl;
		cout << "3. Open a Game" << endl;
		cout << "4. Clear Games" << endl;
		cout << "5. Load Save File" << endl;
		cout << "6. Exit" << endl;
		cout << "Enter Choice (1/2/3/4/5/6)" << endl;


		if (cin >> choice)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input after reading choice

			switch (choice)
			{
			case 1:
			{
				system("cls");
				string name, path, args;
				cout << "Enter Game Name: ";
				getline(cin, name);
				cout << "Enter Game Path: ";
				getline(cin, path);
				cout << "Any Command Arguments: ";
				getline(cin, args);
				GameManager.addGame(path, name, args);
				system("cls");
				break;
			}
			case 2:
			{
				system("cls");
				GameManager.printGames();
				break;
			}
			case 3:
			{
				string name;
				cout << "\nEnter Game: \n> ";
				getline(cin, name);
				bool status = GameManager.openGame(name);
				if (status)
				{
					choice = 6;
					break;
				}
				cout << "Press Enter to continue...";
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				system("cls");
				break;
			}
			case 4:
			{
				system("cls");
				GameManager.deleteGames();
				break;
			}
			case 5:
			{
				system("cls");
				GameManager.loadFromFile();
				GameManager.loopThroughSaveArray();
				break;
			}
			default:
				system("cls");
				cin.clear();
			}
		}
		else
		{
			system("cls");
			cout << "Invalid input. Please enter a valid choice." << endl;
			cin.clear(); // Clear the stream's error state
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore remaining input
		}

	} while (choice != 6);

	return 0;
};