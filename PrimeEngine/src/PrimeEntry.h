#pragma once

extern Prime::PrimeApp* Prime::CreateApplication();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(pCmdLine);

	auto app = Prime::CreateApplication();
	app->Run();
	delete app;
}
