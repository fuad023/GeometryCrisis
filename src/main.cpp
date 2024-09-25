
#include "GameEngine.h"

int main(int argc, char* argv[])
{
    GameEngine game(argc, argv);
    game.init("config.txt");
    game.run();

}