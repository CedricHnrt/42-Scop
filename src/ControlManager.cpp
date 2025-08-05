#include "ControlManager.hpp"

ControlManager& ControlManager::getInstance() {
    static ControlManager instance;
    return instance;
}

ControlManager::ControlManager()
{
    this->controls[EXIT] = XK_Escape;
    this->controls[TOGGLE_KEY_LAYOUT] = XK_Tab;
    this->controls[RESET_POSITION] = XK_r;
    this->controls[TOGGLE_TEXTURE] = XK_space;
    this->controls[DOWN] = XK_e;
    this->controls[UP] = XK_q;
    this->controls[RIGHT] = XK_d;
    this->controls[BACKWARD] = XK_s;
    this->controls[LEFT] = XK_a;
    this->controls[FORWARD] = XK_w;

    this->currentKeyLayout = "QWERTY"; // Default key layout

    for (const auto& control : this->controls) {
        this->activeControls[control.first] = false; // Initialize all controls as inactive
        this->justPressedControls[control.first] = false; // Initialize all controls as not just pressed
    }

    this->keyLayout[LEFT] = "LEFT";
    this->keyLayout[RIGHT] = "RIGHT";
    this->keyLayout[FORWARD] = "FORWARD";
    this->keyLayout[BACKWARD] = "BACKWARD";
    this->keyLayout[UP] = "UP";
    this->keyLayout[DOWN] = "DOWN";
    this->keyLayout[RESET_POSITION] = "RESET_POSITION";
    this->keyLayout[TOGGLE_TEXTURE] = "TOGGLE_TEXTURE";
    this->keyLayout[TOGGLE_KEY_LAYOUT] = "TOGGLE_KEY_LAYOUT";
    this->keyLayout[EXIT] = "EXIT_PROGRAM";
}

static void clearTerminalLines(const int n)
{
    for (int i = 0; i < n; ++i) {
        std::cout << "\33[2K\r"; // Clear the current line
        if (i < n - 1)
            std::cout << "\033[A"; // Move the cursor up one line
    }
}

Control ControlManager::findControl(const KeySym keysym)
{
    for (const auto& [control, key] : this->controls) {
        if (key == keysym) {
            return control;
        }
    }
    return NOT_FOUND;
}

void ControlManager::handleKeyPress(const KeySym key) {
    if (const Control control = findControl(key); control != NOT_FOUND) {
        if (!this->activeControls[control]) {
            this->justPressedControls[control] = true; // Mark as just pressed
        }
        this->activeControls[control] = true;
    }
}

void ControlManager::handleKeyRelease(const KeySym key) {
    if (const Control control = findControl(key); control != NOT_FOUND) {
        this->activeControls[control] = false;
    }
}

bool ControlManager::justPressed(const Control control) const{
    for (const auto& [c, justPressed] : this->justPressedControls) {
        if (c == control) {
            return justPressed;
        }
    }
    return false;
}

void ControlManager::resetJustPressedControls() {
    for (auto& [control, justPressed] : this->justPressedControls) {
        justPressed = false;
    }
}

void ControlManager::checkActiveControls()
{
    for (const auto& [control, active] : this->activeControls) {
        if (active) {
            switch (control) {
                case LEFT: ObjectData::getInstance().moveObject(LEFT); break;
                case RIGHT: ObjectData::getInstance().moveObject(RIGHT); break;
                case FORWARD: ObjectData::getInstance().moveObject(FORWARD); break;
                case BACKWARD: ObjectData::getInstance().moveObject(BACKWARD); break;
                case UP: ObjectData::getInstance().moveObject(UP); break;
                case DOWN: ObjectData::getInstance().moveObject(DOWN); break;
                case RESET_POSITION: ObjectData::getInstance().moveObject(RESET_POSITION); break;
                case TOGGLE_TEXTURE:
                    if (this->justPressed(TOGGLE_TEXTURE)) {
                        ObjectData::getInstance().toggleTexture();
                    } break;
                case TOGGLE_KEY_LAYOUT:
                    if (this->justPressed(TOGGLE_KEY_LAYOUT)) {
                        this->switchKeyLayout();
                    } break;
                case EXIT: WindowManager::getInstance().exitProgram(); break;
            default: break;
            }
        }
    }
    this->resetJustPressedControls();
}

void ControlManager::switchKeyLayout() {
    // Switch between QWERTY and AZERTY layouts
    if (this->currentKeyLayout == "QWERTY") {
        this->currentKeyLayout = "AZERTY";
        this->controls[LEFT] = XK_q;
        this->controls[RIGHT] = XK_d;
        this->controls[FORWARD] = XK_z;
        this->controls[BACKWARD] = XK_s;
        this->controls[UP] = XK_a;
        this->controls[DOWN] = XK_e;
    } else if (this->currentKeyLayout == "AZERTY") {
        this->currentKeyLayout = "QWERTY";
        this->controls[LEFT] = XK_a;
        this->controls[RIGHT] = XK_d;
        this->controls[FORWARD] = XK_w;
        this->controls[BACKWARD] = XK_s;
        this->controls[UP] = XK_q;
        this->controls[DOWN] = XK_e;
    }
    clearTerminalLines(14);
    this->printInfo();
}

void ControlManager::printInfo() const {
    clearTerminalLines(1);
    std::cout << "Current key layout: " << this->currentKeyLayout << std::endl;
    std::cout << "Controls:" << std::endl;
    for (const auto& [control, key] : this->controls) {
        std::cout << "  " << BLUE << this->keyLayout.at(control) << ": " << RESET
            << BOLD << XKeysymToString(key) << RESET << std::endl;
    }
    std::cout << std::endl;
}