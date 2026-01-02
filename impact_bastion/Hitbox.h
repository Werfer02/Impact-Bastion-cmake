#pragma once

class Hitbox
{
private:
    float x1, y1; // bottom-left
    float x2, y2; // top-right

public:
    Hitbox(float x1, float y1, float x2, float y2);

    float getX1() const;
    float getY1() const;
    float getX2() const;
    float getY2() const;

    float getWidth() const;
    float getHeight() const;

    void moveX(float dist);
    void moveY(float dist);

    bool isOverlapping(const Hitbox& other) const;
};