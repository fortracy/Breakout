//
//  GameObject.cpp
//  BreakOut
//
//  Created by newworld on 2017/3/24.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#include "GameObject.hpp"



GameObject::GameObject()
: Position{0, 0}, Size{1, 1}, Velocity{0.0f,0.0f}, Color{1.0f,0.0f,0.0f}, Rotation(0.0f), Sprite(), isSolid(false), Destroyed(false) { };

GameObject::GameObject(GLKVector2 pos, GLKVector2 size, Texture2D sprite, GLKVector3 color,GLKVector2 velocity)
: Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), isSolid(false), Destroyed(false) { };

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}
