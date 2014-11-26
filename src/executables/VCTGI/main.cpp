#include "Renderer.h"
#include "Context.h"

int main()
{
	Context::getInstance();
	Renderer::getInstance()->init();
	Renderer::getInstance()->run();
	delete Renderer::getInstance();
	return 0;
}
