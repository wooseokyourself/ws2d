#include "Object.h"

Object::Object()
: m_Degree(0)
{
    
}

Object::Object(const std::string PngPath, const int w, const int h)
: m_Degree(0)
{
    m_Surface = PngToSurface(PngPath, w, h);
    m_WorldBox.w = w;
    m_WorldBox.h = h;
}

Object::~Object()
{
    SDL_FreeSurface(m_Surface);
    SDL_DestroyTexture(m_Texture);
}

void Object::SetPosition(const int x, const int y)
{
    m_WorldBox.x = x;
    m_WorldBox.y = y;
}

void Object::SetRotation(const float degree)
{
    m_Degree = degree;
}

b2Vec2 Object::GetPosition()
{
    return b2Vec2(m_WorldBox.x, m_WorldBox.y);
}

SDL_Rect Object::GetBoundingBox()
{
    return m_WorldBox;
}

void Object::Update()
{
    
}

SDL_Surface* Object::PngToSurface(const std::string& PngPath, const int w, const int h)
{
    SDL_Surface* _Surface = IMG_Load(PngPath.c_str());
    if (_Surface == NULL)
    {
        fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
    
    // Resize surface.
    SDL_Surface* Surface = SDL_CreateRGBSurface(0, w, h, _Surface->format->BitsPerPixel, _Surface->format->Rmask, _Surface->format->Gmask, _Surface->format->Bmask, _Surface->format->Amask);
    SDL_BlitScaled(_Surface, NULL, Surface, NULL);
    SDL_FreeSurface(_Surface);
    
    return Surface;
}

void Object::SetTexture(SDL_Renderer* Renderer)
{
    m_Texture = SDL_CreateTextureFromSurface(Renderer, m_Surface);
    if (!m_Texture)
    {
        fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
}
