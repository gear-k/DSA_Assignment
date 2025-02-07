/***************************************************
 * main.cpp
 *
 * Complete example of a Movie Management System
 * with fully validated input loops for each menu
 * option, now including rating prompts.
 *
 * Team:            Gearoid, Cedric
 * Group:           1
 * Student IDs:     S10241866, S10241549
 *
 ***************************************************/

#include "MovieApp.h"
#include <iostream>
#include <limits>
#include <string>
#include <cctype>

 /**
  * @brief Trims leading and trailing whitespace from a string.
  *
  * This function removes spaces, tabs, carriage returns, and newline characters
  * from both the beginning and the end of the input string.
  *
  * @param str The string to be trimmed.
  * @return A new string with whitespace removed from both ends.
  */
static std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");
    if (start == std::string::npos || end == std::string::npos) {
        return "";
    }
    return str.substr(start, end - start + 1);
}

/**
 * @brief Prompts the user for an integer within a specified range.
 *
 * This function displays the given prompt and validates that the input is an integer
 * within the range [minVal, maxVal]. If allowCancel is true, entering 0 cancels the prompt.
 *
 * @param prompt The message to display to the user.
 * @param minVal The minimum allowed value.
 * @param maxVal The maximum allowed value.
 * @param allowCancel If true, 0 will signal cancellation.
 * @return The integer entered by the user, or 0 if cancelled.
 */
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return 0;
        }
        // Check range
        if (value < minVal || value > maxVal) {
            std::cout << "[Error] Value out of allowed range (" << minVal << " - " << maxVal << ").\n";
            continue;
        }

        // Clear leftover input and return the valid value
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return value;
    }
}

/**
 * @brief Prompts the user for a non-empty string.
 *
 * This function displays the given prompt and ensures the user does not input an empty string.
 * If allowCancel is true and the user types "exit", an empty string is returned to signal cancellation.
 *
 * @param prompt The message to display to the user.
 * @param allowCancel If true, typing "exit" will cancel the prompt.
 * @return The non-empty string entered by the user, or an empty string if cancelled.
 */
std::string promptForString(const std::string& prompt, bool allowCancel = true) {
    while (true) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);

        input = trim(input);
        if (allowCancel && input == "exit") {
            return "";  // Signal cancellation
        }
        if (input.empty()) {
            std::cout << "[Error] Input cannot be empty.\n";
            continue;
        }
        return input;
    }
}

/**
 * @brief Displays the main menu based on the current mode (admin or user).
 *
 * This function prints the menu options to the console.
 *
 * @param isAdmin True if the application is in admin mode; false otherwise.
 */
void displayMenu(bool isAdmin) {
    std::cout << "\n========== Movie Management System ==========\n";
    if (isAdmin) {
        std::cout << "1.  Add New Actor\n";
        std::cout << "2.  Add New Movie\n";
        std::cout << "3.  Add Actor to Movie\n";
        std::cout << "4.  Update Actor Details\n";
        std::cout << "5.  Update Movie Details\n";
        std::cout << "14. Run All Tests\n"; // Admin-only feature
        std::cout << "19. Toggle Admin/User Mode\n";
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
        std::cout << "19. Toggle Admin/User Mode\n";
        std::cout << "13. Exit\n";
    }
    std::cout << "Enter your choice: ";
}

/**
 * @brief The main function for the Movie Management System.
 *
 * This function loads data, sets the application mode (admin/user), and
 * displays the main menu in a loop until the user chooses to exit.
 *
 * @return 0 upon successful execution.
 */
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
        // Display the appropriate menu based on the current mode
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
                int actorId = promptForInt("Enter actor ID (1-99999, or '0' to cancel): ", 1, 999999999, true);
                if (actorId == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                int movieId = promptForInt("Enter movie ID (1-99999, or '0' to cancel): ", 1, 999999999, true);
                if (movieId == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.addActorToMovieById(actorId, movieId, true);
                break;
            }
            case 4: {
                int actorId = promptForInt("Enter actor ID (1-99999, or '0' to cancel): ", 1, 999999999, true);
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
                int movieId = promptForInt("Enter movie ID (1-99999, or '0' to cancel): ", 1, 999999999, true);
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
                break;
            }
            //case 14:
            //    std::cout << "[Admin] Running all tests...\n";
            //    app.runAllTests();
            //    break;
            case 19: {
                // Switch from ADMIN mode -> USER mode
                app.setAdminMode(false);
                std::cout << "[Switched to USER mode]\n";
                break;
            }
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
                // Prompt for (partial) actor name
                std::string inputName = promptForString("Enter actor name (type 'exit' to cancel): ");
                if (inputName.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }

                // Collect all matching actors
                List<Actor> matchedActors;
                app.findActorsByName(inputName, matchedActors);

                // Count the number of matches
                int matchCount = 0;
                matchedActors.display([&](const Actor& a) {
                    matchCount++;
                    return false; // Continue iterating
                    });

                if (matchCount == 0) {
                    std::cout << "[Error] No actor found with name \"" << inputName << "\".\n";
                    break;
                }
                else if (matchCount == 1) {
                    // Retrieve the actor's ID if exactly one match is found
                    int actorId = 0;
                    matchedActors.display([&](const Actor& a) {
                        actorId = a.getId();
                        return true; // Stop after first match
                        });
                    app.displayMoviesOfActor(actorId);
                }
                else {
                    // Multiple actors found with the same name
                    std::cout << "[Info] Multiple actors found with the name \"" << inputName << "\":\n";

                    static const int MAX_ACTORS = 50;
                    Actor actorArray[MAX_ACTORS];
                    int idx = 0;

                    // Store matched actors into an array for later reference
                    matchedActors.display([&](const Actor& a) {
                        if (idx < MAX_ACTORS) {
                            actorArray[idx++] = a;
                        }
                        return false;
                        });

                    // Display each matched actor's details
                    for (int i = 0; i < idx; i++) {
                        std::cout << "  ID=" << actorArray[i].getId()
                            << ", Name=" << actorArray[i].getName()
                            << ", BirthYear=" << actorArray[i].getBirthYear()
                            << ", Rating=" << actorArray[i].getRating()
                            << "\n";
                    }

                    // Prompt the user to select the correct actor ID
                    int chosenId = promptForInt("Enter the correct Actor ID (or '0' to cancel): ", 1, 999999999, true);
                    if (chosenId == 0) {
                        std::cout << "[Cancelled] Returning to main menu.\n";
                        break;
                    }

                    // Validate the chosen ID
                    bool found = false;
                    std::string finalActorName;
                    for (int i = 0; i < idx; i++) {
                        if (actorArray[i].getId() == chosenId) {
                            finalActorName = actorArray[i].getName();
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        std::cout << "[Error] Invalid Actor ID selected. Returning to main menu.\n";
                        break;
                    }

                    // Display movies of the chosen actor
                    app.displayMoviesOfActor(chosenId);
                }
                break;
            }
            case 11: {
                // Prompt for movie title and display its actors
                std::string inputTitle = promptForString("Enter movie title (type 'exit' to cancel): ");
                if (inputTitle.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.displayActorsInMovie(inputTitle);
                break;
            }
            case 12: {
                // Prompt for actor name and display known actors
                std::string inputName = promptForString("Enter actor name (type 'exit' to cancel): ");
                if (inputName.empty()) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    break;
                }
                app.displayActorsKnownBy(inputName);
                break;
            }
            case 15: {
                int actorId = promptForInt("Enter actor ID (1-99999, '0' to cancel): ", 1, 999999999, true);
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
                int movieId = promptForInt("Enter movie ID (1-99999, '0' to cancel): ", 1, 999999999, true);
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
            case 19: {
                // Prompt for admin password
                std::cout << "Enter admin password: ";
                std::string password;
                std::getline(std::cin, password);

                if (password == "dsaadmin") {
                    app.setAdminMode(true);
                    std::cout << "[Switched to ADMIN mode]\n";
                }
                else {
                    std::cout << "[Error] Incorrect password; staying in user mode.\n";
                }
                break;
            }
            default:
                std::cout << "[Error] Invalid user menu choice!\n";
                break;
            }
        }
        std::cout << std::endl;
    } while (choice != 13);

    return 0;
}
