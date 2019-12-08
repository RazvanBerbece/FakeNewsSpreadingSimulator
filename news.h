// ******************************* VERY IMPORTANT *************************************************
// THE FOLLOWING DECLARATIONS SHOULD NOT BE DELETED OR CHANGED
// REMOVING OR CHANGING ANY OF THEM WILL STOP YOUR PROGRAM FROM
// COMPILING SUCCESSFULLY WITH THE TEST HARNESS OR MAKE YOUR PROGRAM FAIL THE TESTS
// PLEASE USE THEM FOR THE PURPOSE FOR WHICH THEY WERE CREATED
//*************************************************************************************************

#define MaxNameSize 8			// the maximum number of characters for an account name INCLUDING the trailing \0
#define MaxStoryLength 32		// the maximum number of characters for a story INCLUDING the trailing \0
#define MaxFriends 32			// the maximum number of friends for one account
#define MaxNumberAccounts 512	// the maximum number of accounts in the social network
#define MaxMessageArraySize 1000	// the maximum number of messages in the received or sent arrays
#define MaxNumberPendingMessages 10000	// the maximum number of messages pending transmission in the server
#define MaxReportedFakeStroies 100		// the maximum number of stories that can be reported
#define MaxAccountId 9999				// the maximum account id that can be reached

#define NumberConsonants 16				// the number of consonants in the Consonants array
#define NumberVowels 4					// the number of vowels in the Vowels array
#define NumberTrueStoryWords	10		// the number of words in the TrueStoryWords array

// Accounts' types according to their role in the network
#define RealNewsPublisher	1	// these are the people who publish real news
#define FakeNewsFabricator 2	// these are the evil guys who fabricate fake news
#define NaiveForwarder 3		// these are the naive people who always forward any story to their friends
#define FakeNewsSink 4			// these are the clever people who never forward a fake story but do not bother to report it
#define FakeNewsReporter 5		// these are the angels of the social network, they do not forward fake stories AND they report it

// this is the Message struct, it defines the fields of a message
typedef struct
{
	char theStory[MaxStoryLength];			// the story (payload) of this message
	struct AccountStruct* theSender;		// the sender of the message
	struct AccountStruct* theRecipient;		// the recipient of the message
	int isRead;								// a flag that is initially 0 but set to 1 if the message is read
} Message;


// this is the Account strcut, it defines the fields of an account in the social network
typedef struct AccountStruct
{
	int accId;										// a unique id for this account (because account names may be similar)
	char accName[MaxNameSize];						// the name of the account (must have the CVCV pattern)
	int accType;									// the type of the account (0-4)
	struct AccountStruct* Friends[MaxFriends];		// an array of pointers to the friends of this account
	int numFriends;									// the number of friends of this account
	Message receivedM[MaxMessageArraySize];			// an array for storing received messages, i.e. the inbox of this account
	int numberReceivedM;							// the number of messages in the receivedM array (an array for received messages)
	Message sentM[MaxMessageArraySize];				// the array of sent messages
	int numberSentM;								// the number of messages in the sentM array (an array for sent messages
} Account;

// this is the server structure
typedef struct
{
	Message pendingMessages [MaxNumberPendingMessages];	// stores messages waiting to be forwarded to recipients
	int numberPendingMessages; // number of messages waiting to be forwarded to recipients
	char* reportedFakeStories[MaxReportedFakeStroies]; // A list of stories that have been reported as fake news stories
	int numberReportedFakeSt; // the number reported fake news stories
} Server;

// these variables are defined in template.c
extern char Consonants[NumberConsonants];
extern char Vowels[NumberVowels];
extern Account* AllAccounts[MaxNumberAccounts];
extern int NumberAccounts;
extern char* TrueStoryWords[NumberTrueStoryWords];
extern Server theServer;
extern int MaxNewFakeStory;
extern int  MaxNewRealStory;

// fucntion prototypes
int initAll ();																		// already implemented
int newAccountId ();																// already implemented
char aConsonant ();																	// 2 marks
char aVowel ();																		// 2 marks
int isConsonant (char letter);														// 2 marks
int isVowel (char letter);															// 2 marks
char* newAccountName ();															// 3 marks
Account* newAccount (int account_number, char accout_name[], int account_type);		// 3 marks
int addAccount (Account* an_account);												// 3 marks
int addFriend (Account* an_account, Account* a_friend);								// 3 marks
int isFriend (Account *a , Account *b);												// 3 marks
char* aFakeStory ();																// 3 marks
int isFakeStory (char* story);														// 3 marks
char* aRealStory ();																// 3 marks
Message createMessage (char* a_story, Account* sender, Account* receiver);			// 3 marks
int uploadMessage (Message a_message);												// 3 marks
int pushMessage (Message a_message);												// 3 marks
int sendStory (char* a_story, Account* sender, Account* receiver);					// 4 marks
int isReported (char * story);														// 2 marks
int ReportFakeStory(char* story);													// 3 marks
int transmitAllMessages ();															// 5 marks
int isSent (Account* account, char* story);											// 3 marks
int createSocialNetwork (int num_publishers, int num_fabricators,
						 int num_forwarders, int num_sinks,
						 int num_reporters, int num_friends);						// 8 marks
int originateOrforwardNews (Account* account);										// 8 marks
int simulate ();																	// 6 marks
int CountFakeNews ();																// already implemented
int CountRealNews ();																// already implemented


// helper functions already implemented
int lisAllAccounts ();
int showFriendshipMatrix();
int showAllMailBoxes();