/*
 * TODO:
 * 
 * - Create "master" module and make rest interface partitions
 * 	 - Make them communicate "behind the scenes"
 */

// Copyright 2025 Caleb Whitmer
import std;
import cakeFramework;

class TestComp final : public Component {
	void start() {
		std::cout << "Start TestComp!" << std::endl;
	}
};

int main(int argc, char const *argv[])
{
	Entity a;
	a.addComponent<TestComp>();

	return 0;
}