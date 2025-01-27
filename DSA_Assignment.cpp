/***************************************************
 * main.cpp
 *
 * Complete example of a Movie Management System
 * with fully validated input loops for each menu
 * option, now including rating prompts.
 ***************************************************/

#include "MovieApp.h"
#include <iostream>
#include <limits>
#include <string>
#include <cctype>

 //-----------------------------------------------
 // 1) Helper functions
 //-----------------------------------------------

 // Trim leading & trailing whitespace
static std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

// Prompt for integer in [minVal, maxVal]. 
// 0 means "cancel" if allowCancel = true.
int promptForInt(const std::string& prompt, int minVal, int maxVal, bool allowCancel = true) {
    while (true) {
        std::cout << prompt;

        int value;
        std::cin >> value;

        if (std::cin.fail()) {
            // Non-integer entered
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[Error] Please enter a valid integer.\n";
            continue;
        }
        // If user enters 0 and cancellation is allowed, return 0
        if (allowCancel && value == 0) {
            // Clear leftover input
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return 0;
        }
        // Check range
        if (value < minVal || value > maxVal) {
            std::cout << "[Error] Value out of allowed range ("
                << minVal << " - " << maxVal << ").\n";
            continue;
        }

        // Clear leftover input and return
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

// Prompt for non-empty string. 
// If user types "exit" and allowCancel = true, returns "" to signal cancellation.
std::string promptForString(const std::string& prompt, bool allowCancel = true) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        input = trim(input);
        if (allowCancel && input == "exit") {
            return "";  // signals cancellation
        }
        if (input.empty()) {
            std::cout << "[Error] Input cannot be empty.\n";
            continue;
        }
        return input;
    }
}

//-----------------------------------------------
// 2) Display menu
//-----------------------------------------------
void displayMenu(bool isAdmin) {
    std::cout << "\n========== Movie Management System ==========\n";
    if (isAdmin) {
        std::cout << "1.  Add New Actor\n";
        std::cout << "2.  Add New Movie\n";
        std::cout << "3.  Add Actor to Movie\n";
        std::cout << "4.  Update Actor Details\n";
        std::cout << "5.  Update Movie Details\n";
        std::cout << "14. Run All Tests\n"; // Admin-only feature
        std::cout << "13. Exit\n";
    }
    else {
        std::cout << "6.  Display All Actors\n";
        std::cout << "7.  Display All Movies\n";
        std::cout << "8.  Display Actors by Age Range\n";
        std::cout << "9.  Display Movies Released in the Last 3 Years\n";
        std::cout << "10. Display Movies of an Actor\n";
        std::cout << "11. Display Actors in a Movie\n";
        std::cout << "12. Display Actors Known by an Actor\n";
        std::cout << "15. Set Actor Rating\n";
        std::cout << "16. Set Movie Rating\n";
        std::cout << "17. Recommend Movies by Rating\n";
        std::cout << "18. Recommend Actors by Rating\n";
        std::cout << "13. Exit\n";
    }
    std::cout << "Enter your choice: ";
}


//-----------------------------------------------
// 3) Main
//-----------------------------------------------
int main() {
    MovieApp app;
    std::cout << "Loading data from CSV files...\n";
    app.readActors("actors.csv");
    app.readMovies("movies.csv");
    app.readCast("cast.csv");
    std::cout << "Data loaded successfully.\n";

    // Admin Mode Setup
    std::cout << "Enter admin mode? (y/n): ";
    char adminChoice;
    std::cin >> adminChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (tolower(adminChoice) == 'y') {
        std::cout << "Enter admin password: ";
        std::string password;
        std::getline(std::cin, password);
        if (password == "dsaadmin") {
            app.setAdminMode(true);
            std::cout << "[Admin Mode Enabled]\n";
        }
        else {
            std::cout << "[Error] Incorrect password. Switching to user mode.\n";
            app.setAdminMode(false);
        }
    }
    else {
        app.setAdminMode(false);
        std::cout << "[User Mode Enabled]\n";
    }

    int choice;
    do {
        // Display the appropriate menu based on the mode
        displayMenu(app.isAdminMode());
        std::cin >> choice;

        // Handle non-integer input
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[Error] Invalid menu choice!\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (app.isAdminMode()) {
            // Admin-specific options
            switch (choice) {
            case 1: {
                std::string name = promptForString("Enter actor name (type 'exit' to cancel): ");
                if (name.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                int yearOfBirth = promptForInt("Enter year of birth (1-2024, or '0' to cancel): ", 1, 2024, true);
                if (yearOfBirth == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.addNewActor(name, yearOfBirth);
                std::cout << "[Success] Actor added successfully.\n";
                break;
            }
            case 2: {
                std::string title = promptForString("Enter movie title (type 'exit' to cancel): ");
                if (title.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                std::cout << "Enter movie plot (can be blank; type 'exit' alone to cancel): ";
                std::string plot;
                std::getline(std::cin, plot);
                plot = trim(plot);
                if (plot == "exit") {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                int releaseYear = promptForInt("Enter release year (1900-2024, '0' to cancel): ", 1900, 2024, true);
                if (releaseYear == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.addNewMovie(title, plot, releaseYear);
                std::cout << "[Success] Movie added successfully.\n";
                break;
            }
            case 3: {
                int actorId = promptForInt("Enter actor ID (1-99999, or '0' to cancel): ", 1, 99999, true);
                if (actorId == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                int movieId = promptForInt("Enter movie ID (1-99999, or '0' to cancel): ", 1, 99999, true);
                if (movieId == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.addActorToMovieById(actorId, movieId, true);
                break;
            }
            case 4: {
                int actorId = promptForInt("Enter actor ID (1-99999, or '0' to cancel): ", 1, 99999, true);
                if (actorId == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                std::string newName = promptForString("Enter new actor name (type 'exit' to cancel): ");
                if (newName.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                int newYOB = promptForInt("Enter new year of birth (1-2024, or '0' to cancel): ", 1, 2024, true);
                if (newYOB == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.updateActorDetails(actorId, newName, newYOB);
                break;
            }
            case 5: {
                int movieId = promptForInt("Enter movie ID (1-99999, or '0' to cancel): ", 1, 99999, true);
                if (movieId == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                std::string newTitle = promptForString("Enter new movie title (type 'exit' to cancel): ");
                if (newTitle.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                std::cout << "Enter new plot (type 'exit' alone to cancel): ";
                std::string newPlot;
                std::getline(std::cin, newPlot);
                newPlot = trim(newPlot);
                if (newPlot == "exit") {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                int newYear = promptForInt("Enter new release year (1900-2024, '0' to cancel): ", 1900, 2024, true);
                if (newYear == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.updateMovieDetails(movieId, newTitle, newPlot, newYear);
                break;  // Added break here to terminate the case
            }
            case 14:
                std::cout << "[Admin] Running all tests...\n";
                app.runAllTests();
                break;
            case 13:
                std::cout << "Exiting... Thank you for using the Movie Management System!\n";
                break;
            default:
                std::cout << "[Error] Invalid admin menu choice!\n";
                break;
            }
        }
        else {
            // User-specific options
            switch (choice) {
            case 6:
                app.displayAllActors();
                break;
            case 7:
                app.displayAllMovies();
                break;
            case 8: {
                int minAge = promptForInt("Enter minimum age (1-120, '0' to cancel): ", 1, 120, true);
                if (minAge == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                int maxAge = promptForInt("Enter maximum age (>= minAge, '0' to cancel): ", minAge, 120, true);
                if (maxAge == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.displayActorsByAge(minAge, maxAge);
                break;
            }
            case 9:
                app.displayRecentMovies();
                break;
            case 10: {
                std::string actorName = promptForString("Enter actor name (type 'exit' to cancel): ");
                if (actorName.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.displayMoviesOfActor(actorName);
                break;
            }
            case 11: {
                std::string movieTitle = promptForString("Enter movie title (type 'exit' to cancel): ");
                if (movieTitle.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.displayActorsInMovie(movieTitle);
                break;
            }
            case 12: {
                std::string actorName = promptForString("Enter actor name (type 'exit' to cancel): ");
                if (actorName.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.displayActorsKnownBy(actorName);
                break;
            }
            case 15: {
                int actorId = promptForInt("Enter actor ID (1-99999, '0' to cancel): ", 1, 99999, true);
                if (actorId == 0) {
                    std::cout << "[Cancelled]\n";
                    break;
                }
                int newRating = promptForInt("Enter new actor rating (1-10, '0' to cancel): ", 1, 10, true);
                if (newRating == 0) {
                    std::cout << "[Cancelled]\n";
                    break;
                }
                app.setActorRating(actorId, newRating);
                break;
            }
            case 16: {
                int movieId = promptForInt("Enter movie ID (1-99999, '0' to cancel): ", 1, 99999, true);
                if (movieId == 0) {
                    std::cout << "[Cancelled]\n";
                    break;
                }
                int newRating = promptForInt("Enter new movie rating (1-10, '0' to cancel): ", 1, 10, true);
                if (newRating == 0) {
                    std::cout << "[Cancelled]\n";
                    break;
                }
                app.setMovieRating(movieId, newRating);
                break;
            }
            case 17: {
                int minRating = promptForInt("Enter minimum movie rating for recommendations (1-10, '0' to cancel): ", 1, 10, true);
                if (minRating == 0) {
                    std::cout << "[Cancelled]\n";
                    break;
                }
                int maxRating = promptForInt("Enter maximum movie rating for recommendations (>= minRating, '0' to cancel): ", minRating, 10, true);
                if (maxRating == 0) {
                    std::cout << "[Cancelled]\n";
                    break;
                }
                app.recommendMoviesByRating(minRating, maxRating);
                break;
            }
            case 18: {
                int minRating = promptForInt("Enter minimum actor rating for recommendations (1-10, '0' to cancel): ", 1, 10, true);
                if (minRating == 0) {
                    std::cout << "[Cancelled]\n";
                    break;
                }
                int maxRating = promptForInt("Enter maximum actor rating for recommendations (>= minRating, '0' to cancel): ", minRating, 10, true);
                if (maxRating == 0) {
                    std::cout << "[Cancelled]\n";
                    break;
                }
                app.recommendActorsByRating(minRating, maxRating);
                break;
            }
            case 13:
                std::cout << "Exiting... Thank you for using the Movie Management System!\n";
                break;
            default:
                std::cout << "[Error] Invalid user menu choice!\n";
                break;
            }
        }
        std::cout << std::endl;
    } while (choice != 13);

    return 0;
}