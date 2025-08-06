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
    this->controls[TOGGLE_MOUSE] = XK_m;
    this->controls[DOWN] = XK_e;
    this->controls[UP] = XK_q;
    this->controls[RIGHT] = XK_d;
    this->controls[BACKWARD] = XK_s;
    this->controls[LEFT] = XK_a;
    this->controls[FORWARD] = XK_w;

    this->currentKeyLayout = "QWERTY"; // Default key layout

    for (const auto& [control, active] : this->controls) {
        this->activeControls[control] = false; // Initialize all controls as inactive
        this->justPressedControls[control] = false; // Initialize all controls as not just pressed
    }

    this->keyLayout[LEFT] = "LEFT";
    this->keyLayout[RIGHT] = "RIGHT";
    this->keyLayout[FORWARD] = "FORWARD";
    this->keyLayout[BACKWARD] = "BACKWARD";
    this->keyLayout[UP] = "UP";
    this->keyLayout[DOWN] = "DOWN";
    this->keyLayout[RESET_POSITION] = "RESET_POSITION";
    this->keyLayout[TOGGLE_MOUSE] = "TOGGLE_MOUSE";
    this->keyLayout[TOGGLE_TEXTURE] = "TOGGLE_TEXTURE";
    this->keyLayout[TOGGLE_KEY_LAYOUT] = "TOGGLE_KEY_LAYOUT";
    this->keyLayout[EXIT] = "EXIT_PROGRAM";
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

void ControlManager::updateMouseData(const int mouseX, const int mouseY)
{
    this->mouse.lastX = this->mouse.currentX;
    this->mouse.lastY = this->mouse.currentY;

    this->mouse.currentX = mouseX;
    this->mouse.currentY = mouseY;

    this->mouse.deltaX = this->mouse.currentX - this->mouse.lastX;
    this->mouse.deltaY = this->mouse.currentY - this->mouse.lastY;

    this->mouse.rotationX += (this->mouse.deltaX * this->mouse.sensitivity) * FrameTimer::getInstance().getDeltaTime();
    this->mouse.rotationY += (this->mouse.deltaY * this->mouse.sensitivity) * FrameTimer::getInstance().getDeltaTime();
}


void ControlManager::processActiveControls()
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
                case TOGGLE_MOUSE:
                    if (this->justPressed(TOGGLE_MOUSE)) {
                        this->mouse.enabled = !this->mouse.enabled;
                        if (this->mouse.enabled) {
                            std::cout << "Mouse controls enabled." << std::endl;
                        } else {
                            std::cout << "Mouse controls disabled." << std::endl;
                        }
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

mouseData& ControlManager::getMouseData() {
    return this->mouse;
}

float ControlManager::getRotationX() const {
    return this->mouse.rotationX;
}

float ControlManager::getRotationY() const {
    return this->mouse.rotationY;
}

void ControlManager::printInfo() const {
    clearTerminalLines();
    std::cout << "Current key layout: " << this->currentKeyLayout << std::endl;
    std::cout << "Controls:" << std::endl;
    for (const auto& [control, key] : this->controls) {
        if (control != NOT_FOUND) {
            std::cout << "  " << BLUE << this->keyLayout.at(control) << ": " << RESET
                << BOLD << XKeysymToString(key) << RESET << std::endl;
        }
    }
    std::cout << std::endl;
}