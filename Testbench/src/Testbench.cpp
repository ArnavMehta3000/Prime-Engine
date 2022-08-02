#include "pch.h"
#include <Prime.h>

class TestApp : public Prime::PrimeApp
{
public:
	TestApp() : Prime::PrimeApp()
	{
		
	}
	
	~TestApp()
	{
		
	}
};

Prime::PrimeApp* Prime::CreateApplication()
{
	return new TestApp();
}