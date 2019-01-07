#include "Ridge.h"

#include <iostream>

using namespace std;
using namespace Ridge;

class Sandbox : public Application
{
public:
	Sandbox() { }
	~Sandbox() { }
	void Init() override
	{
		RIDGE_INFO("Welcome to the Sandbox!");
	}
};

Application* CreateApplication()
{
	return new Sandbox();
}