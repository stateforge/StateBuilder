#include "Engine.h"
#include <iostream>

using namespace std;

Engine::Engine(void)
{
}

Engine::~Engine(void)
{
}

void Engine::StartOpen()
{
	cout << "Engine::StartOpen()\n";
}

void Engine::StartClose()
{
	cout << "Engine::StartClose()\n";
}

void Engine::Stop()
{
	cout << "Engine::Stop()\n";
}
