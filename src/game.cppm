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

class Game {
	// We friend the main function so that it has exclusive access to the
	// Game::loop call.
	friend int main(int argc, char const *argv[]);

	/**
	 * @brief      Defines the main program loop; refreshes the state of the
	 *             Game on each call; called once per frame
	 *
	 * @param[in]  body  The body of the loop
	 */
	static void loop(std::function<void()> body) {
		while (gameActive__) {
			Internal::refresh();

			body();
		}
	}

 public:
 	/**
 	 * @brief      Sends the signal to end the Game at the start of the next
 	 *             frame.
 	 */
 	static void end(void) {
 		Internal::end();
 		gameActive__ = 0;
 	}
};
