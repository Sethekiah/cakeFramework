/* TODO:
 *
 * fix visibility issue with Game::loop. Currently it is visible to any file
 * which imports the game module. This is an issue as any Component could in
 * theory call the game loop which would absolutely screw up the flow of events
 * and probably cause a game crash.
 */

// Copyright 2025 Caleb Whitmer
module;

// Forward declaration of main class so as to allow it friend access
int main(int argc, char const *argv[]);

export module cakeFramework:game;
import :internal;
import std;

export class Game;

// Private members are called out here since the main function has access to the
// private section of the Game class.
bool gameActive__ = 1;

/**
 * \ingroup System
 * @brief      Provides the interface to control the state of the game
 */
class Game final {
	// Does not work anymore - gotta fix this for now I have made it public
	// 
	// We friend the main function so that it has exclusive access to the
	// Game::loop call.
	// friend int main(int argc, char const *argv[]);

 protected:


 public:
	/**
	 * @brief      Provides the means of defining the game loop. Will update all
	 *             Entity and Component instances on each frame
	 *
	 *             Outdated - "Provides the means of defining the game loop and
	 *             is only visible to the main function; Will update all Entity
	 *             and Component instances on each frame"
	 *
	 * @param[in]  body  The body of the game loop
	 */
	static void loop(std::function<void()> body) {
		while (gameActive__) {
			Internal::refresh();

			body();
		}
	}

 	/**
 	 * @brief      Sends the signal to end the Game at the start of the next
 	 *             frame
 	 */
 	static void end(void) {
 		Internal::end();
 		gameActive__ = 0;
 	}
};
