#ifndef EMUSELECTOR_HPP
# define EMUSELECTOR_HPP

# include <exception>
# include <iostream>
# include "SDL2/SDL.h"
# include "SDL2/SDL_ttf.h"
# include "../Includes/Colors.hpp"

# define SELECTOR_W 320
# define SELECTOR_H 320

class EmuSelector {

public:

    EmuSelector(int debug_flag);
    ~EmuSelector(void);

    void    startup(void);

    int     get_confirmed_option(void);

    class EmuSelectorFailure : public std::exception {
        public:
        virtual const char *what(void) const throw();
    };

private:

    int     initialise_SDL(void);           // Initialise all SDL components
    int     initialise_TTF(void);           // Initialise font library
    void    window_loop(void);              // Main loop. Wait for user to make a choice between available systems

    // Utility
    void    display_choices(void);
    void    display_select_box(void);

    int     debug_flag;                 // Turns debugging on or off
    int     selected_option;            // Currently selected option (GB, GBC, GBA, EXIT)
    int     confirmed_option;
    bool    refresh_needed;             // States if we need to refresh the display immediatly

    SDL_Window      *selector_window;
    SDL_Renderer    *selector_renderer;
    SDL_Texture     *selector_texture;

    TTF_Font        *selector_font;
    SDL_Surface     *font_surface;
    SDL_Rect        text_rect;
    SDL_Texture     *selector_gb_text;
    SDL_Texture     *selector_gbc_text;
    SDL_Texture     *selector_gba_text;

};

#endif
