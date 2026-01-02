#include "Hitbox.h"

    //hitboxes can be used as a property of an another object (ball/block) to indicate what area they occupy

    float x1, y1; // bottom-left
    float x2, y2; // top-right

    Hitbox::Hitbox(float x1, float y1, float x2, float y2){ 
        if(x2 < x1){
            this->x2 = x1;
            this->x1 = x2;
        }
        else{
            this->x1 = x1;
            this->x2 = x2;
        }

        if(y2 < y1){
            this->y2 = y1;
            this->y1 = y2;
        }
        else{
            this->y1 = y1;
            this->y2 = y2;
        }
    }

    float Hitbox::getX1() const {
        return this->x1;
    }
    float Hitbox::getY1() const {
        return this->y1;
    }
    float Hitbox::getX2() const {
        return this->x2;
    }
    float Hitbox::getY2() const {
        return this->y2;
    }

    float Hitbox::getWidth() const {
        return this->x2 - this->x1;
    }

    float Hitbox::getHeight() const {
        return this->y2 - this->y1;
    }

    void Hitbox::moveX(float dist)
    {
        this->x1 += dist;
        this->x2 += dist;
    }
    void Hitbox::moveY(float dist)
    {
        this->y1 += dist;
        this->y2 += dist;
    }


    bool Hitbox::isOverlapping(const Hitbox& other) const {
        return !(x2 <= other.x1 || x1 >= other.x2 || y2 <= other.y1 || y1 >= other.y2);
    }