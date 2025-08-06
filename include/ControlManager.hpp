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
    TOGGLE_MOUSE,
    LEFT,
    RIGHT,
    FORWARD,
    BACKWARD,
    UP,
    DOWN
};

struct mouseData
{
    float sensitivity = 0.5f;
    int currentX = 0.0f;
    int currentY = 0.0f;
    int lastX = 0.0f;
    int lastY = 0.0f;
    int deltaX = 0.0f;
    int deltaY = 0.0f;
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    bool enabled = false;
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
        void processActiveControls();
        void updateMouseData(int mouseX, int mouseY);
        void printInfo() const;

        [[nodiscard]] mouseData& getMouseData();
        [[nodiscard]] float getRotationX() const;
        [[nodiscard]] float getRotationY() const;

    private:
        std::unordered_map<Control, KeySym> controls;
        std::unordered_map<Control, bool> activeControls;
        std::unordered_map<Control, bool> justPressedControls;
        std::string currentKeyLayout; // Default key layout
        std::unordered_map<Control, std::string> keyLayout;
        mouseData mouse;

        [[nodiscard]] Control findControl(KeySym keysym);
        bool justPressed(Control control) const;
        void resetJustPressedControls();
        void switchKeyLayout();
        ControlManager();
        ~ControlManager() = default;

};

#endif //CONTROLMANAGER_HPP
