#include <string>
#include <functional>

#include <susumu.h>

class Sandbox : public susumu::App
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};

susumu::App* susumu::CreateApp()
{
	return new Sandbox();
}