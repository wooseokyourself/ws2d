#pragma once

#include <string>
#include <box2d.h>
#include <SDL.h>
#include <SDL_image.h>

class Object
{
public:
    Object();
    Object(SDL_Renderer* Renderer, const std::string PngPath, const int w, const int h);
    ~Object();
    void SetPosition(const int x, const int y);
    void SetRotation(const float degree);
    b2Vec2 GetPosition();
    SDL_Rect GetBoundingBox();
    virtual void Update();
    void Draw(SDL_Renderer* Renderer);
    
protected:
    SDL_Surface* PngToSurface(const std::string& PngPath, const int w, const int h);
    void SetTexture(SDL_Renderer* Renderer);
    
protected: // Texture
    SDL_Surface* m_Surface;
    SDL_Texture* m_Texture;
    SDL_Rect m_Bbox;
    float m_Degree;
};
