#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// Function prototypes
int humanTurn(char turn, int& previousRoll, int sixes, int& playerScore, int& turnStartScore);
int computerTurn(char turn, int playerScore, int computerScore, bool& computerTurnComplete, int& previousRoll, int sixes);
void displayScore(int computerScore, int playerScore);

int main() {
    srand(time(nullptr));
    int playerScore = 0;
    int computerScore = 0;

    bool computerLostPoints = false;
    int previousHumanRoll = 0;
    int previousComputerRoll = 0;
    int three6player = 0;
    int three6comp = 0;
    int turnStartScore = 0;
    bool bonusTurn = false;


    while (computerScore < 100 && playerScore < 100) {
        cout << "Your turn:\n";
        char humanTurnInput;
        bool playerLostPoints = false;

        turnStartScore = playerScore;

        while (!playerLostPoints) {
            cout << "Press 'r' to roll or 'h' to hold: ";
            cin >> humanTurnInput;
            cout << "\n";
            if (humanTurnInput != 'r' && humanTurnInput != 'h') {
                cout << "Invalid input. Please enter 'r' to roll or 'h' to hold.\n";
                continue;
            }
            int points = humanTurn(humanTurnInput, previousHumanRoll, three6player, playerScore, turnStartScore);

            if (humanTurnInput == 'r') {
                // If player rolls a 1, they lose all points for this turn
                if (points == 0) {
                    playerLostPoints = true;
                }
                else {
                    playerScore += points;
                    displayScore(computerScore, playerScore);

                    // If points = 6 or 12, player gets bonus turn
                    if (points == 6 || points == 12) {
                        // increment til it hits 3
                        three6player++;
                        if (three6player == 3) {
                            cout << "Bonus Turn! Rolling again... \n";
                            three6player = 0;

                            bool bonusTurnComplete = false;
                            char bonusTurnInput = 'r';
                            int bonusRolls = 0;

                            // handle bonus turn
                            while (!bonusTurnComplete) {
                                int points = humanTurn(bonusTurnInput, previousHumanRoll, three6player, playerScore, turnStartScore);
                                playerScore += points;
                                displayScore(computerScore, playerScore);

                                if (bonusTurnInput == 'r') {
                                    if (points == 1 || bonusRolls == 0) {
                                        bonusTurnComplete = true;
                                        points = 0;
                                    } else {
                                        //playerScore += bonusPoints;
                                        displayScore(computerScore, playerScore + points);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            // continue to computer after holding
            else if (humanTurnInput == 'h') {
                playerLostPoints = true;
            }
        }

        // check if player won
        if (playerScore >= 100) {
            cout << "Congratulations! You win!\n";
            break;
        }


        // Computer's turn
        computerLostPoints = false;
        bool computerTurnComplete = false;
        cout << "Computer's turn:";
        cout << "\n";
        char computerTurnInput = 'r';

        while (!computerLostPoints && !computerTurnComplete) {
            int points = computerTurn(computerTurnInput, playerScore, computerScore, computerTurnComplete, previousComputerRoll, three6comp);
            // If computer rolls a 1, they lose all points for this turn
            if (points == 0) {
                computerLostPoints = true;
                computerTurnComplete = true;
                displayScore(computerScore, playerScore);
                break;
            }
            // If points = 6 or 12, computer gets bonus turn
            else {
                computerScore += points;
                displayScore(computerScore, playerScore);
                if (points == 6 || points == 12) {
                    three6comp++;
                    if (three6comp == 3) {
                        cout << "Bonus Turn! Rolling again... \n";
                        three6comp = 0;

                        bool bonusTurnComplete = false;
                        char bonusTurnInput = 'r';
                        int bonusRolls = 0;

                        // handle bonus turn
                        while (!bonusTurnComplete) {
                            int bonusPoints = computerTurn(bonusTurnInput, playerScore, computerScore, bonusTurnComplete, previousComputerRoll, three6comp);
                            computerScore += points;
                            displayScore(computerScore, playerScore);

                            if (bonusTurnInput == 'r' || bonusRolls == 0) {
                                if (bonusPoints != 1) {
                                    playerScore += bonusPoints;
                                    displayScore(computerScore, playerScore);
                                } else {
                                    bonusTurnComplete = true;
                                    bonusPoints = 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        // Display winner
        if (computerScore >= 100){
            cout << "The computer wins!\n";
            break;
        }
    }
    return 0;
}

// Human's Function
int humanTurn(char turn, int& previousRoll, int sixes, int& playerScore, int& turnStartScore, int& bonusTurn) {
    int points = 0;

    // Check if player's score is 100 or greater after each roll
    if (playerScore >= 100) {
        cout << "Congratulations! You win!\n";
        return points;  // Exit the function
    }

    if (turn == 'r') {
        int rolls = 1 + (rand() % 6);
        cout << "You rolled a " << rolls << ". ";

        /*
        if (rolls != 6) {
            sixes = 0;
        }
        */

        // Conditional statements
        if (rolls == 1) {
            points = 0;
            playerScore = turnStartScore;
            //sixes = 0;
        } else if (previousRoll == rolls) {
            points = rolls * 2;
            cout << "Consecutive roll! Points doubled!\n";
        } else {
                points = rolls;
        }

        previousRoll = rolls;


        } else if (turn == 'h') {
            // Player holds, no additional points for this turn
        }

        return points;
    }

// Computer's Function
    int computerTurn(char turn, int playerScore, int computerScore, bool &computerTurnComplete, int &previousRoll,
                     int sixes) {
        int points = 0;
        int scoreDifference = computerScore - playerScore;
        int targetPoints = 0;

        // Conditional statements to determine target points
        if (scoreDifference >= 10) {
            targetPoints = 10;
        } else if (scoreDifference >= -10) {
            targetPoints = 15;
        } else {
            targetPoints = 25;
        }

        // Conditional statements
        while (points < targetPoints) {
            if (computerScore >= 100) {
                cout << "The computer wins!\n";
                computerTurnComplete = true;
                return points;  // Exit the function
            }

            int rolls = 1 + (rand() % 6);
            cout << "Computer rolled a " << rolls << ".";

            // Check if the computer's score is 100 or greater after each roll
            if (computerScore + points >= 100) {
                cout << "The computer wins!\n";
                computerTurnComplete = true;
                return points;  // Exit the function
            }

            if (rolls == 1) {
                points -= points;
                cout << "Computer lost all points for this turn.\n";
                break;
            } else if (previousRoll == rolls) {
                points += (rolls * 2);
                cout << "Consecutive roll! Points doubled!\n";
            } else {
                points += rolls;
            }

            displayScore(computerScore + points, playerScore);
            previousRoll = rolls;

        }

        // handle target points
        if (points >= targetPoints) {
            cout << "Computer holds." << endl;
            computerTurnComplete = true;
        }

        return points;
    }

// Display score function
    void displayScore(int computerScore, int playerScore) {
        cout << "\nComputer's score: " << computerScore << "  Player's score: " << playerScore << "\n";
        cout << "---------------------------------------\n";
    }