#include "MovieApp.h"
#include <iostream>
#include <limits> // For input validation
#include <string>
using namespace std;

void displayMenu() {
    cout << "\n========== Movie Management System ==========\n";
    cout << "1. Add New Actor\n";
    cout << "2. Add New Movie\n";
    cout << "3. Add Actor to Movie\n";
    cout << "4. Update Actor Details\n";
    cout << "5. Update Movie Details\n";
    cout << "6. Display All Actors\n";
    cout << "7. Display All Movies\n";
    cout << "8. Display Actors by Age Range\n";
    cout << "9. Display Movies Released in the Last 3 Years\n";
    cout << "10. Display Movies of an Actor\n";
    cout << "11. Display Actors in a Movie\n";
    cout << "12. Display Actors Known by an Actor\n";
    cout << "13. Exit\n";
    cout << "14. Run All Tests\n"; // New Option
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
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
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
        case 2: {
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
        case 3: {
            string actorName, movieTitle;
            cout << "Enter actor name: ";
            cin.ignore();
            getline(cin, actorName);
            cout << "Enter movie title: ";
            getline(cin, movieTitle);
            app.addActorToMovie(actorName, movieTitle);
            break;
        }
        case 4: {
            int actorId;
            string newName;
            int newYearOfBirth;
            cout << "Enter actor ID: ";
            cin >> actorId;
            cout << "Enter new name: ";
            cin.ignore();
            getline(cin, newName);
            cout << "Enter new year of birth: ";
            cin >> newYearOfBirth;
            app.updateActorDetails(actorId, newName, newYearOfBirth);
            break;
        }
        case 5: {
            int movieId;
            string newTitle, newPlot;
            int newReleaseYear;
            cout << "Enter movie ID: ";
            cin >> movieId;
            cout << "Enter new title: ";
            cin.ignore();
            getline(cin, newTitle);
            cout << "Enter new plot: ";
            getline(cin, newPlot);
            cout << "Enter new release year: ";
            cin >> newReleaseYear;
            app.updateMovieDetails(movieId, newTitle, newPlot, newReleaseYear);
            break;
        }
        case 6:
            app.displayAllActors();
            break;
        case 7:
            app.displayAllMovies();
            break;
        case 8: {
            int minAge, maxAge;
            cout << "Enter minimum age: ";
            cin >> minAge;
            cout << "Enter maximum age: ";
            cin >> maxAge;
            app.displayActorsByAge(minAge, maxAge);
            break;
        }
        case 9:
            app.displayRecentMovies();
            break;
        case 10: {
            string actorName;
            cout << "Enter actor name: ";
            cin.ignore();
            getline(cin, actorName);
            app.displayMoviesOfActor(actorName);
            break;
        }
        case 11: {
            string movieTitle;
            cout << "Enter movie title: ";
            cin.ignore();
            getline(cin, movieTitle);
            app.displayActorsInMovie(movieTitle);
            break;
        }
        case 12: {
            string actorName;
            cout << "Enter actor name: ";
            cin.ignore();
            getline(cin, actorName);
            app.displayActorsKnownBy(actorName);
            break;
        }
        case 13:
            cout << "Exiting... Thank you for using the Movie Management System!\n";
            break;

        case 14: { // New Case for Testing
            app.runAllTests();
            break;
        }
        default:
            cout << "Invalid choice. Please select a valid option (1-13).\n";
        }

        cout << endl;

    } while (choice != 13);

    return 0;
}
