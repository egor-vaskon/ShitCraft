//
// Created by egorv on 4/8/2023.
//

#ifndef SHITCRAFT_DRAWABLE_H
#define SHITCRAFT_DRAWABLE_H

#include "../renderer/Transform.h"

struct Drawable {
    virtual ~Drawable() = default;
    virtual void draw(const Transform &transform) const = 0;
};

#endif //SHITCRAFT_DRAWABLE_H
