// rats.cpp

// Portions you are to complete are marked with a TODO: comment.
// We've provided some incorrect return statements (so indicated) just
// to allow this skeleton program to compile and run, albeit incorrectly.
// The first thing you probably want to do is implement the utterly trivial
// functions (marked TRIVIAL).  Then get Arena::display going.  That gives
// you more flexibility in the order you tackle the rest of the functionality.
// As you finish implementing each TODO: item, remove its TODO: comment.

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
#include <cassert>
using namespace std;

int main();


///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;            // max number of rows in the arena
const int MAXCOLS = 20;            // max number of columns in the arena
const int MAXRATS = 100;           // max number of rats allowed

const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NUMDIRS = 4;

const int EMPTY = 0;
const int HAS_POISON = 1;




///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;  // This is needed to let the compiler know that Arena is a
			  // type name, since it's mentioned in the Rat declaration.

class Rat
{
public:
	// Constructor
	Rat(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	void move();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool	 m_poisoned;
	bool	m_movedLastRound;
	bool m_isDead;
};

class Player
{
public:
	// Constructor
	Player(Arena* ap, int r, int c);

	// Accessors
	int  row() const;
	int  col() const;
	bool isDead() const;

	// Mutators
	string dropPoisonPellet();
	string move(int dir);
	void   setDead();

private:
	Arena* m_arena;
	int    m_row;
	int    m_col;
	bool   m_dead;
};

class Arena
{
public:
	// Constructor/destructor
	Arena(int nRows, int nCols);
	~Arena();

	// Accessors
	int     rows() const;
	int     cols() const;
	Player* player() const;
	int     ratCount() const;
	int     getCellStatus(int r, int c) const;
	int     numberOfRatsAt(int r, int c) const;
	void    display(string msg) const;

	// Mutators
	void setCellStatus(int r, int c, int status);
	bool addRat(int r, int c);
	bool addPlayer(int r, int c);
	void moveRats();

private:
	int     m_grid[MAXROWS][MAXCOLS];
	int     m_rows;
	int     m_cols;
	Player* m_player;
	Rat*    m_rats[MAXRATS];
	int     m_nRats;
	int     m_turns;

	// Helper functions
	void checkPos(int r, int c) const;
	bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
	// Constructor/destructor
	Game(int rows, int cols, int nRats);
	~Game();

	// Mutators
	void play();

private:
	Arena* m_arena;

	// Helper functions
	string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int randInt(int min, int max);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rat implementation
///////////////////////////////////////////////////////////////////////////

Rat::Rat(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** A rat must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "***** Rat created with invalid coordinates (" << r << ","
			<< c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_poisoned = false;
	m_movedLastRound = true;
	m_isDead = false;
}

int Rat::row() const
{
	return m_row;
}

int Rat::col() const
{
	//Return what column the Rat is at
	return m_col;
}

bool Rat::isDead() const
{
	return m_isDead;
}

void Rat::move()
{
	//   Return without moving if the rat has eaten one poison pellet (so
	//   is supposed to move only every other turn) and this is a turn it
	//   does not move.
	if (m_poisoned && m_movedLastRound) {
		m_movedLastRound = false;
		return;
	}
	m_movedLastRound = true;
	//   Otherwise, attempt to move in a random direction; if it can't
	//   move, don't move.  If it lands on a poison pellet, eat the pellet,
	//   so it is no longer on that grid point.
	int dir = randInt(0, 3);
	attemptMove(*m_arena, dir, m_row, m_col);
	if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON) {
		m_arena->setCellStatus(m_row, m_col, EMPTY);
		if (m_poisoned) m_isDead = true;
		else m_poisoned = true;
	}


}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
	if (ap == nullptr)
	{
		cout << "***** The player must be created in some Arena!" << endl;
		exit(1);
	}
	if (r < 1 || r > ap->rows() || c < 1 || c > ap->cols())
	{
		cout << "**** Player created with invalid coordinates (" << r
			<< "," << c << ")!" << endl;
		exit(1);
	}
	m_arena = ap;
	m_row = r;
	m_col = c;
	m_dead = false;
}

int Player::row() const
{
	//  Return what row the Player is at
	return m_row;
}

int Player::col() const
{
	// Return what column the Player is at
	return m_col;
}

string Player::dropPoisonPellet()
{
	if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
		return "There's already a poison pellet at this spot.";
	m_arena->setCellStatus(m_row, m_col, HAS_POISON);
	return "A poison pellet has been dropped.";
}

string Player::move(int dir)
{
	//		  Attempt to move the player one step in the indicated
	//        direction.  If this fails,
	//        return "Player couldn't move; player stands."
	//        A player who moves onto a rat dies, and this
	//        returns "Player walked into a rat and died."
	//        Otherwise, return one of "Player moved north.",
	//        "Player moved east.", "Player moved south.", or
	//        "Player moved west."

	// const int NORTH = 0;
	// const int EAST = 1;
	// const int SOUTH = 2;
	// const int WEST = 3;

	//couldn't move message
	const string couldntMove = "Player couldn't move; player stands.";


	//Attempt to move
	if (dir < 0 || dir > 3) return couldntMove; //invalid dir

	string directionMoved = "";
	switch (dir) {
	case NORTH:
		if (m_row == 1) return couldntMove;
		else m_row--;
		directionMoved = "north";
		break;
	case EAST:
		if (m_col == (m_arena->cols)()) return couldntMove;
		else m_col++;
		directionMoved = "east";
		break;
	case SOUTH:
		if (m_row == (m_arena->rows)()) return couldntMove;
		else m_row++;
		directionMoved = "south";
		break;
	case WEST:
		if (m_col == 1) return couldntMove;
		else m_col--;
		directionMoved = "west";
		break;
	}

	//Walks into rat 
	if ((m_arena->numberOfRatsAt)(m_row, m_col) != 0) { 
		setDead(); //kill player
		return "Player walked into a rat and died."; 
	}

	
	return "Player moved "+directionMoved+".";

}

bool Player::isDead() const
{
	return m_dead; 
}

void Player::setDead()
{
	m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
	if (nRows <= 0 || nCols <= 0 || nRows > MAXROWS || nCols > MAXCOLS)
	{
		cout << "***** Arena created with invalid size " << nRows << " by "
			<< nCols << "!" << endl;
		exit(1);
	}
	m_rows = nRows;
	m_cols = nCols;
	m_player = nullptr;
	m_nRats = 0;
	m_turns = 0;
	for (int r = 1; r <= m_rows; r++)
		for (int c = 1; c <= m_cols; c++)
			setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
	// Deallocate the player and all remaining dynamically allocated rats
	delete m_player;

	for (int i = 0; i < m_nRats; i++) {
		delete m_rats[i];
	
	}

}

int Arena::rows() const
{
	return m_rows;
}

int Arena::cols() const
{
	return m_cols;
}

Player* Arena::player() const
{
	return m_player;
}

int Arena::ratCount() const
{
	return m_nRats;
}

int Arena::getCellStatus(int r, int c) const
{
	checkPos(r, c);
	return m_grid[r - 1][c - 1];
}

int Arena::numberOfRatsAt(int r, int c) const
{
//	if (!isPosInBounds(r, c)) return 0; 
//not nessecary since r and c being out of bounds does not produce
//undnefined behavior; 
//no rat in m_rats will have col = c and row = r if 
//either c or r is out of bounds, and so 0 is returned anyway


	int count = 0;
	for (int i = 0; i < m_nRats; i++) {
		if( !((m_rats[i]->isDead)())  &&		
			((m_rats[i]->col)() == c && (m_rats[i]->row)() == r) ) 
				count++;
	}
	return count; 
}

void Arena::display(string msg) const
{
	char displayGrid[MAXROWS][MAXCOLS];
	int r, c;

	// Fill displayGrid with dots (empty) and stars (poison pellets)
	for (r = 1; r <= rows(); r++)
		for (c = 1; c <= cols(); c++) {
			displayGrid[r - 1][c - 1] = (getCellStatus(r, c) == EMPTY ? '.' : '*');

			//Indicate each rat's position
			int ratCount = numberOfRatsAt(r, c);
			if (ratCount == 1) displayGrid[r - 1][c - 1] = 'R';
			else if (ratCount > 1 && ratCount < 9) displayGrid[r - 1][c - 1] = '0' + ratCount;
			else if (ratCount >= 9) displayGrid[r - 1][c - 1] = '9';

		}


	// Indicate player's position
	if (m_player != nullptr)
		displayGrid[m_player->row() - 1][m_player->col() - 1] = (m_player->isDead() ? 'X' : '@');

	// Draw the grid
	clearScreen();
	for (r = 1; r <= rows(); r++)
	{
		for (c = 1; c <= cols(); c++)
			cout << displayGrid[r - 1][c - 1];
		cout << endl;
	}
	cout << endl;

	// Write message, rat, and player info
	if (msg != "")
		cout << msg << endl;
	cout << "There are " << ratCount() << " rats remaining." << endl;
	if (m_player == nullptr)
		cout << "There is no player!" << endl;
	else if (m_player->isDead())
		cout << "The player is dead." << endl;
	cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
	checkPos(r, c);
	m_grid[r - 1][c - 1] = status;
}

bool Arena::addRat(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a rat on a spot with a poison pellet
	if (getCellStatus(r, c) != EMPTY)
		return false;

	// Don't add a rat on a spot with a player
	if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
		return false;

	// If there are MAXRATS existing rats, return false.  
	if (m_nRats == MAXRATS) 
		return false;

	// Dynamically allocate a new rat at coordinates (r,c).  Save the
	// pointer to the newly allocated rat and return true.
	m_rats[m_nRats] = new Rat(this,r,c);
	

	m_nRats++;
	return true; 
}

bool Arena::addPlayer(int r, int c)
{
	if (!isPosInBounds(r, c))
		return false;

	// Don't add a player if one already exists
	if (m_player != nullptr)
		return false;

	// Don't add a player on a spot with a rat
	if (numberOfRatsAt(r, c) > 0)
		return false;

	m_player = new Player(this, r, c);
	
	return true;
}

void Arena::moveRats()
{
	// Move all rats
	//		  Move each rat.  Mark the player as dead if necessary.
	//        Deallocate any dead dynamically allocated rat.

	for (int i = 0; i < m_nRats; i++) {
		//Move rat
		m_rats[i]->move();
		//Kill player?
		if ((m_rats[i]->col() == this->m_player->col()) && (m_rats[i]->row() == this->m_player->row()))
			m_player->setDead();
		
	}

	
	//shift rats over (overwriting dead rats)
	int deletedRats = 0;
	for(int k = 0; k<m_nRats; k++)
		if (m_rats[k]->isDead()) {
			//delete the rat
			delete m_rats[k]; 
		
			//shift all after back one
			for (int i = k + 1; i < m_nRats; i++) {
				m_rats[i - 1] = m_rats[i];
			}
			deletedRats++;
		}
	m_nRats-=deletedRats;

	// Another turn has been taken
	m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
	return (r >= 1 && r <= m_rows  &&  c >= 1 && c <= m_cols);
}

void Arena::checkPos(int r, int c) const
{
	if (r < 1 || r > m_rows || c < 1 || c > m_cols)
	{
		cout << "***** " << "Invalid arena position (" << r << ","
			<< c << ")" << endl;
		exit(1);
	}
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRats)
{
	if (nRats < 0)
	{
		cout << "***** Cannot create Game with negative number of rats!" << endl;
		exit(1);
	}
	if (nRats > MAXRATS)
	{
		cout << "***** Trying to create Game with " << nRats
			<< " rats; only " << MAXRATS << " are allowed!" << endl;
		exit(1);
	}
	if (rows == 1 && cols == 1 && nRats > 0)
	{
		cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
		exit(1);
	}

	// Create arena
	m_arena = new Arena(rows, cols);


	// Add player
	int rPlayer;
	int cPlayer;
	do
	{
		rPlayer = randInt(1, rows);
		cPlayer = randInt(1, cols);
	} while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
	m_arena->addPlayer(rPlayer, cPlayer);

	// Populate with rats
	while (nRats > 0)
	{
		int r = randInt(1, rows);
		int c = randInt(1, cols);
		if (r == rPlayer && c == cPlayer)
			continue;
		if(m_arena->addRat(r, c))
			nRats--;
	}
}

Game::~Game()
{
	delete m_arena;
}

string Game::takePlayerTurn()
{
	for (;;)
	{
		cout << "Your move (n/e/s/w/x or nothing): ";
		string playerMove;
		getline(cin, playerMove); 

		Player* player = m_arena->player();
		int dir;

		if (playerMove.size() == 0)
		{
			if (recommendMove(*m_arena, player->row(), player->col(), dir))
				return player->move(dir);
			else
				return player->dropPoisonPellet();
		}
		else if (playerMove.size() == 1)
		{
			if (tolower(playerMove[0]) == 'x')
				return player->dropPoisonPellet();
			else if (decodeDirection(playerMove[0], dir))
				return player->move(dir);
		}
		cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
	}
}

void Game::play()
{
	m_arena->display("");
	while (!m_arena->player()->isDead() && m_arena->ratCount() > 0)
	{
		string msg = takePlayerTurn();
		Player* player = m_arena->player();
		if (player->isDead())
		{
			cout << msg << endl;
			break;
		}
		m_arena->moveRats();
		m_arena->display(msg);
	}

	if (m_arena->player()->isDead()) {
		m_arena->display("");
		cout << "You lose." << endl;
	}
	else
		cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
	if (max < min)
		swap(max, min);
	static random_device rd;
	static mt19937 generator(rd());
	uniform_int_distribution<> distro(min, max);
	return distro(generator);
}

bool decodeDirection(char ch, int& dir)
{
	switch (tolower(ch))
	{
	default:  return false;
	case 'n': dir = NORTH; break;
	case 'e': dir = EAST;  break;
	case 's': dir = SOUTH; break;
	case 'w': dir = WEST;  break;
	}
	return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
	a.getCellStatus(r, c); //check r and c validity


	switch (dir) {
	case NORTH:
		if (r == 1) return false;
		else r--;
		break;
	case EAST:
		if (c == a.cols()) return false;
		else c++;
		break;
	case SOUTH:
		if (r == a.rows()) return false;
		else r++;
		break;
	case WEST:
		if (c == 1) return false;
		else c--;
		break;
	}
	return true;  
}

int adjacentRats(const Arena& a, int r, int c) {
	return
		a.numberOfRatsAt(r - 1, c)
		+ a.numberOfRatsAt(r + 1, c)
		+ a.numberOfRatsAt(r, c - 1)
		+ a.numberOfRatsAt(r, c + 1);
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poison pellet and not
// move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{	
	//Recommendation algorithm
		// If staying put runs the risk of a
		// rat possibly moving onto the player's location when the rats
		// move, yet moving in a particular direction puts the player in a
		// position that is safe when the rats move, then the chosen
		// action is to move to a safer location.  
		// Similarly, if staying put
		// is safe, but moving in certain directions puts the player in danger,
		// then the chosen action should not be to move in one of the
		// dangerous directions; instead, the player should stay put or move to
		// another safe position.  
		// In general, a position that may be moved to
		// by many rats is more dangerous than one that may be moved to by
		// few.
		//ADDITIONAL:
		// If two moves have the same number of adjacent rats, the one without poison yet is prioritized.

	a.getCellStatus(r, c); //check r and c validity

	const int initialR = r;
	const int initialC = c;
	
	int min = -1; //current direction with minimum rats; -1 is stay still
	int currentLow = adjacentRats(a, r, c);
	
	//Check North
	if (attemptMove(a, NORTH, r, c))
		/*and*/ if (a.numberOfRatsAt(r, c) == 0) {
			int numAdjacentRats = adjacentRats(a, r, c);
			//Extra intelligent thing: this checks if the current min has poison and if the potential move has = numAdjacentRats but no poison, it'll prioritize moving 
			if (numAdjacentRats < currentLow || (numAdjacentRats==currentLow && a.getCellStatus(initialR,initialC)== HAS_POISON && a.getCellStatus(r, c) == EMPTY) ) {
				min = NORTH;
				currentLow = adjacentRats(a, r, c);
			}
		}
	r = initialR;
	c = initialC;

	//Check East
	if (attemptMove(a, EAST, r, c))
		/*and*/ if (a.numberOfRatsAt(r, c) == 0) {
			int numAdjacentRats = adjacentRats(a, r, c);
			if (numAdjacentRats < currentLow || (numAdjacentRats == currentLow && a.getCellStatus(initialR, initialC) == HAS_POISON && a.getCellStatus(r, c) == EMPTY) ) {
				min = EAST;
				currentLow = adjacentRats(a, r, c);
			}
		}
	r = initialR;
	c = initialC;

	//Check South
	if (attemptMove(a, SOUTH, r, c))
		/*and*/ if (a.numberOfRatsAt(r, c) == 0) {
			int numAdjacentRats = adjacentRats(a, r, c);
			if (numAdjacentRats < currentLow || (numAdjacentRats == currentLow && a.getCellStatus(initialR, initialC) == HAS_POISON && a.getCellStatus(r, c) == EMPTY) ) {
				min = SOUTH;
				currentLow = adjacentRats(a, r, c);
			}
		}
	r = initialR;
	c = initialC;

	//Check West
	if (attemptMove(a, WEST, r, c))
		/*and*/ if (a.numberOfRatsAt(r, c) == 0) {
			int numAdjacentRats = adjacentRats(a, r, c);
			if (numAdjacentRats < currentLow || (numAdjacentRats == currentLow && a.getCellStatus(initialR, initialC) == HAS_POISON && a.getCellStatus(r, c) == EMPTY) ) {
				min = WEST;
				currentLow = adjacentRats(a, r, c);
			}
		}
	r = initialR;
	c = initialC;

	//stand still
	if (min == -1)
		return false;  
	else bestDir = min;

		return true;
				   // Your replacement implementation should do something intelligent.
				   // You don't have to be any smarter than the following, although
				   // you can if you want to be:  If staying put runs the risk of a
				   // rat possibly moving onto the player's location when the rats
				   // move, yet moving in a particular direction puts the player in a
				   // position that is safe when the rats move, then the chosen
				   // action is to move to a safer location.  Similarly, if staying put
				   // is safe, but moving in certain directions puts the player in danger,
				   // then the chosen action should not be to move in one of the
				   // dangerous directions; instead, the player should stay put or move to
				   // another safe position.  In general, a position that may be moved to
				   // by many rats is more dangerous than one that may be moved to by
				   // few.
				   //
				   // Unless you want to, you do not have to take into account that a
				   // rat might be poisoned and thus sometimes less dangerous than one
				   // that is not.  That requires a more sophisticated analysis that
				   // we're not asking you to do.


}

 

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
	COORD upperLeft = { 0, 0 };
	DWORD dwCharsWritten;
	FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
		&dwCharsWritten);
	SetConsoleCursorPosition(hConsole, upperLeft);
}



void doBasicTests();
///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
//	doBasicTests(); // Remove this line after completing test.
//	return 0;       // Remove this line after completing test.

	string replay = "r";

	while (replay == "r") {
		// Create a game
		// g(row,col,nRats);
		Game g(16, 16, 50);

		// Play the game
		g.play();
		string s;

		do {
			cout << "Enter any (non \\n) character to quit" << endl << "or enter r to play again" << endl;
			getline(cin, s);
	
		} while (s == "");
		replay = s;
		
	}
	


	return 0;

}

int main2() {
	Game g(16, 16, 90);

	// Play the game
	g.play();
	return 0;
}



#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
	static const char* term = getenv("TERM");
	if (term == nullptr || strcmp(term, "dumb") == 0)
		cout << endl;
	else
	{
		static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
		cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
	}
}

#endif	

//simple tests
#include <type_traits>


#define CHECKTYPE(c, f, r, a)  \
	static_assert(std::is_same<decltype(&c::f), r (c::*)a>::value, \
	   "FAILED: You changed the type of " #c "::" #f);  \
	[[gnu::unused]] r (c::* xxx##c##_##f) a = &c::f

void thisFunctionWillNeverBeCalled()
{
	// If the student deleted or changed the interfaces to the public
	// functions, this won't compile.  (This uses magic beyond the scope
	// of CS 31.)

	Rat r(static_cast<Arena*>(0), 1, 1);
	CHECKTYPE(Rat, row, int, () const);
	CHECKTYPE(Rat, col, int, () const);
	CHECKTYPE(Rat, isDead, bool, () const);
	CHECKTYPE(Rat, move, void, ());

	Player p(static_cast<Arena*>(0), 1, 1);
	CHECKTYPE(Player, row, int, () const);
	CHECKTYPE(Player, col, int, () const);
	CHECKTYPE(Player, isDead, bool, () const);
	CHECKTYPE(Player, dropPoisonPellet, string, ());
	CHECKTYPE(Player, move, string, (int));
	CHECKTYPE(Player, setDead, void, ());

	Arena a(1, 1);
	CHECKTYPE(Arena, rows, int, () const);
	CHECKTYPE(Arena, cols, int, () const);
	CHECKTYPE(Arena, player, Player*, () const);
	CHECKTYPE(Arena, ratCount, int, () const);
	CHECKTYPE(Arena, getCellStatus, int, (int, int) const);
	CHECKTYPE(Arena, numberOfRatsAt, int, (int, int) const);
	CHECKTYPE(Arena, display, void, (string) const);
	CHECKTYPE(Arena, setCellStatus, void, (int, int, int));
	CHECKTYPE(Arena, addRat, bool, (int, int));
	CHECKTYPE(Arena, addPlayer, bool, (int, int));
	CHECKTYPE(Arena, moveRats, void, ());

	Game g(1, 1, 1);
	CHECKTYPE(Game, play, void, ());
}

void findTheRat(const Arena& a, int& r, int& c)
{
	if (a.numberOfRatsAt(r - 1, c) == 1) r--;
	else if (a.numberOfRatsAt(r + 1, c) == 1) r++;
	else if (a.numberOfRatsAt(r, c - 1) == 1) c--;
	else if (a.numberOfRatsAt(r, c + 1) == 1) c++;
	else assert(false);
}

void doBasicTests()
{
	{
		Arena a(10, 20);
		a.addPlayer(2, 5);
		Player* pp = a.player();
		assert(pp->row() == 2 && pp->col() == 5 && !pp->isDead());
		assert(pp->move(NORTH) == "Player moved north.");
		assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
		assert(pp->move(NORTH) == "Player couldn't move; player stands.");
		assert(pp->row() == 1 && pp->col() == 5 && !pp->isDead());
		pp->setDead();
		assert(pp->row() == 1 && pp->col() == 5 && pp->isDead());
	}
	{
		Arena a(10, 20);
		int r = 4;
		int c = 4;
		a.setCellStatus(r - 1, c, HAS_POISON);
		a.setCellStatus(r + 1, c, HAS_POISON);
		a.setCellStatus(r, c - 1, HAS_POISON);
		a.setCellStatus(r, c + 1, HAS_POISON);
		a.addRat(r, c);
		a.addPlayer(8, 18);
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 1);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 0);
		findTheRat(a, r, c);
		assert(a.getCellStatus(r, c) != HAS_POISON);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 1);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 0);
		findTheRat(a, r, c);
		a.moveRats();
		assert(a.ratCount() == 1 && a.numberOfRatsAt(r, c) == 1);
		a.setCellStatus(r - 1, c, HAS_POISON);
		a.setCellStatus(r + 1, c, HAS_POISON);
		a.setCellStatus(r, c - 1, HAS_POISON);
		a.setCellStatus(r, c + 1, HAS_POISON);
		a.moveRats();
		assert(a.ratCount() == 0 && a.numberOfRatsAt(r, c) == 0);
		assert(a.numberOfRatsAt(r - 1, c) == 0);
		assert(a.numberOfRatsAt(r + 1, c) == 0);
		assert(a.numberOfRatsAt(r, c - 1) == 0);
		assert(a.numberOfRatsAt(r, c + 1) == 0);

		for (int k = 0; k < MAXRATS / 4; k++)
		{
			a.addRat(7, 18);
			a.addRat(9, 18);
			a.addRat(8, 17);
			a.addRat(8, 19);
		}
		assert(!a.player()->isDead());
		a.moveRats();
		assert(a.player()->isDead());
	}
	cout << "Passed all basic tests" << endl;
	system("PAUSE");
}
//