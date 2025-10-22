#include "SDL3/SDL_render.h"
#include <imgui.h>
#include <backends/imgui_impl_sdl3.h>
#include <backends/imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>

typedef struct {
	SDL_Window* window;
	SDL_Renderer* renderer;
} AppState;


int main(int argc, char* argv[]) {
	// Init systems
	if(!SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO)) {
		SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	// Create window and renderer
	AppState* as = (AppState*)SDL_calloc(1, sizeof(AppState));
	if(!as) {
		return SDL_APP_FAILURE;
	}

	if(!SDL_CreateWindowAndRenderer("Empty Project Template", 800, 600, 0, &as->window, &as->renderer)) {
		SDL_Log("Couldn't create window and renderer: %s\n", SDL_GetError());
		return SDL_APP_FAILURE;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	ImGui_ImplSDL3_InitForSDLRenderer(as->window, as->renderer);
	ImGui_ImplSDLRenderer3_Init(as->renderer);

	bool done = false;

	while(!done) {
		// Event handle
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			ImGui_ImplSDL3_ProcessEvent(&event);
			
			if(event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_QUIT) {
				done = true;
			}
		}
		
		// Start frame
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		// Menu bar
		if(ImGui::BeginMainMenuBar()) {
			if(ImGui::BeginMenu("File")) {
				if(ImGui::MenuItem("Close")) {
					done = true;
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// Render
		ImGui::Render();
		SDL_SetRenderDrawColor(as->renderer, 138, 118, 145, 255);
		SDL_RenderClear(as->renderer);
		ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), as->renderer);
		SDL_RenderPresent(as->renderer);

	}
	
	// Cleanup
	ImGui_ImplSDLRenderer3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();

	if(as) {
		SDL_DestroyRenderer(as->renderer);
		SDL_DestroyWindow(as->window);
		SDL_free(as);
	}

	SDL_Quit();

	return 0;
}
