/*

    Final Project: Arduino Asteroids

    Name: Aditya Harvi
    Student ID: 1532770
    Name: Micheal Antifaoff
    Student ID: 1542254
    CMPUT 274, Fall 2018

    Sources:
    Used this to learn how to make sound using the speaker:
    https://learn.adafruit.com/adafruit-arduino-lesson-10-making-sounds/overview

    Went through this guide to understand how concurrency or "multi-tasking" on
    the Arduino is possible using the millis() function:
    https://forum.arduino.cc/index.php?topic=503368.0
*/

#include <Arduino.h>

// Initializes all required outputs and variables.
int cols[4] = {2, 3, 4, 5};
int colPin[20] = {53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, \
A0, A1, A2, A3, A4, A5};
int ButtonRight = {9};
int ButtonLeft = {8};
int speaker = {31};
int tones[11] = {261, 277, 294, 311, 330, 349, 370, 392, 415, 440, 500};
int ButtonStateRight = 0;
int ButtonStateLeft = 0;
int RightState, LeftState;
int Life[3] = {13 , 12, 11};
int counter = -16;
unsigned long startMillis;
unsigned long currentMillis;
int deathComparison[4] = {};
int dropTime = 2000;
int endFlag = 0;
bool AandM = false;

/*
    Sets the initial conditions for the game. It will turn all LED's to OUTPUT
    and set them to be initially off. 'Life' will be turned on to represent
    three lives of the player.
    This will be the first function that is called in main().
*/
void setup() {
    // Sets all green LED's to be the OUTPUT and be initially LOW.
    // The player will move on these LED's
    for (int i = 0; i < 4; i++) {
        pinMode(cols[i], OUTPUT);
        digitalWrite(cols[i], LOW);
    }
    // Sets al lred LED's to be OUTPUT and initially LOW.
    // The asteroids will fall from these LED's.
    for (int i = 0; i < 20; i++) {
        pinMode(colPin[i], OUTPUT);
        digitalWrite(colPin[i], LOW);
    }
    // Sets the buttons to be the trigger whenever they are pressed.
    pinMode(ButtonRight, INPUT_PULLUP);
    pinMode(ButtonLeft, INPUT_PULLUP);
    // Sets the inital conditions for the Right and left button states.
    RightState = digitalRead(ButtonRight);
    LeftState = digitalRead(ButtonLeft);
    // Sets the player in the middle of the board.
    digitalWrite(cols[2], HIGH);
    // All lives will be turned on.
    for (int i = 0; i < 3; i++) {
        pinMode(Life[i], OUTPUT);
        digitalWrite(Life[i], HIGH);
    }
}

/*
    This is going to immitate the setup. It will turn off all red led's and
    place the player in the starting position.
    It will be called whenver the player hits an asteroid.
*/
void immitatesetup() {
    // Turns off all red LED's.
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], LOW);
    }
    // Turns off all green LED's.
    for (int i = 0; i < 4; i++) {
        digitalWrite(cols[i], LOW);
    }
    // Sets player back to initial position.
    digitalWrite(cols[2], HIGH);
}

/*
    This will play a death sounds when the player is hit.
*/
void deathsound() {
    startMillis = millis();
    while (currentMillis - startMillis <= 100) {
        currentMillis = millis();
        tone(speaker, tones[10]);
    }
    startMillis = millis();
    while (currentMillis - startMillis <= 100) {
        currentMillis = millis();
        tone(speaker, tones[8]);
    }
    startMillis = millis();
    while (currentMillis - startMillis <= 300) {
        currentMillis = millis();
        tone(speaker, tones[6]);
    }
    noTone(speaker);
}

/*
    This will play a reset sound letting the player know the game is about
    to reset.
*/
void resetsound() {
    tone(speaker, tones[4]);
    startMillis = millis();
    while (currentMillis - startMillis <= 100) {
        currentMillis = millis();
    }
    tone(speaker, tones[10]);
    startMillis = millis();
    while (currentMillis - startMillis <= 100) {
        currentMillis = millis();
    }
    noTone(speaker);
}

/*
    This function is going to be called in movement(). It will turn the current
    green LED off and turn on the one to the right.
*/
void moveRight() {
    // This is a check to make sure the player does not exit the bounds.
    if (digitalRead(cols[3]) == HIGH) {
        digitalWrite(cols[3], HIGH);
    } else {
        // This chunk of code will allow the player to move one LED over to the
        // right through the use of Flag variable.
        int Flag = 0;
        for (int i = 0; i < 4; i++) {
            if (Flag == 1) {
                digitalWrite(cols[i], HIGH);
                Flag = 0;
                break;
            }
            // Turns off current LED and turns on Flag. This allows the next
            // LED to be turned on in the next itteration of the loop.
            if (digitalRead(cols[i]) == HIGH) {
                digitalWrite(cols[i], LOW);
                Flag = 1;
            }
        }
    }
}

/*
    This function is going to be called in movement(). It will turn the current
    green LED off and turn on the one to the left.
*/
void moveLeft() {
    // The following code is near identical to moveright() function.
    if (digitalRead(cols[0]) == HIGH) {
        digitalWrite(cols[0], HIGH);
    } else {
        int Flag = 0;
        for (int i = 3; i >= 0; i--) {
            if (Flag == 1) {
                digitalWrite(cols[i], HIGH);
                Flag = 0;
                break;
            }
            if (digitalRead(cols[i]) == HIGH) {
                digitalWrite(cols[i], LOW);
                Flag = 1;
            }
        }
    }
}

/*
    If this function is called and lostLife equals true, it will:
    - reduce the number of lives by one.
    - call immitatesetup()
    - flash red on the screen
    - if the player is out of lives, it will turn the game off allowing for
    the final score to be seen.
    If lostLife equals false, it will: 
    - activate the Easter Egg
    - call immitatesetup()
    - restore all of the user's lives
    - flash red on the screen.
*/
void death(bool lostLife) {
    deathsound();
    // Resets the board.
    immitatesetup();
    // Will recude number of lives by one.
    if (lostLife){
        for (int i = 0; i < 3; i++) {
            if (digitalRead(Life[i]) == HIGH) {
                digitalWrite(Life[i], LOW);
                break;
            }
        }
    }
    //If the Easter Egg was found, reset all the lives and play sounds
    if (lostLife == false){
        digitalWrite(Life[2], HIGH);
        deathsound();
        digitalWrite(Life[1], HIGH);
        deathsound();
        digitalWrite(Life[0], HIGH);
    }
    startMillis = millis();
    while (currentMillis - startMillis >= 100) {
        currentMillis = millis();
    }
    resetsound();
    // The following code will make the board flash red to give a
    // visual code that the game will recommence soon.
    for (int i = 0; i < 6; i ++) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 14; i < 20; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[10], HIGH);
    digitalWrite(colPin[9], HIGH);
    delay(100);
    for (int i = 0; i < 6; i ++) {
        digitalWrite(colPin[i], LOW);
    }
    for (int i = 14; i < 20; i++) {
        digitalWrite(colPin[i], LOW);
    }
    digitalWrite(colPin[10], LOW);
    digitalWrite(colPin[9], LOW);
    // Will pause the game for 3 seconds to give the player.
    startMillis = millis();
    while (true) {
        currentMillis = millis();
        // Once 3 seconds have elapsed, it will run the following code.
        if (currentMillis - startMillis >= 3000) {
            // Calls immitatesetup() to reset the board.
            immitatesetup();
            break;
        }
    }
    // If the number of lives are depleated, then it will end the game
    // allowing for the final score to be displayed.
    if (digitalRead(Life[2]) == LOW) {
        endFlag = 1;
    }
}


/*
    This function wil randomly pick a number from 0-4 and turn on the top 4 LED
    depending on the number chosen.
*/
void randomizer() {
    //Easter Egg - lights up four lights in a row
    //This code will only be run if the counter is between 96 and 99 inclusive,
    //otherwise the main code in randomizer will be run
    if (counter == 96 or counter == 97 or counter == 98 or counter == 99){
        digitalWrite(colPin[0], HIGH);
    }
    else{
    // This will pick a random number from 0-4
        int randnum;
        randnum = random(0, 4);
        // If 0 is chosed, the top left LED is turned on.
        if (randnum == 0) {
            digitalWrite(colPin[0], HIGH);
        // If 1 is chosed, then the top left-middle LED is turned on.
        } else if (randnum == 1) {
            digitalWrite(colPin[5], HIGH);
        // If 2 is chosen, the top right-middle LED is turned on.
        } else if (randnum == 2) {
            digitalWrite(colPin[10], HIGH);
        // If 3 is chosen, the top right LED is turned on.
        } else if (randnum == 3) {
            digitalWrite(colPin[15], HIGH);
        }
    }
}

/*
    This function will:
    - Compare the state of the last asteroid and the player to determine if
    they will lose a life
    - Shift all the red LED's down by one
    - Calls randomizer()
*/
void chainreaction() {
    // This will set the death comparison variable to the value of the LED
    // right above the green LED's.
    deathComparison[0] = digitalRead(colPin[4]);
    deathComparison[1] = digitalRead(colPin[9]);
    deathComparison[2] = digitalRead(colPin[14]);
    deathComparison[3] = digitalRead(colPin[19]);
    // Increase the score of the player.
    counter++;
    // It will compare the values of the players position and the asteroids
    // to determine whether or not a player will lose lives. Also resets the
    // speed of the asteroids.
    // The Easter Egg in the code occurs if the user is on their last life
    // and they die on the fourth light in a row in the same column
    for (int i = 0; i < 4; i++) {
        if (digitalRead(cols[i]) == HIGH && deathComparison[i] == HIGH) {
            // If the user is on their last life and they die on the fourth light in a row,
            // call death with the Easter Egg condition and set AandM to true so that it
            // displays A and M after the score
            if (counter == 104 and digitalRead(Life[1]) == LOW){
                death(false);
                AandM = true;
                dropTime = 2000;
            }
            else{
                death(true);
                dropTime = 2000;
            }
        }
    }
    // This is a check to see if the game is still going on.
    if (endFlag == 0) {
        // All this code will simply cascade the red LED's downward.
        int scrapnum[20] = {};
        // Saves the state of current red LED's in a column.
        for (int i = 0; i < 4; i++) {
            if (digitalRead(colPin[i]) == HIGH) {
                scrapnum[i] = 1;
            } else {
                scrapnum[i] = 0;
            }
        }
        // Copies the saved states to the next LED down.
        for (int i = 1; i < 5; i++) {
            if (scrapnum[i-1] == 1) {
                digitalWrite(colPin[i], HIGH);
            } else {
                digitalWrite(colPin[i], LOW);
            }
        }
        // Repeat of before. Now for column 2.
        for (int i = 5; i < 9; i++) {
            if (digitalRead(colPin[i]) == HIGH) {
                scrapnum[i] = 1;
            } else {
                scrapnum[i] = 0;
            }
        }
        for (int i = 6; i < 10; i++) {
            if (scrapnum[i-1] == 1) {
                digitalWrite(colPin[i], HIGH);
            } else {
                digitalWrite(colPin[i], LOW);
            }
        }
        // Repeat of before. Now for column 3.
        for (int i = 10; i < 14; i++) {
            if (digitalRead(colPin[i]) == HIGH) {
                scrapnum[i] = 1;
            } else {
                scrapnum[i] = 0;
            }
        }
        for (int i = 11; i < 15; i++) {
            if (scrapnum[i-1] == 1) {
                digitalWrite(colPin[i], HIGH);
            } else {
                digitalWrite(colPin[i], LOW);
            }
        }
        // Repeat of before. Now column 4.
        for (int i = 15; i < 19; i++) {
            if (digitalRead(colPin[i]) == HIGH) {
                scrapnum[i] = 1;
            } else {
                scrapnum[i] = 0;
            }
        }
        for (int i = 16; i < 20; i++) {
            if (scrapnum[i-1] == 1) {
                digitalWrite(colPin[i], HIGH);
            } else {
                digitalWrite(colPin[i], LOW);
            }
        }
        // Turns off all top red LED's
        for (int i = 0; i < 20; i += 5) {
            digitalWrite(colPin[i], LOW);
        }
        // Calls randomizer for a new red LED to be chosen.
        randomizer();
    }
}

/*
    This function will allow the user to move left-right to avoid the asteroids.
    Calls the moveRight and moveLeft functions.
    Only allows the button to be pressed once to trigger an event.
*/
void movement() {
    // Reads the state of the button.
    RightState = digitalRead(ButtonRight);
    // If the state is different from the previous state then call moveRight().
    if (RightState != ButtonStateRight) {
        if (RightState == LOW) {
            moveRight();
        }
    }
    // Resets the state to a new state so this doesn't repeat.
    ButtonStateRight = RightState;
    // Similar to above.
    LeftState = digitalRead(ButtonLeft);
    if (LeftState != ButtonStateLeft) {
        if (LeftState == LOW) {
            moveLeft();
        }
    }
    ButtonStateLeft = LeftState;
    // Delay for bouncing.
    delay(5);
}

/*
    If called, it will display the number '1' in a 7-seg display.
*/
void number1() {
    for (int i = 19; i >= 15; i--) {
        digitalWrite(colPin[i], HIGH);
    }
}

/*
    If called, it will display the number '2' in a 7-seg display.
*/
void number2() {
    for (int i = 19; i >= 15; i -= 2) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 14; i >= 10; i -= 2) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 9; i >= 5; i -= 2) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[8], HIGH);
    digitalWrite(colPin[16], HIGH);
}

/*
    If called, it will display the number '3' in a 7-seg display.
*/
void number3() {
    for (int i = 19; i >= 15; i--) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 14; i >= 10; i -= 2) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 9; i >= 5; i -= 2) {
        digitalWrite(colPin[i], HIGH);
    }
}

/*
    If called, it will display the number '4' in a 7-seg display.
*/
void number4() {
    for (int i = 19; i >= 15; i--) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 5; i < 8; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[12], HIGH);
}

/*
    If called, it will display the number '5' in a 7-seg display.
*/
void number5() {
    for (int i = 17; i < 20; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[15], HIGH);
    for (int i = 10; i < 15; i += 2) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[14], HIGH);
    for (int i = 5; i < 8; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[9], HIGH);
    digitalWrite(colPin[15], HIGH);
}

/*
    If called, it will display the number '6' in a 7-seg display.
*/
void number6() {
    for (int i = 5; i < 10; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 17; i < 20; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[15], HIGH);
    digitalWrite(colPin[10], HIGH);
    digitalWrite(colPin[12], HIGH);
    digitalWrite(colPin[14], HIGH);
}

/*
    If called, it will display the number '7' in a 7-seg display.
*/
void number7() {
    for (int i = 19; i >= 15; i--) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[5], HIGH);
    digitalWrite(colPin[10], HIGH);
}

/*
    If called, it will display the number '8' in a 7-seg display.
*/
void number8() {
    for (int i = 19; i >= 15; i--) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 5; i < 10; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[10], HIGH);
    digitalWrite(colPin[12], HIGH);
    digitalWrite(colPin[14], HIGH);
}

/*
    If called, it will display the number '9' in a 7-seg display.
*/
void number9() {
    for (int i = 19; i >= 15; i--) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 5; i < 8; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[10], HIGH);
    digitalWrite(colPin[12], HIGH);
}

/*
    If called, it will display the number '0' in a 7-seg display.
*/
void number0() {
    for (int i = 19; i >= 14; i--) {
        digitalWrite(colPin[i], HIGH);
    }
    for (int i = 5; i < 11; i++) {
        digitalWrite(colPin[i], HIGH);
    }
}

/*
    If called, it will display the number from 0-9 in a 7-seg display.
    It will call the previous 'number' functions in order to do this.
    Will display a number based off the score of the player.
*/
void displaynumber(int input) {
    // Turns off all the LEDS- prepairs to display the numbers.
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], LOW);
    }
    delay(100);
    // Calls the proper number function.
    if (input == 1) {
        number1();
    } else if (input == 2) {
        number2();
    } else if (input == 3) {
        number3();
    } else if (input == 4) {
        number4();
    } else if (input == 5) {
        number5();
    } else if (input == 6) {
        number6();
    } else if (input == 7) {
        number7();
    } else if (input == 8) {
        number8();
    } else if (input == 9) {
        number9();
    } else if (input == 0) {
        number0();
    }
}

/*
Display "A" (for Aditya) in morse code, symbolic "and", 
and "M" (for Micheal) in morse code
Easter Egg to be found by user!
*/
void displayAandM(){
    //A is .-
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    delay(700);
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], LOW);
    }
    delay(500);
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    delay(2100);
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], LOW);
    }
    delay(1500);

    //"and" character
    for (int i = 1; i < 4; i++){
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[6], HIGH);
    digitalWrite(colPin[8], HIGH);
    for (int i = 10; i < 15; i++){
        digitalWrite(colPin[i], HIGH);
    }
    digitalWrite(colPin[16], HIGH);
    digitalWrite(colPin[18], HIGH);
    delay(2000);

    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], LOW);
    }
    delay(1000);
    //M is --
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    delay(2100);
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], LOW);
    }
    delay(500);
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], HIGH);
    }
    delay(2100);
    for (int i = 0; i < 20; i++) {
        digitalWrite(colPin[i], LOW);
    }
    delay(2000);

}

/*
    This function will be called in main. Once both buttons are pushed down for
    3 seconds, the game will start.
*/
void start() {
    // This loop is run forever until the player chooses to start the game.
    bool loopBreaker = true;
    while (true) {
        // This will wait until both right and left buttons are pressed for 3
        // seconds before allowing for game to begin.
        startMillis = millis();
        while (digitalRead(ButtonLeft) == LOW &&\
            digitalRead(ButtonRight) == LOW) {
            currentMillis = millis();
            // This will show the number 3 and play a sound.
            if (currentMillis - startMillis <= 500) {
                tone(speaker, tones[0]);
                number3();
            // Turns off everything.
            } else if (currentMillis - startMillis <= 1000) {
                noTone(speaker);
                immitatesetup();
            // Displays number 2 and plays a sound.
            } else if (currentMillis - startMillis <= 1500) {
                tone(speaker, tones[5]);
                number2();
            // Turns everything off.
            } else if (currentMillis - startMillis <=2000) {
                noTone(speaker);
                immitatesetup();
            // Displays number 1 and plays a sound.
            } else if (currentMillis - startMillis < 2500) {
                tone(speaker, tones[10]);
                number1();
            // Turns everything off. Prepares to start the game.
            } else if (currentMillis - startMillis >= 3000) {
                noTone(speaker);
                immitatesetup();
                loopBreaker = false;
                break;
            }
        }
        // If the buttons are ever let go, this will reset the board.
        while (digitalRead(ButtonLeft) == HIGH &&\
            digitalRead(ButtonRight) == HIGH) {
                immitatesetup();
                noTone(speaker);
            }
        if (loopBreaker == false)
            break;
    }
}

/*
    This is the first function which will be called. It will start and end the
    game.
*/
int main() {
    init();
    setup();
    start();
    // This loop here is the essence of the game. It will run forever until the
    // player is out of lives.
    while (true) {
        // Calls chainreaction().
        chainreaction();
        startMillis = millis();
        // Allows 3 seconds for the player to be moving before calling the next
        // itteration of chainreaction.
        while (true) {
            movement();
            currentMillis = millis();
            if (currentMillis - startMillis >= dropTime) {
                // Increase in difficulty every few asteroids
                if (dropTime > 1000) {
                    dropTime -= 50;
                } else if (dropTime > 750) {
                    dropTime -= 25;
                } else if (dropTime > 500) {
                    dropTime -= 10;
                } else {
                    dropTime -= 5;
                }
                startMillis = currentMillis;
                break;
            }
        }
        // This break will be triggered if the player is out of lives.
        if (endFlag == 1) {
            break;
        }
    }
    // This is going to display the final score for the player.
    while (true) {
        // Calls the display number based off 3 digits. So if you have a score
        // of 3, the number 003 will be displayed.
        displaynumber(counter / 100 % 10);
        delay(1000);
        displaynumber(counter / 10 % 10);
        delay(1000);
        displaynumber(counter % 10);
        delay(1000);

        // Turns off the board.
        for (int i = 0; i < 20; i++) {
            digitalWrite(colPin[i], LOW);
        }
        delay(1500);

        //If the Easter Egg condition was met, display our first initials after the score
        if (AandM == true){
            displayAandM();
            for (int i = 0; i < 20; i++) {
                digitalWrite(colPin[i], LOW);
            }
            delay(1500);
        }
        // Turns off the board.
    }
    return 0;
}
