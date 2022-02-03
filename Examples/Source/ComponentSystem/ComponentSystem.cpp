#include "Engine.h"

#include "GameObject.h"
#include "SpriteComponent.h"

GameObject* testObj = nullptr;

void Init()
{
    testObj = new GameObject;
    testObj->SetPosition({ 300, 300 });
    SpriteComponent* spr = testObj->AddComponent<SpriteComponent>();
    spr->SetSpritePath("../Assets/dawdwdawda.png");

    GameObject::InitAll();
}

void Update(float dt)
{
    static float totalTime = 0.0f;
    totalTime += dt / 2.0f;

    testObj->SetPosition({ 400.0f + 300.0f * abs(sinf(totalTime)), 300.f });

    uint8_t color = uint8_t((abs(sinf(totalTime))) * 255.0f);
    testObj->GetComponent<SpriteComponent>()->SetColor({ color, 0, uint8_t(255 - color) });

    GameObject::UpdateAll(dt);
    GameObject::RenderAll();
}

int main()
{
    Elia2D::Start("Component System Example", &Update, &Init);
}