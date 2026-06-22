#pragma once
class WindowState {
private:
    int width;
    int height;
public:
    WindowState() {
        this->height = 600;
        this->width = 800;
    }
    int getWidth()
    {
        return width;
    }
    int getHeight()
    {
        return height;
    }
    void setWidth(int width)
    {
        this->width = width;
    }
    void setHeight(int height)
    {
        this->height = height;
    }

};