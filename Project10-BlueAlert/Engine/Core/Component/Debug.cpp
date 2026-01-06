#include "Debug.h"
Debug* Debug::instance = nullptr;

Debug::~Debug()
{
    if (instance == this) instance = nullptr;
}
