#include "EmuSelector.hpp"

EmuSelector::EmuSelector(int flag) : debug_flag(flag) {

    this->selected_option = 0;
    this->confirmed_option = -1;
    this->refresh_needed = false;
}

// This should be called before starting one of the emulators
// Consider nesting the selector part into try / catch brackets
EmuSelector::~EmuSelector(void) {

    SDL_DestroyTexture(this->selector_texture);
    SDL_DestroyRenderer(this->selector_renderer);
    SDL_DestroyWindow(this->selector_window);
    SDL_Quit();
    TTF_CloseFont(this->selector_font);
    TTF_Quit();
    return ;
}

// Called from outside the class, calls everything else
void    EmuSelector::startup(void) {

    if (!this->initialise_SDL()) {
        std::cerr << RED << "SDL Error: " << SDL_GetError() << RESET << std::endl;
        throw EmuSelector::EmuSelectorFailure();
    }

    if (!this->initialise_TTF()) {
        std::cerr << RED << "TTF Error: " << TTF_GetError() << RESET << std::endl;
        throw EmuSelector::EmuSelectorFailure();
    }

    this->window_loop();

}

int EmuSelector::get_confirmed_option(void) {
    return (this->confirmed_option);
}

int EmuSelector::initialise_SDL(void) {

    if (this->debug_flag) {
        std::cout << YELLOW << "Initialising selector's SDL components." << RESET << std::endl;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
         return (0);
    }

    this->selector_window = SDL_CreateWindow("MultiBoyEmu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SELECTOR_W, SELECTOR_H, 0);
    if (!this->selector_window) {
         return (0);
    }

    this->selector_renderer = SDL_CreateRenderer(this->selector_window, -1, SDL_RENDERER_ACCELERATED);
    if (!this->selector_renderer) {
         return (0);
    }

    this->selector_texture = SDL_CreateTexture(this->selector_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SELECTOR_W, SELECTOR_H);
    if (!this->selector_texture) {
         return (0);
    }

    return (1);
}

int EmuSelector::initialise_TTF(void) {

    if (this->debug_flag) {
        std::cout << YELLOW << "Initialising selector's font (TTF) components." << RESET << std::endl;
    }

    if (TTF_Init() == -1) {
        return (0);
    }

    this->selector_font = TTF_OpenFont("./EmuSelect/PixeloidMono.ttf", 20);
    if (!this->selector_font) {
        return (0);
    }

    // Bind the three texts (GB, GBC, GBA) to a texture -> is it best practice ? unsure

    SDL_Color color = {248, 248, 255, 255};

    this->font_surface = TTF_RenderUTF8_Solid(this->selector_font, "GAME BOY", color);
    if (!this->font_surface) {
        return (0);
    }
    this->selector_gb_text = SDL_CreateTextureFromSurface(this->selector_renderer, this->font_surface);
    if (!this->selector_gb_text) {
        return (0);
    }

    this->font_surface = TTF_RenderUTF8_Solid(this->selector_font, "GAME BOY COLOR", color);
    if (!this->font_surface) {
        return (0);
    }
    this->selector_gbc_text = SDL_CreateTextureFromSurface(this->selector_renderer, this->font_surface);
    if (!this->selector_gbc_text) {
        return (0);
    }

    this->font_surface = TTF_RenderUTF8_Solid(this->selector_font, "GAME BOY ADVANCE", color);
    if (!this->font_surface) {
        return (0);
    }
    this->selector_gba_text = SDL_CreateTextureFromSurface(this->selector_renderer, this->font_surface);
    if (!this->selector_gba_text) {
        return (0);
    }

    return (1);
}


// Main loop for the selector
void    EmuSelector::window_loop(void) {

    bool quit = false;
    SDL_Event event;

    while (quit != true) {

        if (SDL_WaitEvent(&event) != 0) {

            switch (event.type) {

                case SDL_QUIT:
                    quit = true;
                    break;

                case SDL_KEYDOWN:
                    if (event.key.keysym.scancode ==  SDL_SCANCODE_RETURN) {
                        this->confirmed_option = 1; //TODO: make it responsive to user input
                        return ;
                    }
                    break;
            }
        }

        this->display_choices();
        this->display_select_box();
        SDL_RenderPresent(this->selector_renderer);
    }

    if (quit == true) {
        this->confirmed_option = -1;
    }
}

void    EmuSelector::display_choices(void) {

    // Render background color
    SDL_SetRenderTarget(this->selector_renderer, this->selector_texture);
    SDL_SetRenderDrawColor(this->selector_renderer, 31, 32, 34, 255);
    SDL_RenderClear(this->selector_renderer);
    SDL_SetRenderTarget(this->selector_renderer, NULL);
    SDL_RenderCopy(this->selector_renderer, this->selector_texture, NULL, NULL);

    // Display the choices text
    int textW = 0;
    int textH = 0;
    SDL_QueryTexture(this->selector_gb_text, NULL, NULL, &textW, &textH);
    this->text_rect = {25, 75, textW, textH};
    SDL_RenderCopy(this->selector_renderer, this->selector_gb_text, NULL, &this->text_rect);

    SDL_QueryTexture(this->selector_gbc_text, NULL, NULL, &textW, &textH);
    this->text_rect = {25, 150, textW, textH};
    SDL_RenderCopy(this->selector_renderer, this->selector_gbc_text, NULL, &this->text_rect);

    SDL_QueryTexture(this->selector_gba_text, NULL, NULL, &textW, &textH);
    this->text_rect = {25, 225, textW, textH};
    SDL_RenderCopy(this->selector_renderer, this->selector_gba_text, NULL, &this->text_rect);

    return;
}

void    EmuSelector::display_select_box(void) {

    // hardcoded for now, TODO: make it change with input

    this->text_rect = {10, 65, 300, 50};
    SDL_SetRenderDrawColor(this->selector_renderer, 251, 79, 20, 255);
    SDL_RenderDrawRect(this->selector_renderer, &this->text_rect);

    return;
}

const char *EmuSelector::EmuSelectorFailure::what() const throw() {
	return ("Fatal error: selectore failure.\nPlease consider reporting this on github.");
}
