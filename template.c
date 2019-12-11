/*************************************************************************

University of Leeds

School of Computing

COMP1711- Procedural Programming

Coursework 3

I confirm that the following program has been developed and written by me and it is entirely the result of my own work. I also confirm that I have not copied any parts of this program from another person or any other source or facilitated someone to copy this program.

I confirm that I will not post this program online or share it with anyone before the end of the semester.

Student Name: Razvan-Antonio Berbece

Student ID: 201301759

Email: sc19rab@leeds.ac.uk

Date Work Commenced: 27.11.2019

*************************************************************************/


// ******************************* VERY IMPORTANT *************************************************
// THE FOLLOWING DECLARATIONS SHOULD NOT BE DELETED OR CHANGED
// REMOVING OR CHANGING ANY OF THEM WILL STOP YOUR PROGRAM FROM
// COMPILING SUCCESSFULLY WITH THE TEST HARNESS
// PLEASE USE THEM FOR THE PURPOSE FOR WHICH THEY WERE CREATED
//*************************************************************************************************

#include <stdlib.h>

#include <stdio.h>

#include <string.h>

#include<time.h>

#include "news.h"

int NumberAccounts; // the actual number of accounts in the social network
int AccountIdCounter; // a counter for creating unique account IDs

char Consonants[NumberConsonants] = {
  'B',
  'C',
  'D',
  'F',
  'G',
  'J',
  'K',
  'L',
  'M',
  'N',
  'P',
  'R',
  'S',
  'T',
  'V',
  'Z'
};
char Vowels[NumberVowels] = {
  'A',
  'I',
  'O',
  'U'
};
char * TrueStoryWords[NumberTrueStoryWords] = {
  "ELECTIONS",
  "BREXIT",
  "UK",
  "MINISTER",
  "DATE",
  "EDUCATION",
  "RESIGN",
  "MAYOR",
  "NEWS",
  "WIN"
};

Account * AllAccounts[MaxNumberAccounts]; // the array of all accounts in the network
Server theServer; // this is the server that receives messages from senders and transmit them forward to recipients

int MaxNewFakeStory; // the maximum number of times fabricators can create new fake stories
int MaxNewRealStory; // the maximum number of times real news publishers can create new real stories
int numberNewFakeStories; // the number of fake news stories fabricated so far
int numberNewRealStories; // the number of new real stories published so far

// Inititialise all variables to their initial values
int initAll() {
  NumberAccounts = 0;
  AccountIdCounter = 1;
  theServer.numberPendingMessages = 0;
  theServer.numberReportedFakeSt = 0;
  MaxNewFakeStory = 5;
  MaxNewRealStory = 5;
  numberNewFakeStories = 0;
  numberNewRealStories = 0;
  srand((unsigned int) time(0)); // seed the random number generator with the current time
  return 1;
}

// returns a new account id
// first time returns 1
// if no more IDs can be generated returns -1
int newAccountId() {
  int currentID = AccountIdCounter;
  if (AccountIdCounter < MaxAccountId) {
    AccountIdCounter++;
    return currentID;
  }
  return -1;
}

// returns a random consonant from the Consonants array
// my implementation is 2 lines
char aConsonant() {
  int randomIndex = rand() % (15 + 1 - 0) + 0;
  return Consonants[randomIndex];
}

// returns a random vowel from the Vowels array
// my implementation is 2 lines
char aVowel() {
  int randomIndex = rand() % (3 + 1 - 0) + 0;
  return Vowels[randomIndex];
}

// returns 1 if the letter is a consonant, i.e. is one of the letters in the Consonants array
// otherwise returns 0
// my implementation is 4 lines
int isConsonant(char letter) {
  if (letter != 'A' && letter != 'I' && letter != 'O' && letter != 'U')
    return 1;
  return 0;
}

// returns 1 if the letter is a vowel, i.e. is one of the letters in the Vowels array
// otherwise returns 0
// my implementation is 4 lines
int isVowel(char letter) {
  if (strchr(Vowels, letter))
    return 1;
  return 0;
}

// creates a new account name (i.e. a new string initialised to the name)
// returns a pointer to this string
// account names are exactly four letters long and have the following pattern CVCV
// where C is any consonant from the Consonant array, and V is any vowel from the Vowels array
// examples of valid account names TITO, SAMI, KILO, LOVE, NOBU ...
// examples of invalid account names KLMI (second letter is not a vowel), KUKUA (more than 4 letters), LAL (less than 4 letters)
// my implementation is 7 lines
char * newAccountName() {
  char * newName = (char * ) malloc(5 * sizeof(char));
  newName[0] = aConsonant();
  newName[1] = aVowel();
  newName[2] = aConsonant();
  newName[3] = aVowel();
  newName[4] = '\0';
  return newName;
}

// creates a new account 'object' in the heap and initialises all its fields to the correct values
// returns a pointer to the newly created account
// the account type is an integer between 1-5 corresponding to the five account types:
// my implementation is 8 lines
Account * newAccount(int account_number, char accout_name[], int account_type) {
  Account * newAccount = (Account * ) malloc(sizeof(Account));

  newAccount -> accId = account_number;
  strcpy(newAccount -> accName, accout_name);
  newAccount -> accType = account_type;
  newAccount -> numFriends = 0;
  newAccount -> numberReceivedM = 0;
  newAccount -> numberSentM = 0;

  return newAccount;
}

// adds an account (more precisely a pointer to the account) to the AllAccounts array
// if the array is full and cannot accept any more accounts the function returns -1
// otherwise the function returns 1
// my implementation is 5 lines
int addAccount(Account * an_account) {
  if (NumberAccounts >= MaxNumberAccounts)
    return -1;
  else {
    AllAccounts[NumberAccounts] = an_account;
    NumberAccounts++;
  }
  return 1;
}

// Makes an account (the first parameter) friend with another account (the second parameter)
// notice that friendship is mutual hence if LILI is friend with TALA then TALA is also friend with LILI
// if either account cannot accept any more friends (because the Friends array is full) the function returns -1
// otherwise the function returns 1
// my implementation is 7 lines
int addFriend(Account * an_account, Account * a_friend) {
  if (an_account -> numFriends >= MaxFriends || a_friend -> numFriends >= MaxFriends)
    return -1;

  an_account -> Friends[an_account -> numFriends] = a_friend;
  a_friend -> Friends[a_friend -> numFriends] = an_account;
  an_account -> numFriends += 1;
  a_friend -> numFriends += 1;

  return 1;
}

// returns 1 (true) if account a is friend with account b otherwise it returns 0 (false)
// my implementation is 4 lines
int isFriend(Account * a, Account * b) {
  for (int i = 0; i < (a -> numFriends); ++i)
    if (a -> Friends[i] == b)
      return 1;
  return 0;
}

// create a social network having
// num_publishers real news publishers
// num_fabricators fake news fabricators
// num_forwarders naive forwarders
// num_sinks fake story sinks
// num_reporters fake news reporters (people who report fake news to the server)
// each account then should be made a friend with another num_friends friends
// the friends are randomly picked
// make sure an account is not made a friend with itself, or made a friend with another account more than once
// my implementation is 44 lines
int createSocialNetwork(int num_publishers, int num_fabricators, int num_forwarders, int num_sinks, int num_reporters, int num_friends) {
  int totalAccounts = num_publishers + num_fabricators + num_forwarders + num_sinks + num_reporters;

  while (num_publishers) {
    addAccount(newAccount(newAccountId(), newAccountName(), 1));
    num_publishers--;
  }
  while (num_fabricators) {
    addAccount(newAccount(newAccountId(), newAccountName(), 2));
    num_fabricators--;
  }
  while (num_forwarders) {
    addAccount(newAccount(newAccountId(), newAccountName(), 3));
    num_forwarders--;
  }
  while (num_sinks) {
    addAccount(newAccount(newAccountId(), newAccountName(), 4));
    num_sinks--;
  }
  while (num_reporters) {
    addAccount(newAccount(newAccountId(), newAccountName(), 5));
    num_reporters--;
  }

  for (int i = 0; i < totalAccounts; ++i) {
    int makeFriendsWith = num_friends;
    while (makeFriendsWith) {
      int randomFriendIndex = rand() % totalAccounts;
      if (!isFriend(AllAccounts[i], AllAccounts[randomFriendIndex]) && (i != randomFriendIndex)) {
        addFriend(AllAccounts[i], AllAccounts[randomFriendIndex]);
        makeFriendsWith--;
      }
    }
  }
  return 1;
}

// creates a new fake story and returns a pointer to the newly created string
// a fake story is comprised of execty two 3-letter words separated by single white space and has the following pattern CVC CVC, for example RIS PUZ or DAR MUC
// where C is one of the consonants in the Consonants array and V is one of the vowels in the Vowels array
// my implementation is 10 lines
char * aFakeStory() {
  char * fakeStory = (char * ) malloc(8 * sizeof(char));;
  fakeStory[0] = aConsonant();
  fakeStory[1] = aVowel();
  fakeStory[2] = aConsonant();
  fakeStory[3] = ' ';
  fakeStory[4] = aConsonant();
  fakeStory[5] = aVowel();
  fakeStory[6] = aConsonant();
  fakeStory[7] = '\0';
  return fakeStory;
}

// returns 1 if story is a fake story
// otherwise retunrs 0
// my implementation is 10 lines
int isFakeStory(char * story) {
  if (story[3] != ' ' || strlen(story) > 7) return 0;
  else {
    int len = strlen(story);
    for (int i = 0; i < len; ++i) {
      if (i == 3) continue;
      else if (i % 2 == 0 && !isConsonant(story[i])) return 0;
      else if (i % 2 != 0 && !isVowel(story[i])) return 0;
    }
  }
  return 1;
}

// creates a new real story.
// A real story is comprised of two DIFFERENT words randomly taken from the TrueStoryWords array
// Hence, if the first randomly picked word is MAYOR the second one cannot be MAYOR
// Exmaples of correctly formed true stories: BREXIT UK, MINISTER DATE, UK WIN, UK MINISTER, DATE WIN, ...
// Exmaples of incorrectly formed stories include: DATE DATE (same word), BREXIT (one word),  UK WIN MAYOR (more than 2 words)
// the function returns a pointer to the newly created string
// my implementation is 9 lines
char * aRealStory() {
  char * returnedStory = (char * ) malloc(20 * sizeof(char));
  char * space = " ";
  int randomWord_1 = rand() % (9 + 1 - 0) + 0;
  int randomWord_2 = rand() % (9 + 1 - 0) + 0;
  while (randomWord_2 == randomWord_1)
    randomWord_2 = rand() % (9 + 1 - 0) + 0;
  strcat(returnedStory, TrueStoryWords[randomWord_1]);
  strcat(returnedStory, space);
  strcat(returnedStory, TrueStoryWords[randomWord_2]);

  return returnedStory;
}

// create a new message to encapsulate a story to be sent from sender to recipient
// and returns the newly created message
// my implementation is 6 lines
Message createMessage(char * a_story, Account * sender, Account * recipient) {
  Message newMessage;
  strcpy(newMessage.theStory, a_story);
  newMessage.theSender = sender;
  newMessage.theRecipient = recipient;
  newMessage.isRead = 0;

  return newMessage;
}

// upload a message to the server, i.e. add the message to the server's pendingMessages array
// returns -1 if the pendingMessages array is full
// otherwise returns 1
// my implementation is 5 lines
int uploadMessage(Message a_message) {
  if (theServer.numberPendingMessages >= MaxNumberPendingMessages)
    return -1;
  else {
    theServer.pendingMessages[theServer.numberPendingMessages] = a_message;
    theServer.numberPendingMessages++;
  }
  return 1;
}

// push a message to the recipient's inbox, i.e. add the message to the recipient's receivedM array
// returns -1 if the recipient's receivedM array is full
// otherwise returns 1
// my implementation is 5 lines
int pushMessage(Message a_message) {
  Account * recipient = a_message.theRecipient;
  if (recipient -> numberReceivedM >= MaxMessageArraySize)
    return -1;
  else {
    recipient -> receivedM[recipient -> numberReceivedM] = a_message;
    recipient -> numberReceivedM += 1;
  }
  return 1;
}

// send a story from sender to recipient by:
// 1. creaing a message for the story
// 2. uploading the message to the server
// 3. adding the message to the sentM array of the sender so that the sender knows that it has been sent
// returns -1 if something goes wrong, e.g. upload message to server failed or the sender's sentM array is full
// otherwise returns 1
// my implementation is 9 lines
int sendStory(char * a_story, Account * sender, Account * recipient) {
  Message newMessage = createMessage(a_story, sender, recipient);
  if (uploadMessage(newMessage) == 1) {
    if (sender -> numberSentM >= MaxMessageArraySize)
      return -1;
    else {
      sender -> sentM[sender -> numberSentM] = newMessage;
      sender -> numberSentM += 1;
      return 1;
    }
  }
  return -1;
}

// returns 1 (true) if the story has been reported, i.e. it exists in the server's reportedFakeStories array
// otherwise it returns 0
// my implementation is 4 lines
int isReported(char * story) {
  for (int i = 0; i < theServer.numberReportedFakeSt; ++i)
    if (strcmp(theServer.reportedFakeStories[i], story) == 0)
      return 1;
  return 0;
}

// report a fake story, i.e. add the story to the server's reportedFakeStories array
// if the story has already been reported (it does exist in the reportedFakeStories array) the function should not
// add it again
// returns -1 if the reportedFakeStories array is full
// otherwise it returns 1
// my implementation is 8 lines
int ReportFakeStory(char * story) {
  if (isReported(story) || theServer.numberReportedFakeSt >= MaxReportedFakeStroies)
    return -1;
  else {
    theServer.reportedFakeStories[theServer.numberReportedFakeSt] = story;
    theServer.numberReportedFakeSt += 1;
    return 1;
  }
}

// transmit all the messages in the server's pendingMessages array to their respetive recipient's
// by adding them to the recipient's inbox
// if a message has been reported as fake (it exists in the reportedFakeStories array), it should not be transmitted
// my implementation is 9 lines
int transmitAllMessages() {
  int len = theServer.numberPendingMessages;
  for (int i = 0; i < len; ++i)
    if (isReported((theServer.pendingMessages[i]).theStory))
      continue;
    else pushMessage(theServer.pendingMessages[i]);
  theServer.numberPendingMessages = 0;
  return 1;
}

// seraches for a story in the account's sentM array
// returns 1 if found
// 0 if not found
// my implementation is 4 lines
int isSent(Account * account, char * story) {
  for (int i = 0; i < account -> numberSentM; ++i)
    if (strcmp(account -> sentM[i].theStory, story) == 0)
      return 1;
  return 0;
}

// for RealNewsPublisher and FakeNewsFabricator, this function allows the account to create a new story
// (unless the maximum number of new stories is reach) and send the story to all friends
// for all other account types the function will
// go through all messages in the inbox of account and for each message that has not been read:
// set the isRead flag is 1
// decide if the story must be forwarded to all friends or not (the decision depends on the account type)
// decide if the story must be reportd as fake (the decision depends on the account type) and report it
// note that if a story has already been sent to friends (because it exists in the sentM array) it should not be sent again
// the function should return the number of sent messages
// my implementation is 55 lines
int originateOrforwardNews(Account * account) {
  int sentMessages = 0;

  if (account -> accType == 1)
    if (numberNewRealStories < MaxNewRealStory) {
      char * newStory = aRealStory();
      while(isSent(account, newStory))
      {
	strcpy(newStory, aRealStory());
      }
        for (int i = 0; i < account -> numFriends; ++i) 
	{
          sendStory(newStory, account, account -> Friends[i]);
          sentMessages++;
        }
	numberNewRealStories++;
    }
  else if (account -> accType == 2)
    if (numberNewFakeStories < MaxNewFakeStory) {
      char * newStory = aFakeStory();
      while(isSent(account, newStory)) 
      {
	strcpy(newStory, aFakeStory());
      }
        for (int i = 0; i < account -> numFriends; ++i) 
	{
          sendStory(newStory, account, account -> Friends[i]);
          sentMessages++;
        }
	numberNewFakeStories++;
    }
  else if (account -> accType == 3) {
    for (int i = 0; i < account -> numberReceivedM; ++i) {
      if (account -> receivedM[i].isRead = 0) {
        account -> receivedM[i].isRead = 1;
        if (!isSent(account, account -> receivedM[i].theStory))
          for (int j = 0; j < account -> numFriends; ++j) {
            sendStory(account -> receivedM[i].theStory, account, account -> Friends[i]);
            sentMessages++;
          }
      } else continue;
    }
  } else if (account -> accType == 4) {
    for (int i = 0; i < account -> numberReceivedM; ++i) {
      if (account -> receivedM[i].isRead = 0) {
        account -> receivedM[i].isRead = 1;
        if (!isSent(account, account -> receivedM[i].theStory) && !isFakeStory(account -> receivedM[i].theStory))
          for (int j = 0; j < account -> numFriends; ++j) {
            sendStory(account -> receivedM[i].theStory, account, account -> Friends[i]);
            sentMessages++;
          }
      } else continue;
    }
  } else if (account -> accType == 5) {

    for (int i = 0; i < account -> numberReceivedM; ++i) {
      if (account -> receivedM[i].isRead = 0) {
        account -> receivedM[i].isRead = 1;
        if (!isSent(account, account -> receivedM[i].theStory) && !isFakeStory(account -> receivedM[i].theStory))
          for (int j = 0; j < account -> numFriends; ++j) {
            sendStory(account -> receivedM[i].theStory, account, account -> Friends[i]);
            sentMessages++;
          }
        else if (isFakeStory(account -> receivedM[i].theStory))
          ReportFakeStory(account -> receivedM[i].theStory);
      }
    }
  }

  return sentMessages;
}

// this is the function that simulates the propagation of news throughout the network
// the pseudocode of the function is in the problem description
// always returns 1
// my implementation is 13 lines
int simulate() {
  do {
    for (int i = 0; i < NumberAccounts; ++i)
      originateOrforwardNews(AllAccounts[i]);
    transmitAllMessages();
  } while (theServer.numberPendingMessages);
  return 1;
}

// The following functions have already been implmented
// you may wish

// this function counts the number of fake news stories in every inbox of every account in the newtwork
int CountFakeNews() {
  int countfake = 0;
  for (int i = 0; i < NumberAccounts; i++)
    for (int j = 0; j < AllAccounts[i] -> numberReceivedM; j++)
      if (isFakeStory(AllAccounts[i] -> receivedM[j].theStory))
        countfake++;
  return countfake;
}

// this function counts the number of real news stories in every inbox of every account in the newtwork
int CountRealNews() {
  int count = 0;
  for (int i = 0; i < NumberAccounts; i++)
    for (int j = 0; j < AllAccounts[i] -> numberReceivedM; j++)
      if (isFakeStory(AllAccounts[i] -> receivedM[j].theStory) == 0)
        count++;
  return count;
}

// displays all the accounts on the terminal
// always returns 1
int lisAllAccounts() {
  printf("ID\tName\tClass.\tNo.Friends\tFriends\n");
  for (int i = 0; i < NumberAccounts; i++) {
    printf("%i\t", AllAccounts[i] -> accId);
    printf("%s\t", AllAccounts[i] -> accName);
    printf("%i\t", AllAccounts[i] -> accType);
    printf("%i\t", AllAccounts[i] -> numFriends);
    for (int j = 0; j < AllAccounts[i] -> numFriends; j++)
      printf("%s,", AllAccounts[i] -> Friends[j] -> accName);
    printf("\n");
  }
  return 1;
}

// displays the social network as a friendhsip matrix
// always returns 1
int showFriendshipMatrix() {
  printf("The friendship matrix\n");
  printf("     ");
  for (int i = 0; i < NumberAccounts; i++) {
    Account * this_account = AllAccounts[i];
    printf("%s ", this_account -> accName);
  }
  printf("\n");
  for (int i = 0; i < NumberAccounts; i++) {
    Account * this_account = AllAccounts[i];
    printf("%s ", this_account -> accName);
    for (int f = 0; f < NumberAccounts; f++) {
      if (isFriend(this_account, AllAccounts[f]))
        printf("  f  ");
      else
        printf("  n  ");
    }
    printf("\n");
  }
  return 1;
}
