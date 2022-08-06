#include "pch.h"
#include <Prime.h>
using namespace Prime;

class TestApp : public Prime::PrimeApp
{
public:
	TestApp() : Prime::PrimeApp()
	{
	}

	~TestApp()
	{

	}


	virtual void OnStart() override
	{
		
	}

	virtual void OnUpdate(float dt) override
	{
	}
	virtual void OnRender(float dt) override
	{

	}
};

Prime::PrimeApp* Prime::CreateApplication()
{
	return new TestApp();
}