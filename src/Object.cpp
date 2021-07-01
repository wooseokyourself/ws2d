#include "Object.h"

Object::Object()
: m_Degree(0)
{
    
}

Object::Object(SDL_Renderer* Renderer, const std::string PngPath, const int w, const int h)
: m_Degree(0)
{
    m_Surface = PngToSurface(PngPath, w, h);
    m_Bbox.w = w;
    m_Bbox.h = h;
    SetTexture(Renderer);
    SDL_FreeSurface(m_Surface);
}

Object::~Object()
{
    SDL_DestroyTexture(m_Texture);
}

void Object::SetPosition(const int x, const int y)
{
    m_Bbox.x = x;
    m_Bbox.y = y;
}

void Object::SetRotation(const float degree)
{
    m_Degree = degree;
}

b2Vec2 Object::GetPosition()
{
    return b2Vec2(m_Bbox.x, m_Bbox.y);
}

SDL_Rect Object::GetBoundingBox()
{
    return m_Bbox;
}

void Object::Update()
{
    
}

void Object::Draw(SDL_Renderer* Renderer) const
{
    SDL_Point p; p.x = 0; p.y = 0;
    SDL_RenderCopyEx(Renderer, m_Texture, NULL, &m_Bbox, -m_Degree, &p, SDL_FLIP_NONE);
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
    if (m_Texture == NULL)
    {
        fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
        exit(1);
    }
}
