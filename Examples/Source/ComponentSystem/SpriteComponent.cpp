#include "SpriteComponent.h"

#include "GameObject.h"

SpriteComponent::SpriteComponent()
    : myTexture(0U)
    , mySize({ 0, 0 })
    , myColor({ 255, 255, 255 })
    , myTexturePath("")
{}
SpriteComponent::~SpriteComponent()
{
    if (myTexture)
    {
        ReturnTexture(myTexture);
    }
}

SpriteComponent::SpriteComponent(const SpriteComponent& sc)
    : myTexture(0U)
    , mySize(sc.mySize)
    , myColor(sc.myColor)
    , myTexturePath(sc.myTexturePath)
{
    myTexture = CreateTexture(myTexturePath);
}
SpriteComponent::SpriteComponent(SpriteComponent&& sc)
    : myTexture(sc.myTexture)
    , mySize(sc.mySize)
    , myColor(sc.myColor)
    , myTexturePath(sc.myTexturePath)
{
    sc.myTexture = 0;
    sc.myTexturePath = "";
}
SpriteComponent& SpriteComponent::operator=(const SpriteComponent& sc)
{
    mySize = sc.mySize;
    myColor = sc.myColor;
    myTexturePath = sc.myTexturePath;
    myTexture = CreateTexture(myTexturePath);

    return *this;
}
SpriteComponent& SpriteComponent::operator=(SpriteComponent&& sc)
{
    mySize = sc.mySize;
    myColor = sc.myColor;
    myTexturePath = sc.myTexturePath;
    myTexture = sc.myTexture;

    sc.myTexture = 0;
    sc.myTexturePath = "";

    return *this;
}

void SpriteComponent::Init(class GameObject* parent)
{}
void SpriteComponent::Update(GameObject* parent, float dt)
{}
void SpriteComponent::Render(GameObject* parent)
{
    DrawTexturedQuad(myTexture, Convertv2f_v2(parent->GetPosition()), mySize, myColor);
}

void SpriteComponent::SetSpritePath(const char* const path)
{
    if (myTexture)
    {
        ReturnTexture(myTexture);
    }

    myTexturePath = path;
    myTexture = CreateTexture(path, &mySize);
}
void SpriteComponent::SetSize(v2 size)
{
    mySize = size;
}
void SpriteComponent::SetColor(Color color)
{
    myColor = color;
}

v2 SpriteComponent::GetSize() const
{
    return mySize;
}
Color SpriteComponent::GetColor() const
{
    return myColor;
}