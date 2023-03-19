#include "core/application.h"

int main() 
{
	Application app = Application();	
	app.Init();
	app.OnUpdate();

	return 0;
}