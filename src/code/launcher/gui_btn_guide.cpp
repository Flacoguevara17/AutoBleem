//
// Created by screemer on 2019-03-02.
//

#include "gui_btn_guide.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "../gui/gui.h"
#include "../lang.h"
#include "../engine/scanner.h"

using namespace std;

//*******************************
// GuiBtnGuide::render
//*******************************
void GuiBtnGuide::render() {
    shared_ptr<Gui> gui(Gui::getInstance());
    gui->renderBackground();

    gui->renderTextBar();
    // -50 is because the "button guide" line is too low and the lines go below the bottom of the rectangle
    int offset = gui->renderLogo(true) - 50;
    int yoffsetPerLine = 5;  // +5 is so the icons don't overlap with the prior line

    int xLeft = 370;
    int xRight = 520;
    int line = 0;
    auto font = gui->sonyFonts[FONT_15];

    auto renderTextLineToColumns = [&] (const string &textLeft, const string &textRight, int line, int offset) {
        gui->renderTextLine(textLeft,  line, offset, POS_LEFT, xLeft, font);
        gui->renderTextLine(textRight, line, offset, POS_LEFT, xRight, font);
    };

    renderTextLineToColumns("",                     "-=" + _("Button Guide") + "=-", line++, offset += yoffsetPerLine);
    line++; offset += yoffsetPerLine;
    renderTextLineToColumns("|@X| / |@O|",          _("Select or cancel highlighted option"), line++, offset += yoffsetPerLine);
    renderTextLineToColumns("|@S|",                 _("Run using RetroBoot"), line++, offset += yoffsetPerLine);
    renderTextLineToColumns("|@R1| / |@L1|",        _("Quick scroll to next letter"), line++, offset += yoffsetPerLine);
    renderTextLineToColumns("|@Select|",            _("Games filter"), line++, offset += yoffsetPerLine);
    renderTextLineToColumns("|@L2| + |@Select|",    _("Change USB Games Sub-Directory"), line++, offset += yoffsetPerLine);
    renderTextLineToColumns("|@L2| + |@Select|",    _("Change RetroBoot System"), line++, offset += yoffsetPerLine);
    line++; offset += yoffsetPerLine;
    renderTextLineToColumns("",                     "-=" + _("In Game") + "=-", line++, offset += yoffsetPerLine);
    renderTextLineToColumns("|@Select| + |@T|",     _("Emulator config MENU"), line++, offset += yoffsetPerLine);
    renderTextLineToColumns("RESET",                _("Quit emulation - back to AutoBleem"), line++, offset += yoffsetPerLine);
    line++; offset += yoffsetPerLine;
    renderTextLineToColumns("",                     "-=" + _("In Retroarch Game") + "=-", line++, offset += yoffsetPerLine);
    renderTextLineToColumns("|@Select| + |@Start|", _("Open Retroarch Menu"), line++, offset += yoffsetPerLine);
    renderTextLineToColumns(_("POWER"),             _("Exit to EvoUI"), line++, offset += yoffsetPerLine);
    line++; offset += yoffsetPerLine;
    renderTextLineToColumns("|@L2| + |@R2|",        _("In Boot Menu: Safe Power Off The Console"), line++, offset += yoffsetPerLine);

    gui->renderStatus("|@O| " + _("Go back") + "|");
    SDL_RenderPresent(renderer);
}

//*******************************
// GuiBtnGuide::loop
//*******************************
void GuiBtnGuide::loop() {
    shared_ptr<Gui> gui(Gui::getInstance());
    bool menuVisible = true;
    while (menuVisible) {
        gui->watchJoystickPort();
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.scancode == SDL_SCANCODE_SLEEP) {
                    gui->drawText(_("POWERING OFF... PLEASE WAIT"));
                    Util::powerOff();

                }
            }
            // this is for pc Only
            if (e.type == SDL_QUIT) {
                menuVisible = false;
            }
            switch (e.type) {
                case SDL_JOYBUTTONUP:


                    if (e.jbutton.button == gui->_cb(PCS_BTN_CIRCLE, &e)) {
                        Mix_PlayChannel(-1, gui->cancel, 0);
                        menuVisible = false;

                    };


            }

        }
    }
}
