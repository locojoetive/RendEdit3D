#include "Window.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

int main()
{
	// Error check if window creation failed
	Window* window = new Window(
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		"3D Rasterizer"
	);

	// Main loop: keep window open until closed
	while (!window->shouldClose())
	{
		window->Update();
	}
	window->closeWindow();
	return 0;
}
