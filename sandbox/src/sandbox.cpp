#include <susumu.h>

class Sandbox : public susumu::Game
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

susumu::Game* susumu::CreateGame()
{
	return new Sandbox();
}