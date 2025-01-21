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
void displayMenu() {
    std::cout << "\n========== Movie Management System ==========\n";
    std::cout << "1.  Add New Actor\n";
    std::cout << "2.  Add New Movie\n";
    std::cout << "3.  Add Actor to Movie\n";
    std::cout << "4.  Update Actor Details\n";
    std::cout << "5.  Update Movie Details\n";
    std::cout << "6.  Display All Actors\n";
    std::cout << "7.  Display All Movies\n";
    std::cout << "8.  Display Actors by Age Range\n";
    std::cout << "9.  Display Movies Released in the Last 3 Years\n";
    std::cout << "10. Display Movies of an Actor\n";
    std::cout << "11. Display Actors in a Movie\n";
    std::cout << "12. Display Actors Known by an Actor\n";
    std::cout << "13. Exit\n";
    std::cout << "14. Run All Tests\n";
    std::cout << "15. Set Actor Rating\n";
    std::cout << "16. Set Movie Rating\n";
    std::cout << "17. Recommend Movies by Rating\n";
    std::cout << "18. Recommend Actors by Rating\n";
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

    int choice;
    do {
        displayMenu();
        std::cin >> choice;

        // Handle non-integer choice
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[Error] Invalid menu choice!\n\n";
            continue;
        }
        // Clear leftover input to safely use getline later
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            //--------------------------------
            // 1) Add New Actor
            //--------------------------------
        case 1: {
            std::string name;
            int yearOfBirth;

            // Prompt for name
            name = promptForString("Enter actor name (type 'exit' to cancel): ");
            if (name.empty()) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Prompt for YOB
            yearOfBirth = promptForInt("Enter year of birth (1-2024, or '0' to cancel): ",
                1, 2024, true);
            if (yearOfBirth == 0) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Add
            app.addNewActor(name, yearOfBirth);
            std::cout << "[Success] Actor added successfully.\n";

            // OPTIONAL: Prompt for rating
            int rating = promptForInt("Enter actor rating (1-10, '0' to skip): ",
                1, 10, true);
            if (rating != 0) {
                // If you have a quick way to get that newly created actor's ID, do so:
                // e.g., assume app.getNextActorId() returns the *next* ID not used,
                // so the newly added actor is getNextActorId()-1. 
                int assignedActorId = app.getNextActorId() - 1;
                app.setActorRating(assignedActorId, rating);
            }

            break;
        }

              //--------------------------------
              // 2) Add New Movie
              //--------------------------------
        case 2: {
            // Prompt for title
            std::string title = promptForString("Enter movie title (type 'exit' to cancel): ");
            if (title.empty()) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Prompt for plot (just one line, can be blank)
            std::cout << "Enter movie plot (can be blank; type 'exit' alone to cancel): ";
            std::string plot;
            std::getline(std::cin, plot);
            plot = trim(plot);
            if (plot == "exit") {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Prompt for release year
            int releaseYear = promptForInt("Enter release year (1900-2024, '0' to cancel): ",
                1900, 2024, true);
            if (releaseYear == 0) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            app.addNewMovie(title, plot, releaseYear);
            std::cout << "[Success] Movie added successfully.\n";

            // OPTIONAL: Prompt for rating
            int rating = promptForInt("Enter movie rating (1-10, '0' to skip): ",
                1, 10, true);
            if (rating != 0) {
                // same logic as for actors
                int assignedMovieId = app.getNextMovieId() - 1;
                app.setMovieRating(assignedMovieId, rating);
            }

            break;
        }

              //--------------------------------
              // 3) Add Actor to Movie
              //--------------------------------
        case 3: {
            // Prompt for actor ID
            std::string actorIdInput = promptForString("Enter actor ID (type 'exit' to cancel): ");
            if (actorIdInput.empty()) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Convert actor ID input to integer
            int actorId;
            try {
                actorId = std::stoi(actorIdInput);
            }
            catch (const std::invalid_argument&) {
                std::cout << "[Error] Invalid actor ID format. Please enter a numerical ID.\n";
                break;
            }
            catch (const std::out_of_range&) {
                std::cout << "[Error] Actor ID out of range. Please enter a valid ID.\n";
                break;
            }

            // Check if actor ID exists
            if (!app.isActorIdUsed(actorId)) {
                std::cout << "[Error] Actor ID " << actorId << " not found.\n";
                break;
            }

            // Prompt for movie ID
            std::string movieIdInput = promptForString("Enter movie ID (type 'exit' to cancel): ");
            if (movieIdInput.empty()) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Convert movie ID input to integer
            int movieId;
            try {
                movieId = std::stoi(movieIdInput);
            }
            catch (const std::invalid_argument&) {
                std::cout << "[Error] Invalid movie ID format. Please enter a numerical ID.\n";
                break;
            }
            catch (const std::out_of_range&) {
                std::cout << "[Error] Movie ID out of range. Please enter a valid ID.\n";
                break;
            }

            // Check if movie ID exists
            if (!app.isMovieIdUsed(movieId)) {
                std::cout << "[Error] Movie ID " << movieId << " not found.\n";
                break;
            }

            // Add actor to movie using IDs
            // Add actor to movie using IDs
            app.addActorToMovieById(actorId, movieId);
            break;
        }



              //--------------------------------
              // 4) Update Actor Details
              //--------------------------------
        case 4: {
            // Prompt for actor ID
            int actorId = promptForInt("Enter actor ID (1-99999, or '0' to cancel): ",
                1, 99999, true);
            if (actorId == 0) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Prompt for new name
            std::string newName = promptForString(
                "Enter new actor name (type 'exit' to cancel): "
            );
            if (newName.empty()) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Prompt for new YOB
            int newYOB = promptForInt(
                "Enter new year of birth (1-2024, or '0' to cancel): ",
                1, 2024, true
            );
            if (newYOB == 0) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            app.updateActorDetails(actorId, newName, newYOB);

            // OPTIONAL: Also prompt if they want to update the rating:
            int newRating = promptForInt(
                "Enter new rating for actor (1-10, '0' to skip): ",
                1, 10, true
            );
            if (newRating != 0) {
                app.setActorRating(actorId, newRating);
            }

            break;
        }

              //--------------------------------
              // 5) Update Movie Details
              //--------------------------------
        case 5: {
            // Prompt for movie ID
            int movieId = promptForInt("Enter movie ID (1-99999, or '0' to cancel): ",
                1, 99999, true);
            if (movieId == 0) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Prompt for new title
            std::string newTitle = promptForString(
                "Enter new movie title (type 'exit' to cancel): "
            );
            if (newTitle.empty()) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Prompt for new plot
            std::cout << "Enter new plot (type 'exit' alone to cancel): ";
            std::string newPlot;
            std::getline(std::cin, newPlot);
            newPlot = trim(newPlot);
            if (newPlot == "exit") {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            // Prompt for new release year
            int newYear = promptForInt(
                "Enter new release year (1900-2024, '0' to cancel): ",
                1900, 2024, true
            );
            if (newYear == 0) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }

            app.updateMovieDetails(movieId, newTitle, newPlot, newYear);

            // OPTIONAL: Also prompt for rating
            int newRating = promptForInt(
                "Enter new rating for movie (1-10, '0' to skip): ",
                1, 10, true
            );
            if (newRating != 0) {
                app.setMovieRating(movieId, newRating);
            }

            break;
        }

              //--------------------------------
              // 6) Display All Actors
              //--------------------------------
        case 6:
            app.displayAllActors();
            break;

            //--------------------------------
            // 7) Display All Movies
            //--------------------------------
        case 7:
            app.displayAllMovies();
            break;

            //--------------------------------
            // 8) Display Actors by Age Range
            //--------------------------------
        case 8: {
            int minAge = promptForInt("Enter minimum age (1-120, '0' to cancel): ",
                1, 120, true);
            if (minAge == 0) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }
            // Make sure maxAge >= minAge
            int maxAge = 0;
            while (true) {
                maxAge = promptForInt("Enter maximum age (>= minAge, '0' to cancel): ",
                    minAge, 120, true);
                if (maxAge == 0) {
                    std::cout << "[Cancelled] Returning to main menu.\n";
                    goto endSwitch;
                }
                if (maxAge < minAge) {
                    std::cout << "[Error] Max age cannot be < min age.\n";
                    continue;
                }
                break;
            }

            app.displayActorsByAge(minAge, maxAge);
            break;
        }

              //--------------------------------
              // 9) Display Movies Released in the Last 3 Years
              //--------------------------------
        case 9:
            app.displayRecentMovies();
            break;

            //--------------------------------
            // 10) Display Movies of an Actor
            //--------------------------------
        case 10: {
            std::string actorName = promptForString(
                "Enter actor name (type 'exit' to cancel): ");
            if (actorName.empty()) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }
            app.displayMoviesOfActor(actorName);
            break;
        }

               //--------------------------------
               // 11) Display Actors in a Movie
               //--------------------------------
        case 11: {
            std::string movieTitle = promptForString(
                "Enter movie title (type 'exit' to cancel): ");
            if (movieTitle.empty()) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }
            app.displayActorsInMovie(movieTitle);
            break;
        }

               //--------------------------------
               // 12) Display Actors Known by an Actor
               //--------------------------------
        case 12: {
            std::string actorName = promptForString(
                "Enter actor name (type 'exit' to cancel): ");
            if (actorName.empty()) {
                std::cout << "[Cancelled] Returning to main menu.\n";
                break;
            }
            app.displayActorsKnownBy(actorName);
            break;
        }

               //--------------------------------
               // 13) Exit
               //--------------------------------
        case 13:
            std::cout << "Exiting... Thank you for using the Movie Management System!\n";
            break;

            //--------------------------------
            // 14) Run All Tests
            //--------------------------------
        case 14:
            app.runAllTests();
            break;

            //--------------------------------
            // 15) Set Actor Rating
            //--------------------------------
        case 15: {
            // Prompt for actor ID
            int actorId = promptForInt(
                "Enter actor ID (1-99999, '0' to cancel): ",
                1, 99999, true
            );
            if (actorId == 0) {
                std::cout << "[Cancelled]\n";
                break;
            }
            // Prompt for rating
            int newRating = promptForInt(
                "Enter new actor rating (1-10, '0' to cancel): ",
                1, 10, true
            );
            if (newRating == 0) {
                std::cout << "[Cancelled]\n";
                break;
            }
            app.setActorRating(actorId, newRating);
            break;
        }

               //--------------------------------
               // 16) Set Movie Rating
               //--------------------------------
        case 16: {
            // Prompt for movie ID
            int movieId = promptForInt(
                "Enter movie ID (1-99999, '0' to cancel): ",
                1, 99999, true
            );
            if (movieId == 0) {
                std::cout << "[Cancelled]\n";
                break;
            }
            // Prompt for rating 
            int newRating = promptForInt(
                "Enter new movie rating (1-10, '0' to cancel): ",
                1, 10, true
            );
            if (newRating == 0) {
                std::cout << "[Cancelled]\n";
                break;
            }
            app.setMovieRating(movieId, newRating);
            break;
        }
        
        case 17: {
            // Recommend movies by rating range
            int minRating = promptForInt(
                "Enter minimum movie rating for recommendations (1-10, '0' to cancel): ",
                1, 10, true
            );
            if (minRating == 0) {
                std::cout << "[Cancelled]\n";
                break;
            }

            int maxRating = promptForInt(
                "Enter maximum movie rating for recommendations (>= minRating, '0' to cancel): ",
                minRating, 10, true
            );
            if (maxRating == 0) {
                std::cout << "[Cancelled]\n";
                break;
            }

            app.recommendMoviesByRating(minRating, maxRating);
            break;
        }

        case 18: {
            // Recommend actors by rating range
            int minRating = promptForInt(
                "Enter minimum actor rating for recommendations (1-10, '0' to cancel): ",
                1, 10, true
            );
            if (minRating == 0) {
                std::cout << "[Cancelled]\n";
                break;
            }

            int maxRating = promptForInt(
                "Enter maximum actor rating for recommendations (>= minRating, '0' to cancel): ",
                minRating, 10, true
            );
            if (maxRating == 0) {
                std::cout << "[Cancelled]\n";
                break;
            }

            app.recommendActorsByRating(minRating, maxRating);
            break;
        }

        default:
            std::cout << "[Error] Invalid menu choice!\n";
}


    endSwitch:;
        std::cout << std::endl;
    } while (choice != 13);

    return 0;
}
