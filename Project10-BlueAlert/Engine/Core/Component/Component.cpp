#include "Component.h"
#include "../Scene/Scene.h"

void Component::Awake() {}

void Component::Start() {}

void Component::Update(float deltaTime) {}

void Component::Render(Renderer &renderer) {}

void Component::ProcessEvent(const SDL_Event &e) {}

void Component::Destroy() {}

Component::~Component() {}
