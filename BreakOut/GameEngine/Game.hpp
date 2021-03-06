//
//  Game.hpp
//  BreakOut
//
//  Created by newworld on 2017/3/23.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
#include <OpenGLES/gltypes.h>
#include <GLKit/GLKit.h>
#include <tuple>
#include <GLKit/GLKMathTypes.h>
#include <vector>
#include "GameObject.hpp"
#include "GameLevel.hpp"


extern GLuint framebuffer;

// Represents the current state of the game
enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

// Represents the four possible (collision) directions
enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<GLboolean, Direction, GLKVector2> Collision; // <collision?, what direction?, difference vector center - closest point>

// Initial size of the player paddle
const GLKVector2 PLAYER_SIZE = {100, 20};
// Initial velocity of the player paddle
const GLfloat PLAYER_VELOCITY(500.0f);
// Initial velocity of the Ball
const GLKVector2 INITIAL_BALL_VELOCITY = {100.0f, -350.0f};
// Radius of the ball object
const GLfloat BALL_RADIUS = 12.5f;

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
    // Game state
    GameState              State;
    GLboolean              Keys[1024];
    GLuint                 Width, Height;
    std::vector<GameLevel> Levels;
    GLuint                 Level;
    //std::vector<PowerUp>   PowerUps;
    // Constructor/Destructor
    Game(GLuint width, GLuint height);
    ~Game();
    // Initialize game state (load all shaders/textures/levels)
    void Init();
    // GameLoop
    void Update(GLfloat dt);
    void Render(GLfloat dt);
    void DoCollisions();
    // Reset
    void ResetLevel();
    void ResetPlayer();
    
    void shoot();
    void move(bool left);
    // Powerups
};



#endif /* Game_hpp */
