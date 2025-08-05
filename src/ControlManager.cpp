#include "ControlManager.hpp"

ControlManager& ControlManager::getInstance() {
    static ControlManager instance;
    return instance;
}

ControlManager::ControlManager()
{
    this->controls[LEFT] = XK_a;
    this->controls[RIGHT] = XK_d;
    this->controls[FORWARD] = XK_w;
    this->controls[BACKWARD] = XK_s;
    this->controls[UP] = XK_q;
    this->controls[DOWN] = XK_e;
    this->controls[RESET_POSITION] = XK_r;
    this->controls[TOGGLE_TEXTURE] = XK_space;
    this->controls[EXIT] = XK_Escape;

    for (auto& [control, active] : this->activeControls) {
        active = false;
    }
    for (auto& [control, justPressed] : this->justPressedControls) {
        justPressed = false;
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
                case EXIT: WindowManager::getInstance().exitProgram(); break;
            default: break;
            }
        }
    }
    this->resetJustPressedControls();
}
