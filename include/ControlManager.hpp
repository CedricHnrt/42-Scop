#ifndef CONTROLMANAGER_HPP
#define CONTROLMANAGER_HPP

#include <unordered_map>
#include "WindowManager.hpp"

enum Control {
    NOT_FOUND = -1,
    EXIT,
    RESET_POSITION,
    TOGGLE_TEXTURE,
    TOGGLE_KEY_LAYOUT,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD,
    UP,
    DOWN
};

class ControlManager {
    public:
        static ControlManager& getInstance();
        ControlManager(const ControlManager&) = delete;
        ControlManager& operator=(const ControlManager&) = delete;
        void* operator new(size_t) = delete;
        void operator delete(void*) = delete;

        void handleKeyPress(KeySym);
        void handleKeyRelease(KeySym);
        void checkActiveControls();
        void printInfo() const;

    private:
        std::unordered_map<Control, KeySym> controls;
        std::unordered_map<Control, bool> activeControls;
        std::unordered_map<Control, bool> justPressedControls;
        std::string currentKeyLayout; // Default key layout

        std::unordered_map<Control, std::string> keyLayout;

        [[nodiscard]] Control findControl(KeySym keysym);
        bool justPressed(Control control) const;
        void resetJustPressedControls();
        void switchKeyLayout();
        ControlManager();
        ~ControlManager() = default;

};

#endif //CONTROLMANAGER_HPP
