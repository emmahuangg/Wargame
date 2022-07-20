#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>

using namespace std;

//colours
#define RESET "\033[0m"
#define BLACK "\033[30m"   /* Black */
#define RED "\033[31m"     /* Red */
#define GREEN "\033[32m"   /* Green */
#define YELLOW "\033[33m"  /* Yellow */
#define BLUE "\033[34m"    /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m"    /* Cyan */
#define WHITE "\033[37m"   /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */
#define HBLACK "\033[40m"   /* Black */
#define HRED "\033[41m"     /* Red */
#define HGREEN "\033[42m"   /* Green */
#define HYELLOW "\033[43m"  /* Yellow */
#define HBLUE "\033[44m"    /* Blue */
#define HMAGENTA "\033[45m" /* Magenta */
#define HCYAN "\033[46m"    /* Cyan */
#define HWHITE "\033[47m"   /* White */

//declare global variables
string deck[52];
int playDeck[52];
int deckp1[52];
int deckp2[52];
int deckp1Size = 0;
int deckp2Size = 0;
int tempDeckp1[4];
int tempDeckp2[4];
int tempDeckp1Size = 0;
int tempDeckp2Size = 0;
int discardp1[52];
int discardp2[52];
int discardp1Size = 0;
int discardp2Size = 0;
bool lost = false;
int orderp1[4];
int orderp2[4];
string namep1;
string namep2;
int pointsp1;
int pointsp2;
int gameCount;

void printArray(string arr[], int arrSize)
{
    for (int i = 0; i < arrSize; i++)
    {
        cout << arr[i] << endl;
    }
}

void printArray(int arr[], int arrSize)
{
    for (int i = 0; i < arrSize; i++)
    {
        cout << arr[i] << endl;
    }
}

int getRandom(int totalNum)
{
    return (rand() % totalNum); //generate rand() from seed
}

void shuffle(int arr[], int arrSize)
{
    srand(time(0)); //set a seed based on the current second count
    for (int i = 0; i < arrSize; i++)
    {
        int swappedElem = getRandom(arrSize);
        swap(arr[i], arr[swappedElem]);
    };
}

void printInfo()
{
    cout << endl;
    cout << BOLDWHITE << "PLAYER INFO" << RESET << endl;
    cout << "-------------" << endl;
    cout << HRED << " " + namep1 + ": " << RESET << endl;
    cout << "    Playing deck: " + to_string(deckp1Size) << endl;
    cout << "    Discard pile: " + to_string(discardp1Size) << endl;
    cout << HBLUE << " " + namep2 + ": " << RESET << endl;
    cout << "    Playing deck: " + to_string(deckp2Size) << RESET << endl;
    cout << "    Discard pile: " + to_string(discardp2Size) << endl;
    cout << endl;
}

void drawCards(int &playerDeckSize, int &playerDiscardSize, int (&playerDeck)[52], int (&playerDiscard)[52], int &playerTempDeckSize, int (&playerTempDeck)[4])
{
    //if the player does not have enough cards in their playing pile
    if (playerDeckSize < 4)
    {
        if (playerDiscardSize + playerDeckSize < 4)
        {
            //Player 1 has no more cards to play with
            lost = true;
        }
        else
        {
            //add the discard pile back into the player's playing deck
            for (int i = 0; i < playerDiscardSize; i++)
            {
                playerDeck[playerDeckSize + i] = playerDiscard[i];
            }
            playerDeckSize += playerDiscardSize;
            shuffle(playerDeck, playerDeckSize);
            //reset discard pile
            fill_n(playerDiscard, 52, -1);
            playerDiscardSize = 0;

            //run the function again to redraw 4 cards from the now-sufficient playing deck
            drawCards(playerDeckSize, playerDiscardSize, playerDeck, playerDiscard, playerTempDeckSize, playerTempDeck);
        }
    }
    else
    {
        //initialize the temporary deck
        playerTempDeckSize = 0;

        //loop through the player's deck
        for (int i = 51; i >= 0; i--)
        {
            if (playerDeck[i] != -1) //if there exists a valid card at this location
            {
                playerTempDeck[playerTempDeckSize] = playerDeck[i]; //add the card to the player's temporary deck
                playerDeck[i] = -1;
                playerTempDeckSize++;
                playerDeckSize--;
            }

            //if all four cards have been collected, the loop should end
            if (playerTempDeckSize >= 4)
            {
                break;
            }
        }
    }
}

void arrCards(int (&playerOrder)[4], int (&playerTempDeck)[4])
{
    string orderStr;
    bool validOrder;
    //ask the user to rearrange their cards using permutations of 1234
    do
    {
        fill_n(playerOrder, 4, -1);
        cout << "Please indicate how you would like to arrange your cards by entering a permutation of 1, 2, 3, and 4." << endl;
        cout << BLACK;
        getline(cin, orderStr);
        cout << RESET;
        //remove spaces
        orderStr.erase(remove_if(orderStr.begin(), orderStr.end(), ::isspace), orderStr.end());
        //following serves to validate the user input
        validOrder = true;

        //check if the user entered 4 characters
        if (orderStr.length() != 4)
        {
            cout << YELLOW << "OOPS! Your input was not the right length. Please try again!" << RESET << endl;
            validOrder = false;
            goto tryAgain;
        }
        //iterate through each character to check their validity
        for (int i = 0; i < 4; i++)
        {

            //check if the number is a digit
            if (!isdigit(orderStr[i]))
            {
                cout << YELLOW << "OOPS! Your input is not a permutation of 1234. Please try again!" << RESET << endl;
                validOrder = false;
                goto tryAgain;
            }

            //check if the number is repeated -- if it already exists in the order array
            for (int j = 0; j < 4; j++)
            {
                if ((int)orderStr[i] - 48 == playerOrder[j])
                {
                    cout << YELLOW << "OOPS! Your input has repeated numbers. Please try again!" << RESET << endl;
                    validOrder = false;
                    goto tryAgain;
                }
            }

            //if the number is an unrepeated digit, it can be safely added to the int array orderp1
            playerOrder[i] = (int)orderStr[i] - 48;

            //check if the number is within 1-4
            if (playerOrder[i] < 1 || playerOrder[i] > 4)
            {
                validOrder = false;
                cout << YELLOW << "OOPS! Your input was out of the range 1-4. Please try again!" << RESET << endl;
                goto tryAgain;
            }
        }

    tryAgain:
        cout << endl;
        continue;

    } while (validOrder == false);

    //translate the permutations to valid indices from the overall deck
    for (int i = 0; i < 4; i++)
    {
        playerOrder[i] = playerTempDeck[playerOrder[i] - 1];
    }
}

void printDeck(int printDeck[], int size)
{
    cout << endl;
    for (int i = 0; i < size; i++)
    {
        cout << to_string(i + 1) + " - " + deck[printDeck[i]] << endl;
    }

    cout << endl;
}

void startRound()
{
    do
    {

        cout << endl;
        //print out current info for both players
        printInfo();

        //player 1
        //draw four cards from the player's deck
        drawCards(deckp1Size, discardp1Size, deckp1, discardp1, tempDeckp1Size, tempDeckp1);

        if (lost)
        {
            cout << endl;
            cout << HRED << " " + namep1 + ", you don't have enough cards to draw. Unfortunately, you lost this game." << RESET << endl;
            cout << HBLUE << " " + namep2 + ", you won this game! You won 100 points." << RESET << endl;
            pointsp2 += 100;
            cout << endl;
            break;
        }

        //player 2
        drawCards(deckp2Size, discardp2Size, deckp2, discardp2, tempDeckp2Size, tempDeckp2);

        if (lost)
        {
            cout << HBLUE << " " + namep2 + ", you don't have enough cards to draw. Unfortunately, you lost this game." << RESET << endl;
            break;
        }

        cout << "-- " + namep2 + ", please look away. --" << endl;
        cout << "-- " + namep1 + ", please press enter when you've come to the screen. --" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        //print out the four cards being played currently
        cout << HRED << " " + namep1 + " , here are your cards for this round:"
             << RESET << endl;
        printDeck(tempDeckp1, 4);

        //allow player to rearrange the cards
        arrCards(orderp1, tempDeckp1);

        cout << "-- " + namep1 + ", please look away. --" << endl;
        cout << "-- " + namep2 + ", please press enter when you've come to the screen. --" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        //print out the four cards being played currently
        cout << HBLUE << " " + namep2 + " , here are your cards for this round:"
             << RESET << endl;
        printDeck(tempDeckp2, 4);

        //allow player to rearrange the cards
        arrCards(orderp2, tempDeckp2);

        cout << endl;
        cout << "-- Both players can now look at the screen. Press enter to view how your cards battled: --" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        for (int i = 0; i < 4; i++)
        {
            cout << to_string(i + 1) + " - " << endl;
            cout << "    Player 1 played " + deck[orderp1[i]] + ". Player 2 played " + deck[orderp2[i]] + "." << endl;
            if (orderp1[i] < orderp2[i])
            {
                cout << "    ";
                cout << HBLUE << " Player 2 wins. " << RESET << endl;
                discardp2[discardp2Size] = orderp1[i];
                discardp2[discardp2Size + 1] = orderp2[i];
                discardp2Size += 2;
            }
            else
            {
                cout << "    ";
                cout << HRED << " Player 1 wins. " << RESET << endl;
                discardp1[discardp1Size] = orderp1[i];
                discardp1[discardp1Size + 1] = orderp2[i];
                discardp1Size += 2;
            }
        }

        cout << "\n\n-- Press enter to continue to the next round. --" << endl;
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    } while (!lost);
}

void startGame() {
    cout << "\n---- Please press enter to start game " + to_string(gameCount) + "! ----" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    //introduction
    cout << "♤ ♧ ♡ ♢ ♤ ♧ ♡" << endl;
    cout << "♤  -------  ♢   ";
    cout << HRED << " " + namep1 + ": " + to_string(pointsp1) + " points " << RESET << endl;
    cout << BOLDWHITE << "♤  GAME #" + to_string(gameCount) +"  ♢" << RESET << endl;
    cout << "♤  -------  ♢   ";
    cout << HBLUE << " " + namep2 + ": " + to_string(pointsp2) + " points " << RESET << endl;
    cout << "♤ ♧ ♡ ♢ ♤ ♧ ♡" << endl;

    //initialize the game
    deckp1Size = 0;
    deckp2Size = 0;
    tempDeckp1Size = 0;
    tempDeckp2Size = 0;
    discardp1Size = 0;
    discardp2Size = 0;

    //constants for faces and suits
    const string FACES[] = {"Deuce", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten", "Jack", "Queen", "King", "Ace"};
    const string SUITS[] = {"Spades", "Hearts", "Diamonds", "Clubs"};

    //generate the full deck in order
    int rowCount = 0;
    for (int j = 0; j < 13; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            deck[rowCount] = FACES[j] + " of " + SUITS[i];
            rowCount++;
        }
    }

    //generate a playing deck with indices that refers back to the full deck
    for (int i = 0; i < 52; i++)
    {
        playDeck[i] = i;
    }

    // //shuffle the overall deck
    shuffle(playDeck, 52);

    //fill the decks first with an invalid value, -1
    fill_n(deckp1, 52, -1);
    fill_n(deckp2, 52, -1);
    fill_n(discardp1, 52, -1);
    fill_n(discardp2, 52, -1);

    //divide deck and split to two players
    for (int x = 0; x < 26; x++)
    {
        deckp1[x] = playDeck[x];
    }

    for (int x = 0; x < 26; x++)
    {
        deckp2[x] = playDeck[x + 25];
    }

    deckp1Size = 26;
    deckp2Size = 26;
    lost = false;

    startRound();
    cout << "Would you like to play another round? Please indicate YES or NO." << endl;
    string playAgain;
    getline(cin, playAgain);
    if (playAgain.find("yes") != string::npos || playAgain.find("YES") != string::npos)
    {
        cout << endl;
        cout << "Enjoy!" << endl;
        gameCount++;
        startGame();
    }
    else
    {
        cout << BOLDWHITE << "FINAL SCORES" << RESET << endl;
        cout << "-------------" << endl;
        cout << HRED << " " + namep1 + ": " + to_string(pointsp1) + " points " << RESET << endl;
        cout << HBLUE << " " + namep2 + ": " + to_string(pointsp2) + " points " << RESET << endl;
        cout << "\nBye :) Have a great rest of your day!" << endl;
    }
}

int main()
{
    pointsp1 = 0;
    pointsp2 = 0;
    gameCount = 1;

    // introduce the game with rules & player names
    cout << BOLDYELLOW << "Hint for Mr. Hudson: \nIf you would like to win the game quickly to see how the losing conditions & point systems work, you can comment out 'shuffle(playDeck, 52);' on line 371 :)\nIn that case, player 2 will win automatically after only a few rounds.\n\n" << RESET << endl;
    cout << "♤ ♧ ♡ ♢ ♤ ♧ ♡ ♢ ♤ ♧ ♡ ♢ ♡" << endl;
    cout << "♤  -------------------  ♢" << endl;
    cout << BOLDWHITE << "♤  Welcome to War Game  ♢" << RESET << endl;
    cout << "♤  -------------------  ♢" << endl;
    cout << "♤ ♧ ♡ ♢ ♤ ♧ ♡ ♢ ♤ ♧ ♡ ♢ ♡" << endl;
    cout << endl;

    //rules
    cout << "\n---- Please press enter to continue. ----" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << BOLDWHITE << "INSTRUCTIONS FOR WAR GAME"
         << RESET << endl;
    cout << "-------------------------\n"
    "In War Game, you will compete with an opponent through battles of cards. \n"
    "Two players are needed to play the War Game. Get a friend, and prepare for battle :)\n\n";
    cout << "At the start of each game, a full deck of 52 cards will be split evenly to both of you. \n"
            "Each round, the top four cards from your deck will be revealed to you.\n"
            "You will have the option to rearrange these cards by indicating the order you'd like to play your cards through permutations. For example, 1234.\n"
            "After both players have selected the order they wish to play their cards, your cards will be compared one by one by size. \n";

    cout << "\n---- Please press enter to continue. ----" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "Cards with a larger face value will win over lower face values. The order of faces is as follows:\n"
            "    -- Deuce < Three < Four < Five < Six < Seven < Eight < Nine < Ten < Jack < Queen < King < Ace --\n"
            "If the cards have the same face value, their suits will be compared. The order of suits is as follows:\n"
            "    -- Spades < Hearts < Diamonds < Clubs --\n"
            "For every card that you win, the winning card you've played and the losing card your opponent played will both return to your discard pile.\n"
            "Your inputs will be hidden in black text -- Please do not look at the order of play that your opponent has inputed!" << endl;

    cout << "\n---- Please press enter to continue. ----" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << "If you don't have enough cards in your playing deck to draw four cards and begin a new round, your discard pile will be added back to your playing deck and shuffled.\n"
            "If you still don't have enough cards across your playing deck and your discard pile to draw four cards, you would lose the game.\n"
            "Of course, you can rematch as many times as you want after each game.\n"
            "Winning each game will score you 100 points. See which of you can win the most points!\n"
            "That's all for rules... now let's begin the game!" << endl;

    cout << "\n---- Please press enter to start. ----" << endl;
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    cout << HRED << " Player 1, please enter your name. " << RESET << endl;
    getline(cin, namep1);
    cout << "Welcome to War Game, ";
    cout << HRED << " " + namep1 + " "
        << RESET;

    cout << "!\n" << endl;

    cout << HBLUE << " Player 2, please enter your name. " << RESET << endl;
    getline(cin, namep2);
    cout << "Welcome to War Game, ";
    cout << HBLUE << " " + namep2 + " " << RESET;

    cout << "!" << endl;

    startGame();

    return 0;
}
