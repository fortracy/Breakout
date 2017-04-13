//
//  Game.cpp
//  BreakOut
//
//  Created by newworld on 2017/3/23.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#include "Game.hpp"
#include "SpriteRender.hpp"
#include "GameObject.hpp"
#include "ResourceManager.hpp"
#include "ParticleGenerator.hpp"
#include <GLKit/GLKMathTypes.h>
#include "BallObject.hpp"
#include "PostProcessor.hpp"
#include "GLKVector2_cpp.h"
#include <simd/common.h>

// Game-related State data
SpriteRenderer    *Renderer;
GameObject        *Player;
BallObject        *Ball;
ParticleGenerator *Particles;
PostProcessor     *Effects;
GLfloat            ShakeTime = 0.0f;

Game::Game(GLuint width, GLuint height)
: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
    Init();
}

Game::~Game()
{
    delete Renderer;
    delete Player;
    delete Ball;
    delete Particles;
    delete Effects;
}

void Game::Init()
{
    // Load shaders
    ResourceManager::LoadShader("sprite.vs", "sprite.frag", nullptr, "sprite");
    ResourceManager::LoadShader("particle.vs", "particle.frag", nullptr, "particle");
    ResourceManager::LoadShader("postProcess.vs", "postProcess.frag", nullptr, "postprocessing");
    // Configure shaders
    //MARK:投影矩阵要设置好，否则会影响渲染;
    GLKMatrix4 projection = GLKMatrix4MakeOrtho(0.0f, static_cast<GLfloat>(this->Width), static_cast<GLfloat>(this->Height),0.0f, -1.0f, 1.0f);
    
    
    ResourceManager::GetShader("sprite").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
    ResourceManager::GetShader("particle").SetMatrix4("projection", projection);
    // Load textures
    ResourceManager::LoadTexture("background.jpg", GL_FALSE, "background");
    ResourceManager::LoadTexture("awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("block.png", GL_FALSE, "block");
    ResourceManager::LoadTexture("block_solid.png", GL_FALSE, "block_solid");
    ResourceManager::LoadTexture("paddle.png", GL_TRUE, "paddle");
    ResourceManager::LoadTexture("particle.png", GL_TRUE, "particle");
    // Set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 500);
    Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
    // Load levels
    GameLevel one; one.Load("one.lvl", this->Width, this->Height * 0.5);
    GameLevel two; two.Load("two.lvl", this->Width, this->Height * 0.5);
    GameLevel three; three.Load("three.lvl", this->Width, this->Height * 0.5);
    GameLevel four; four.Load("four.lvl", this->Width, this->Height * 0.5);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    // Configure game objects
    GLKVector2 playerPos = {this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y};
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
    GLKVector2 ballPos = playerPos + GLKVector2Make(PLAYER_SIZE.x / 2 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY, ResourceManager::GetTexture("face"));
}

void Game::Update(GLfloat dt)
{
    // Update objects
    Ball->Move(dt, this->Width);
    // Check for collisions
    this->DoCollisions();
    // Update particles
    Particles->Update(dt, *Ball, 2,{Ball->Radius / 2,Ball->Radius / 2});
    // Reduce shake time
    if (ShakeTime > 0.0f)
    {
        ShakeTime -= dt;
        if (ShakeTime <= 0.0f)
            Effects->Shake = GL_FALSE;
    }
    // Check loss condition
    if (Ball->Position.y >= this->Height) // Did ball reach bottom edge?
    {
        this->ResetLevel();
        this->ResetPlayer();
    }
}

void Game::shoot()
{
    if (this->State == GAME_ACTIVE) {
        Ball->Stuck = GL_FALSE;
    }
}

void Game::move(bool left)
{
    if (this->State == GAME_ACTIVE) {
        GLfloat velocity = PLAYER_VELOCITY * 0.01;
        
        
        if (left) {
            if (Player->Position.x >= 0)
            {
                Player->Position.x -= velocity;
                if (Ball->Stuck)
                    Ball->Position.x -= velocity;
            }
        }else{
            if (Player->Position.x <= this->Width - Player->Size.x)
            {
                Player->Position.x += velocity;
                if (Ball->Stuck)
                    Ball->Position.x += velocity;
            }
        }
    }
}

void Game::Render(GLfloat dt)
{
    if (this->State == GAME_ACTIVE)
    {
        glViewport(0, 0, this->Width, this->Height);
        // Begin rendering to postprocessing quad
        Effects->BeginRender();
        // Draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"), {0, 0}, {static_cast<float>(this->Width), static_cast<float>(this->Height)}, 0.0f);
        // Draw level
        this->Levels[this->Level].draw(*Renderer);
        // Draw player
       Player->Draw(*Renderer);
        // Draw particles
        Particles->Draw();
        // Draw ball
        Ball->Draw(*Renderer);
        // End rendering to postprocessing quad
       Effects->EndRender();
        
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // Render postprocessing quad
        Effects->Render(dt);
    }
}


void Game::ResetLevel()
{
    if (this->Level == 0)this->Levels[0].Load("one.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 1)
        this->Levels[1].Load("two.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 2)
        this->Levels[2].Load("three.lvl", this->Width, this->Height * 0.5f);
    else if (this->Level == 3)
        this->Levels[3].Load("four.lvl", this->Width, this->Height * 0.5f);
}

void Game::ResetPlayer()
{
    // Reset player/ball stats
    Player->Size = PLAYER_SIZE;
    Player->Position = {this->Width / 2 - PLAYER_SIZE.x / 2, this->Height - PLAYER_SIZE.y};
    Ball->Reset(Player->Position + GLKVector2Make(PLAYER_SIZE.x / 2 - BALL_RADIUS,  -(BALL_RADIUS * 2)), INITIAL_BALL_VELOCITY);
    // Also disable all active powerups
    Effects->Chaos = Effects->Confuse = GL_FALSE;
    Ball->PassThrough = Ball->Sticky = GL_FALSE;
    Player->Color = {1.0f,1.0f,1.0f};
    Ball->Color = {1.0f,1.0f,1.0f};
}

// Collision detection
GLboolean CheckCollision(GameObject &one, GameObject &two);
Collision CheckCollision(BallObject &one, GameObject &two);
Direction VectorDirection(GLKVector2 closest);

void Game::DoCollisions()
{
    for (GameObject &box : this->Levels[this->Level].bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Ball, box);
            if (std::get<0>(collision)) // If collision is true
            {
                // Destroy block if not solid
                if (!box.isSolid)
                {
                    box.Destroyed = GL_TRUE;
                  //  this->SpawnPowerUps(box);
                }
                else
                {   // if block is solid, enable shake effect
                    ShakeTime = 0.05f;
                    Effects->Shake = GL_TRUE;
                }
                // Collision resolution
                Direction dir = std::get<1>(collision);
                GLKVector2 diff_vector = std::get<2>(collision);
                if (!(Ball->PassThrough && !box.isSolid)) // don't do collision resolution on non-solid bricks if pass-through activated
                {
                    if (dir == LEFT || dir == RIGHT) // Horizontal collision
                    {
                        Ball->Velocity.x = -Ball->Velocity.x; // Reverse horizontal velocity
                        // Relocate
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.x);
                        if (dir == LEFT)
                            Ball->Position.x += penetration; // Move ball to right
                        else
                            Ball->Position.x -= penetration; // Move ball to left;
                    }
                    else // Vertical collision
                    {
                        Ball->Velocity.y = -Ball->Velocity.y; // Reverse vertical velocity
                        // Relocate
                        GLfloat penetration = Ball->Radius - std::abs(diff_vector.y);
                        if (dir == UP)
                            Ball->Position.y -= penetration; // Move ball bback up
                        else
                            Ball->Position.y += penetration; // Move ball back down
                    }
                }
            }
        }
    }
    // And finally check collisions for player pad (unless stuck)
    Collision result = CheckCollision(*Ball, *Player);
    if (!Ball->Stuck && std::get<0>(result))
    {
        // Check where it hit the board, and change velocity based on where it hit the board
        GLfloat centerBoard = Player->Position.x + Player->Size.x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->Size.x / 2);
        // Then move accordingly
        GLfloat strength = 2.0f;
        GLKVector2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
        Ball->Velocity = GLKVector2Normalize(Ball->Velocity)*GLKVector2Length(oldVelocity);
         // Fix sticky paddle
        Ball->Velocity.y = -1 * std::abs(Ball->Velocity.y);
        
        // If Sticky powerup is activated, also stick ball to paddle once new velocity vectors were calculated
     //   Ball->Stuck = Ball->Sticky;
    }
}

GLboolean CheckCollision(GameObject &one, GameObject &two) // AABB - AABB collision
{
    // Collision x-axis?
    GLboolean collisionX = one.Position.x + one.Size.x >= two.Position.x &&
    two.Position.x + two.Size.x >= one.Position.x;
    // Collision y-axis?
    GLboolean collisionY = one.Position.y + one.Size.y >= two.Position.y &&
    two.Position.y + two.Size.y >= one.Position.y;
    // Collision only if on both axes
    return collisionX && collisionY;
}

int clamp(int value, int min, int max)
{
    if (value < min)
    {
        return min;
    }
    else if (value > max)
    {
        return max;
    }
    
    return value;
}


GLKVector2 clamp(GLKVector2 v, const GLKVector2 min, const GLKVector2 max)
{
    GLKVector2 clampVector;
    clampVector.x = clamp(v.x, min.x, max.x);
    clampVector.y = clamp(v.y, min.y, max.y);
    
    return clampVector;
}


Collision CheckCollision(BallObject &one, GameObject &two) // AABB - Circle collision
{
    // Get center point circle first
    GLKVector2 center(one.Position + one.Radius);
    // Calculate AABB info (center, half-extents)
    GLKVector2 aabb_half_extents = {two.Size.x / 2, two.Size.y / 2};
    GLKVector2 aabb_center = {two.Position.x + aabb_half_extents.x, two.Position.y + aabb_half_extents.y};
    // Get difference vector between both centers
    GLKVector2 difference = center - aabb_center;
    
    GLKVector2 clamped;
    clamped = clamp(difference, -aabb_half_extents, aabb_half_extents);
//    clamped = difference;
    //GLKVector2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // Now that we know the the clamped values, add this to AABB_center and we get the value of box closest to circle
    GLKVector2 closest = aabb_center + clamped;
    // Now retrieve vector between center circle and closest point AABB and check if length < radius
    difference = closest - center;

    if ( GLKVector2Length(difference) < one.Radius) // not <= since in that case a collision also occurs when object one exactly touches object two, which they are at the end of each collision resolution stage.
        return std::make_tuple(GL_TRUE, VectorDirection(difference), difference);
    else
        return std::make_tuple(GL_FALSE, UP,GLKVector2Make(0.0, 0.0));
}
// Calculates which direction a vector is facing (N,E,S or W)
Direction VectorDirection(GLKVector2 target)
{
    GLKVector2 compass[] = {
        {0.0f, 1.0f},	// up
        {1.0f, 0.0f},	// right
        {0.0f, -1.0f},	// down
        {-1.0f, 0.0f}	// left
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        
        GLfloat dot_product = GLKVector2DotProduct(GLKVector2Normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}
