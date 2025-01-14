#include "MovieApp.h"
#include <iostream>
#include <limits> // for cin.ignore()
using namespace std;

void displayMenu() {
    cout << "\n========== Movie Management System ==========\n";
    cout << "1. Add New Actor\n";
    cout << "2. Add New Movie\n";
    cout << "3. Add Actor to Movie\n";
    cout << "4. Display All Actors\n";
    cout << "5. Display All Movies\n";
    cout << "6. Display Movies of an Actor\n"; // New option
    cout << "7. Display Actors in a Movie\n"; // New option
    cout << "8. Display Actors Known by an Actor\n"; // New option
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    MovieApp app;
    cout << "Loading data from CSV files...\n";
    app.readActors("actors.csv");
    app.readMovies("movies.csv");
    app.readCast("cast.csv");
    cout << "Data loaded successfully.\n";

    int choice;
    do {
        displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid input! Please enter a number between 1 and 9.\n";
            continue;
        }

        switch (choice) {
        case 1: { // Add New Actor
            string name;
            int yearOfBirth;
            cout << "Enter actor name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter year of birth: ";
            cin >> yearOfBirth;
            app.addNewActor(name, yearOfBirth);
            break;
        }
        case 2: { // Add New Movie
            string title, plot;
            int releaseYear;
            cout << "Enter movie title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter movie plot: ";
            getline(cin, plot);
            cout << "Enter release year: ";
            cin >> releaseYear;
            app.addNewMovie(title, plot, releaseYear);
            break;
        }
        case 3: { // Add Actor to Movie
            string actorName, movieTitle;
            cout << "Enter actor name: ";
            cin.ignore();
            getline(cin, actorName);
            cout << "Enter movie title: ";
            getline(cin, movieTitle);
            app.addActorToMovie(actorName, movieTitle);
            break;
        }
        case 4: // Display All Actors
            cout << "\nDisplaying all actors:\n";
            app.displayAllActors();
            break;
        case 5: // Display All Movies
            cout << "\nDisplaying all movies:\n";
            app.displayAllMovies();
            break;
        case 6: { // Display Movies of an Actor
            string actorName;
            cout << "Enter actor name: ";
            cin.ignore();
            getline(cin, actorName);
            app.displayMoviesOfActor(actorName);
            break;
        }
        case 7: { // Display Actors in a Movie
            string movieTitle;
            cout << "Enter movie title: ";
            cin.ignore();
            getline(cin, movieTitle);
            app.displayActorsInMovie(movieTitle);
            break;
        }
        case 8: { // Display Actors Known by an Actor
            string actorName;
            cout << "Enter actor name: ";
            cin.ignore();
            getline(cin, actorName);
            app.displayActorsKnownBy(actorName);
            break;
        }
        case 9: // Exit
            cout << "Exiting... Thank you for using the Movie Management System!\n";
            break;
        default:
            cout << "Invalid choice. Please select a valid option (1-9).\n";
        }

        cout << endl;

    } while (choice != 9);

    return 0;
}
