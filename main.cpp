/**********************************************************
 * Project 5: Yahtzee
 * Course: CS 141, Fall 2022
 * System: MacOS using CLion
 * Starter Code Author: Dr. Sara Riazi
 * Student Author: Reem Allam
 **********************************************************/

#include <string>
#include <iostream>
#include <stdlib.h>
#include <iomanip>

using namespace std;

//#################################################################################################
class Dice{
private:
    int value;

public:
    //Randomly assigns a value between from 1 to 6 to the dice.
    void roll(){
      value = (rand() % 6) + 1;
        }
    int reveal(){
        return value;
    }

    //The auto grader will set the values directly instead of calling roll() to avoid randomness!
    void setValue(int value){
        this->value = value;
    }
};

//#################################################################################################

const int HAND_SIZE = 5;

class Hand{
public:

    //Display the value of the five dice
    void show(){
        // reveals dice value
        cout << "Hand:";
        for(int i = 0; i < HAND_SIZE; i++){
           cout << " " << dices[i]->reveal();
         }
        cout << endl;
       }
    void play(){
        //rolls dices user didn't keep
      for (int i = 0; i < HAND_SIZE; i++){
            if (rolling[i]){
                dices[i]->roll();
                }
         }
        }
        //number value of dice, returns integer so it calculates
    Dice* getDice(int diceNum){
        if (diceNum >= 0 && diceNum <= HAND_SIZE - 1){
            return dices[diceNum];
            }
        return nullptr;
        }

    //selection is the string of dice numbers that the player wants to keep.
    //For example, "125" means that player wants to keep the first, second, and fifth dice, and roll the rest.
    void setSelection(string selection){
        // the rolling array, indicate that the dice is kept or not
        // if kept then rolling is false, true means that dice is rolling
        for(int i = 0; i < HAND_SIZE; i++){
            rolling[i] = true;
            }
        // check the dice we want to keep by going through each number from input
        // check if input is valid or not, if length of input > HAND_SIZE then it is invalid
        if (selection.size() <= HAND_SIZE){
            // checks each letter in input
           for (int i = 0; i < selection.size(); i++){
                char ch = selection.at(i);
                // only see letter is digit, then set it to false
                if (ch >= 49 && ch <= (48 + HAND_SIZE)){
                    rolling[ch - 49] = false;
                    }
               }
            }
        }

    Hand(){
      // initializes rolling and dice to true
        for (int i = 0; i < HAND_SIZE; i++) {
            rolling[i] = true;
            dices[i] = new Dice();
            }
        }

private:
   Dice *dices[HAND_SIZE];
    bool rolling[HAND_SIZE];
};

//######################################################################
//List of rows in the board
const int ONES = 0;
const int TWOS = 1;
const int THREES = 2;
const int FOURS = 3;
const int FIVES = 4;
const int SIXES = 5;
const int THREE_OF_KIND = 6;
const int FOUR_OF_KIND = 7;
const int FULL_HOUSE = 8;
const int SMALL_STRAIGHT = 9;
const int LARGE_STRAIGHT = 10;
const int CHANCE = 11;
const int YAHTZEE = 12;

const int BOARD_SIZE = 13;

class Game{

public:
    //calcScore returns a score of a hand (5 dice) for given row in the board.
    //Note the rows are indexed from zero.
    //For example if the hand is 2 1 1 5 1 then calcScore(hand, ONES) returns 3 and calcScore(hand, TWOS) returns 2.
    int calcScore(Hand* hand, int row){
        if(row < 0 || row >= BOARD_SIZE) {
            return 0;
        }
        // gets list of dices
        Dice* dices[HAND_SIZE];
        for(int i = 0; i < HAND_SIZE; i++) {
            dices[i] = hand->getDice(i);
            }
        // sorting it by value of dice by abscessing
        // using selection sort
        for(int i = 0; i < HAND_SIZE; i++) {
            // find the minimum value from rest of array
            // and do swap with current element
            int min = i;
            for(int j = i+1; j < HAND_SIZE; j++) {
                if(dices[min]->reveal() > dices[j]->reveal()) {
                    min = j;
                }
                }
            // do swap if found new min value of rest array
            if(dices[min]->reveal() < dices[i]->reveal()){
                Dice* temp = dices[min];
                dices[min] = dices[i];
                dices[i] = temp;
                }
            }
        // now calculate the score for each row
        int scores = 0;
        if(row <= SIXES) {
            for(int i = 0; i < HAND_SIZE; i++) {
                if(dices[i]->reveal() == (row + 1)){
                    scores += row + 1;
                    }
                }
            }
        else if(row == THREE_OF_KIND) {
            // check if there is three of kind
            bool has_three = false;
            for (int i = 0; i < HAND_SIZE - 2; i++) {
                if (dices[i]->reveal() == dices[i + 1]->reveal() && dices[i]->reveal() == dices[i + 2]->reveal()) {
                    has_three = true;
                    break;
                }
            }

            // if it has three of kind then sum all dices
            if (has_three) {
                for (int i = 0; i < HAND_SIZE; i++) {
                    scores += dices[i]->reveal();
                }
            }
        }

        else if(row == FOUR_OF_KIND){
            // check if there is four of kind
            bool has_four = false;
            for(int i = 0; i < HAND_SIZE - 3; i++) {
                if(dices[i]->reveal() == dices[i+1]->reveal() && dices[i]->reveal() == dices[i+2]->reveal() && dices[i]->reveal() == dices[i+3]->reveal()) {
                    has_four = true;
                    break;
                    }
                }
            // if it has four of kind then sum all dices
            if(has_four) {
                for(int i = 0; i < HAND_SIZE; i++) {
                    scores += dices[i]->reveal();
                    }
                }
            }
        else if(row == FULL_HOUSE){
            // check if it has full house in hand
            if(dices[0]->reveal() == dices[1]->reveal() && dices[2]->reveal() == dices[3]->reveal() && dices[2]->reveal() == dices[4]->reveal()) {
                scores = 25;
            }
            else if(dices[0]->reveal() == dices[1]->reveal() && dices[2]->reveal() == dices[0]->reveal() && dices[3]->reveal() == dices[4]->reveal()) {
                scores = 25;
            }
            }
        else if(row == SMALL_STRAIGHT){
            // create new dices with list of dice no duplicated value
            Dice* new_dices[HAND_SIZE];
            new_dices[0] = dices[0];
            int count = 1;
            for(int i = 1; i < HAND_SIZE; i++) {
                if (new_dices[count - 1]->reveal() != dices[i]->reveal()) {
                new_dices[count++] = dices[i];
            }
                }
            if(count >= 4) {
                // check if it has small straight
                bool isSmall = true;
                for (int i = 0; i < HAND_SIZE - 2; i++) {
                    if ((new_dices[i]->reveal() + 1) != new_dices[i + 1]->reveal()) {
                        isSmall = false;
                        break;
                    }
                }

                if (isSmall) {
                    scores = 30;
                } else if (count == 5) {
                    isSmall = true;
                    for (int i = 1; i < HAND_SIZE - 1; i++) {
                        if ((new_dices[i]->reveal() + 1) != new_dices[i + 1]->reveal()) {
                            isSmall = false;
                            break;
                        }
                    }
                    if(isSmall) {
                        scores = 30;
                    }
                }
            }
            }
        else if(row == LARGE_STRAIGHT){
            // check if it has large straight
            bool isLarge = true;
            for(int i = 0; i < HAND_SIZE - 1; i++) {
                if((dices[i]->reveal() + 1) != dices[i+1]->reveal()) {
                    isLarge = false;
                    break;
                    }
                }
            if(isLarge){
            scores = 40;
            }
            }
        else if(row == CHANCE){
            for(int i = 0; i < HAND_SIZE; i++) {
                scores += dices[i]->reveal();
            }
            }
        else {
            // yahtzee
            //check if order is yahtzee
            bool isYahtzee = true;
            for(int i = 0; i < HAND_SIZE - 1; i++) {
                if(dices[i]->reveal() != dices[i+1]->reveal()){
                    isYahtzee = false;
                    break;
                }
                }
            if(isYahtzee) {
                scores = 50;
            }
            }
        return scores;
        }


    //Displays the yahtzee board
    void show(){
        cout << left;
        if(rowCombine[ONES]){
            cout << setw(5) << "1." << setw(25) << "Ones:" << setw(5) << rowScores[ONES] << endl;
            }
        else{
            cout << setw(5) << "1." << setw(25) << "Ones:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[TWOS]){
            cout << setw(5) << "2." << setw(25) << "Twos:" << setw(5) << rowScores[TWOS]<< endl;
            }
        else{
            cout << setw(5) << "2." << setw(25) << "Twos:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[THREES]){
            cout << setw(5) << "3." << setw(25) << "Threes:" << setw(5) << rowScores[THREES]<< endl;
            }
        else{
            cout << setw(5) << "3." << setw(25) << "Threes:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[FOURS]){
            cout << setw(5) << "4." << setw(25) << "Fours:" << setw(5) << rowScores[FOURS]<< endl;
            }
        else{
            cout << setw(5) << "4." << setw(25) << "Fours:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[FIVES]){
            cout << setw(5) << "5." << setw(25) << "Fives:" << setw(5) << rowScores[FIVES]<< endl;
            }
        else{
            cout << setw(5) << "5." << setw(25) << "Fives:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[SIXES]){
            cout << setw(5) << "6." << setw(25) << "Sixes:" << setw(5) << rowScores[SIXES]<< endl;
            }
        else{
            cout << setw(5) << "6." << setw(25) << "Sixes:" << setw(5) << "-"<< endl;
            }
            cout << setw(5) << " " << setw(25) << "Sum:" << setw(5) << getUpperScore() << endl;
        cout << setw(5) << " " << setw(25) << "Bonus:" << setw(5) << getBonusScore() << endl;

        if(rowCombine[THREE_OF_KIND]){
            cout << setw(5) << "7." << setw(25) << "Three of a kind:" << setw(5) << rowScores[THREE_OF_KIND]<< endl;
            }
        else{
            cout << setw(5) << "7." << setw(25) << "Three of a kind:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[FOUR_OF_KIND]){
            cout << setw(5) << "8." << setw(25) << "Four of a kind:" << setw(5) << rowScores[FOUR_OF_KIND]<< endl;
            }
        else{
            cout << setw(5) << "8." << setw(25) << "Four of a kind:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[FULL_HOUSE]){
            cout << setw(5) << "9." << setw(25) << "Full house:" << setw(5) << rowScores[FULL_HOUSE]<< endl;
            }
        else{
            cout << setw(5) << "9." << setw(25) << "Full house:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[SMALL_STRAIGHT]){
            cout << setw(5) << "10." << setw(25) << "Small straight:" << setw(5) << rowScores[SMALL_STRAIGHT]<< endl;
            }
        else{
            cout << setw(5) << "10." << setw(25) << "Small straight:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[FULL_HOUSE]){
            cout << setw(5) << "11." << setw(25) << "Large straight:" << setw(5) << rowScores[LARGE_STRAIGHT]<< endl;
            }
        else{
            cout << setw(5) << "11." << setw(25) << "Large straight:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[CHANCE]){
            cout << setw(5) << "12." << setw(25) << "Chance:" << setw(5) << rowScores[CHANCE]<< endl;
            }
        else{
            cout << setw(5) << "12." << setw(25) << "Chance:" << setw(5) << "-"<< endl;
            }

        if(rowCombine[YAHTZEE]){
            cout << setw(5) << "13." << setw(25) << "Yahtzee:" << setw(5) << rowScores[YAHTZEE]<< endl;
            }
        else{
            cout << setw(5) << "13." << setw(25) << "Yahtzee:" << setw(5) << "-"<< endl;
            }

        cout << setw(30) << "Total:" << setw(5) << getTotalScore() << endl;
        // hand->show();
        }

        //Returns the score of the upper part of the board
    int getUpperScore(){
        int sum = 0;
        // calculate the sum of all row from ONES to SIXES
        for(int i = ONES; i <= SIXES; i++) {
            sum += rowScores[i];
            }
        return sum;
        }

    //Returns the score of the lower part of the board
    int getLowerScore(){
        // calculate the sum of all row from THREE_OF_KIND to YAHTZEE
        int sum = 0;
        for(int i = THREE_OF_KIND; i <= YAHTZEE; i++) {
            sum += rowScores[i];
            }
        return sum;
        }


    //Returns the bonus points
    int getBonusScore(){
        // calculate the sum of all row from ONES to SIXES
        int upScores = getUpperScore();
        // then check if it has bonus here for upScores >= 63
        if(upScores >= 63) {
            return 35;
        }
        return 0;
        }

    //Returns the total score
    int getTotalScore(){
        int tot = getUpperScore() + getLowerScore() + getBonusScore();
        return tot;
        }

        //Play a hand based on the selected row
    void play(Hand* hand, int row){
        // check if row is valid value [0,(BOARD_SIZE-1)]
        if(row >= 0 && row < BOARD_SIZE) {
            // then check if this row not combined
            if(!rowCombine[row]){
                // calculate the score and update for this row
                int scores = calcScore(hand, row);
                rowScores[row] = scores;// set flag that this row already combined
                rowCombine[row] = true;
                }
            }
        }

    //Check to see if a row has been played or not (returns true if a row has been played)
    bool isPlayed(int row){
        // if this row is combined, then it played
        if(rowCombine[row]) {
            return true;
        }
        return false;
        }

    //Check to see if all rows haven been played or not (returns true if all rows have been played)
    bool isFinished(){
        // if all row is played then game finished
        for(int i = 0; i <BOARD_SIZE; i++){
            if(!rowCombine[i]) {
                return false;
            }
            }
        return true;
        }

    Game(){
        // instructor for Game class by initial the row combine flag array
        // and row scores
        for(int i = 0; i < BOARD_SIZE; i++) {
            rowCombine[i] = false;
            rowScores[i] = 0;
            }
        }

private:
    // using two array, one for flag of row combined, value true mean that this row played
    // false mean that this row not combine or not played yet
    bool rowCombine[BOARD_SIZE];
    // array of scores for each row
    int rowScores[BOARD_SIZE];
};

//#######################################################################################

//The run function is the main loop of your program
void run() {
    // create game and hand
    Game game;
    Hand* hand = new Hand();
    // if game not finished, then do the task
    while(!game.isFinished()) {
        // show the board
        game.show();
        // set all dices rolling
        hand->setSelection("");
        // play the hand
        hand->play();
        // then show the hand
        hand->show();
        // play just max is 3 for each turn
        int times = 1;
        while(times < 3){
            // get selection from user
            string selected = "";
            cout << "Keep Dice (s to stop rolling): ";
            cin >> selected;
            // check if user choice stop
            if(selected == "s"){
                break;
                }
            // if not stop, then set for dices keep
            hand->setSelection(selected);
            // play the next time
            hand->play();
            // and show board
            hand->show();
            // increase times
            times++;
            }

        // ask user for which row they want combine
        int row;
        cout << "Select a combination to play: ";
        cin >> row;
        // if user enter invalid input then ask again
        while(cin.fail() && row < 1 && row > 13){
            cout << "Select a combination to play: ";
            cin >> row;
            }
        // set play for this row
        game.play(hand, row - 1);
        }
    // free memory
    delete hand;
}

// THE AUTO GRADER WILL REPLACE THE MAIN FUNCTION.
// DO NOT ADD ANY LOGIC OF YOUR PROGRAM HERE.
int main() {

    srand(time(0));
    run();
    //test_case();
    return 0;
}
