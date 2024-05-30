#include <SDL_gpu_examples.h>

static Example* Examples[] =
{
	&ClearScreen_Example,
	&ClearScreenMultiWindow_Example,
	&BasicTriangle_Example,
	&BasicStencil_Example,
	&BasicCompute_Example,
	&ToneMapping_Example
};

int main(int argc, char **argv)
{
	Context context = { 0 };
	int exampleIndex = -1;
	int gotoExampleIndex = 0;
	int quit = 0;

	for (int i = 1; i < argc; i += 1)
	{
		if (SDL_strcmp(argv[i], "-name") == 0 && argc > i + 1)
		{
			const char* exampleName = argv[i + 1];
			int foundExample = 0;

			for (int j = 0; j < SDL_arraysize(Examples); j += 1)
			{
				if (SDL_strcmp(Examples[j]->Name, exampleName) == 0)
				{
					gotoExampleIndex = j;
					foundExample = 1;
					break;
				}
			}

			if (!foundExample)
			{
				SDL_Log("No example named '%s' exists.", exampleName);
				return 1;
			}
		}
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
		return 1;
	}

	SDL_Log("Welcome to the SDL_Gpu example suite!");
	SDL_Log("Press A/D to move between examples!");

	while (!quit)
	{
		SDL_Event evt;
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_EVENT_QUIT)
			{
				if (exampleIndex != -1)
				{
					Examples[exampleIndex]->Quit(&context);
				}
				quit = 1;
			}
			else if (evt.type == SDL_EVENT_KEY_DOWN)
			{
				if (evt.key.keysym.sym == SDLK_d)
				{
					gotoExampleIndex = exampleIndex + 1;
					if (gotoExampleIndex >= SDL_arraysize(Examples)) {
						gotoExampleIndex = 0;
					}
				}
				else if (evt.key.keysym.sym == SDLK_a)
				{
					gotoExampleIndex = exampleIndex - 1;
					if (gotoExampleIndex < 0) {
						gotoExampleIndex = SDL_arraysize(Examples) - 1;
					}
				}
				else if (evt.key.keysym.sym == SDLK_LEFT)
				{
					context.LeftPressed = SDL_TRUE;
				}
				else if (evt.key.keysym.sym == SDLK_RIGHT)
				{
					context.RightPressed = SDL_TRUE;
				}
				else if (evt.key.keysym.sym == SDLK_DOWN)
				{
					context.DownPressed = SDL_TRUE;
				}
				else if (evt.key.keysym.sym == SDLK_UP)
				{
					context.UpPressed = SDL_TRUE;
				}
			}
		}
		if (quit)
		{
			break;
		}

		if (gotoExampleIndex != -1)
		{
			if (exampleIndex != -1)
			{
				Examples[exampleIndex]->Quit(&context);
				SDL_zero(context);
			}

			exampleIndex = gotoExampleIndex;
			context.ExampleName = Examples[exampleIndex]->Name;
			SDL_Log("STARTING EXAMPLE: %s", context.ExampleName);
			if (Examples[exampleIndex]->Init(&context) < 0)
			{
				SDL_Log("Init failed!");
				return 1;
			}

			gotoExampleIndex = -1;
		}

		if (Examples[exampleIndex]->Update(&context) < 0)
		{
			SDL_Log("Update failed!");
			return 1;
		}

		if (Examples[exampleIndex]->Draw(&context) < 0)
		{
			SDL_Log("Draw failed!");
			return 1;
		}
	}

	return 0;
}
