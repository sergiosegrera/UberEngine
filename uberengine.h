#ifndef UBERENGINE
#define UBERENGINE
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <vector>


class GameObject {
    public:
        bool operator== (const GameObject *gameObject) const; 

        bool collideRect(GameObject gameObject);
        bool collideRectVector(std::vector <GameObject*> rects, int size);
        
        SDL_Rect* getRect();
        int* getPosX();
        int* getPosY();
        int* getWidth();
        int* getHeight();

        SDL_Renderer* getRenderer();

        virtual void update();
        virtual void draw();
        virtual void draw(int offsetX, int offsetY);

    protected:
        SDL_Renderer *renderer;
        SDL_Surface *surface;
        SDL_Texture *texture; 

        double angle;
        SDL_Point pivot;
        SDL_Rect rect;  
        bool collidable;
        SDL_RendererFlip flip;
        
        GameObject *parent;
};


class SpriteObject : public GameObject {
    public:
        SpriteObject(SDL_Renderer *r);
        SpriteObject(GameObject *p); 

        void createSurface(int x, int y, int w, int h);
        void createSurface(int x, int y, int w, int h, Uint32 c);
        void createSurface(int x, int y, const char *i);

        virtual void update();
        virtual void draw();
        virtual void draw(int offsetX, int offsetY);
};


class TextObject : public GameObject {
    public:
        TextObject(SDL_Renderer *r);
        TextObject(GameObject *p);

        void createSurface(int x, int y, const char *t, int s, const char *f);

        void updateText(const char *t);

        virtual void update();
        virtual void draw();
        virtual void draw(int offsetX, int offsetY);

    private:
        TTF_Font *font;
        SDL_Color color;
        char *text;

};


template <class object>
class GameObjects : public GameObject {
    public:
        void add(object* g) {
            gameObjects.push_back(g);
            gameObjectsSize = gameObjects.size();
        }
        void remove(object* g) {
            for (int i = 0; i < gameObjectsSize; i++) {
                if (*gameObjects[i] == g) {
                    gameObjects.erase(gameObjects.begin() + i);
                    gameObjects = gameObjects.size();
                }
            }
        }
        object* get(int i) {
            return gameObjects[i] ;
        }
        int getObjectsSize() {
            return gameObjectsSize;
        }

        virtual void update() {
            for (int i = 0; i < gameObjectsSize; i++) {
                gameObjects[i]->update();
            }
        }
        virtual void draw() {
            for (int i = 0; i < gameObjectsSize; i++) {
                gameObjects[i]->draw();
            }
        }
        virtual void draw(int offsetX, int offsetY) {
            for (int i = 0; i < gameObjectsSize; i++) {
                gameObjects[i] ->draw(offsetX, offsetY);
            }
        }

    private:
        std::vector <object*> gameObjects;
        int gameObjectsSize;
};


class GameCamera : public GameObject {
    public:
       GameCamera(int w, int h);

       void setTarget(GameObject *gameObject);
       void setSize(int w, int h);
       void setMaximumBounds(int w, int h);

       virtual void update();

    private:
       GameObject *target;
       bool maxBounds;
       int maxW, maxH;
};


class GameManager : public GameObject {
    public:
        GameManager();
        GameManager(int x, int y);

        int getScreenWidth();
        int getScreenHeight();

        virtual void events();
        virtual void update();
        virtual void draw();
        void capFps();
        void loop();
    
    protected:
        SDL_Window *window;
        SDL_Surface *screen;

    private: 
        Uint32 startTick;
        SDL_Event event;
        bool quit;
        int screenWidth, screenHeight;
};

#endif
