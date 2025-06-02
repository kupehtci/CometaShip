#ifndef COMETA_EVENT_H
#define COMETA_EVENT_H

#include "EventType.h"

class Event{

protected:
    bool _handled = false;
    EventType _type;

public:
    virtual ~Event() = default;

    [[nodiscard]] EventType GetEventType() const { return _type; }

    [[nodiscard]] bool HasBeenHandled() const { return _handled; }
    void SetHandled() { _handled = true; }

    virtual std::string ToString() = 0;
};

// ------------------------------------------
// |              MOUSE EVENTS              |
// ------------------------------------------

class MouseMoveEvent : public Event
{
private:
    float _x = 0.0f;
    float _y = 0.0f;
public:
    MouseMoveEvent(const float x, const float y) : _x(x), _y(y)
    {
        _type = COMETA_MOUSE_MOVE_EVENT;
    }

    [[nodiscard]] float GetX() const { return _x; }
    [[nodiscard]] float GetY() const { return _y; }

    std::string ToString() override
    {
        return "MouseMoveEvent: (" + std::to_string(_x) + ", " + std::to_string(_y) + ")";
    }

};

class MouseScrollEvent : public Event
{
    private:
    float _offsetX = 0.0f;
    float _offsetY = 0.0f;
    public:
    MouseScrollEvent(const float offsetX, const float offsetY) : _offsetX(offsetX), _offsetY(offsetY)
    {
        _type = COMETA_MOUSE_SCROLL_EVENT;
    }

    [[nodiscard]] float GetOffsetX() const { return _offsetY; }
    [[nodiscard]] float GetOffsetY() const { return _offsetY; }
    std::string ToString() override
    {
        return "MouseMoveEvent: (" + std::to_string(_offsetX) + ", " + std::to_string(_offsetX) + ")";
    }
};

class MousePressButtonEvent : public Event
{
private:
    int _mouseCode = 0;
public:
    explicit MousePressButtonEvent(int mouseCode){
        _mouseCode = mouseCode;
        _type = COMETA_MOUSE_BUTTON_PRESS_EVENT;
    }

    [[nodiscard]] int GetMouseCode() const { return _mouseCode; }
    std::string ToString() override
    {
        return "MousePressButtonEvent: " + std::to_string(_mouseCode);
    }
};

class MouseReleaseButtonEvent : public Event
{
private:
    int _mouseCode = 0;
public:
    explicit MouseReleaseButtonEvent(int mouseCode)
    {
        _mouseCode = mouseCode;
        _type = COMETA_MOUSE_BUTTON_RELEASE_EVENT;
    }

    [[nodiscard]] int GetMouseCode() const { return _mouseCode; }
    std::string ToString() override
    {
        return "MouseReleaseButtonEvent: " + std::to_string(_mouseCode);
    }
};

// ------------------------------------------
// |            KEYBOARD EVENTS             |
// ------------------------------------------

class KeyPressEvent : public Event
{
private:
    int _key = 0;
    bool _repeat = false;
public:

    explicit KeyPressEvent(const int key)
    {
        _key = key;
        _repeat = false;
        _type = COMETA_KEY_PRESS_EVENT;
    }

    explicit KeyPressEvent(const int key, const bool repeat)
    {
        _key = key;
        _repeat = repeat;
        _type = COMETA_KEY_PRESS_EVENT;
    }

    [[nodiscard]] int GetKey() const { return _key; }

    std::string ToString() override{
        return ("KeyPressEvent" + std::to_string(_key));
    }
};

class KeyReleasedEvent : public Event
{
private:
    int _key = 0;
public:
    explicit KeyReleasedEvent(const int key){
        _key = key;
        _type = COMETA_KEY_RELEASE_EVENT;
    }

     [[nodiscard]] int GetKey() const { return _key; }

    std::string ToString() override{
        return ("KeyReleaseEvent" + std::to_string(_key));
    }
};

#endif