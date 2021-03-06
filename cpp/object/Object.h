#include <iostream>
#include <glm/glm.hpp>

#ifndef _OBJECT_H_
#define _OBJECT_H_
class Object
{
private:
protected:
    unsigned int id;
    /* data */
public:
    virtual void createVAO() = 0;
    virtual void draw() = 0;
    virtual void bind() = 0;
};

#endif