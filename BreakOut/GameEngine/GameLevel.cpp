//
//  GameLevel.cpp
//  BreakOut
//
//  Created by newworld on 2017/3/23.
//  Copyright © 2017年 siyuxing. All rights reserved.
//

#include "GameLevel.hpp"
#include <fstream>
#include <sstream>
#include <GLKit/GLKMathTypes.h>
#include "ResourceManager.hpp"



static const GLchar * fullPath(const GLchar *filename)
{
    const GLchar *fullPath = nullptr;
    if (filename) {
        NSString *fileName = [NSString stringWithUTF8String:filename];
        NSString *FullPath = [[NSBundle mainBundle] pathForResource:fileName ofType:nil];
        fullPath = FullPath.UTF8String;
    }
    
    return fullPath;
    
}



void GameLevel::Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight)
{
    // Clear old data
    this->bricks.clear();
    // Load from file
    GLuint tileCode;
    GameLevel level;
    std::string line;
    
    const GLchar * filePath = fullPath(file);

    std::ifstream fstream(filePath);
    std::vector<std::vector<GLuint>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // Read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<GLuint> row;
            while (sstream >> tileCode) // Read each word seperated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
    
}



void GameLevel::draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

GLboolean GameLevel::isCompleted()
{
    for (GameObject &tile : this->bricks)
        if (!tile.isSolid && !tile.Destroyed)
            return GL_FALSE;
    return GL_TRUE;
}

void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
    // Calculate dimensions
    GLuint height = tileData.size();
    GLuint width = tileData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
    GLfloat unit_width = levelWidth / static_cast<GLfloat>(width), unit_height = levelHeight / height;
    // Initialize level tiles based on tileData
    for (GLuint y = 0; y < height; ++y)
    {
        for (GLuint x = 0; x < width; ++x)
        {
            // Check block type from level data (2D level array)
            if (tileData[y][x] == 1) // Solid
            {
                
                GLKVector2 pos = {unit_width * x, unit_height * y};
                GLKVector2 size = {unit_width, unit_height};
                GameObject obj(pos, size, ResourceManager::GetTexture("block_solid"),{0.8f, 0.8f, 0.7f});
                obj.isSolid = GL_TRUE;
                this->bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)	// Non-solid; now determine its color based on level data
            {
                GLKVector3 color = {1.0f,1.0f,1.0f}; // original: white
                if (tileData[y][x] == 2)
                    color = {0.2f, 0.6f, 1.0f};
                else if (tileData[y][x] == 3)
                    color = {0.0f, 0.7f, 0.0f};
                else if (tileData[y][x] == 4)
                    color = {0.8f, 0.8f, 0.4f};
                else if (tileData[y][x] == 5)
                    color = {1.0f, 0.5f, 0.0f};
                
                GLKVector2 pos = {unit_width * x, unit_height * y};
                GLKVector2 size = {unit_width, unit_height};
                this->bricks.push_back(GameObject(pos, size, ResourceManager::GetTexture("block"), color));
            }
        }
    }
}
