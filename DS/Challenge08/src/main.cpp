// Damian Seals
// CS 302
// Prof. Scott Emrich
// March 28, 2019

// This is a modification of a Lazyfoo productions tutorial program
// that opens a window and displays images when the up, down, left,
// and right arrow keys on the keyboard are pressed. The window does
// not close until the user closes it

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <iostream>

using namespace std;
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// Enumerates listed variables with ints starting at 0
enum KeyPressSurfaces {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL

};

// Global Variables, I know, don't use them, but even LazyFoo
// uses them in the beginning. They won't be global for long
SDL_Surface* gKeyPressSurfaces[ KEY_PRESS_SURFACE_TOTAL ];
SDL_Surface* loadSurface( string path )
{
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    
    return loadedSurface;
}

// Loads png files that will be shown when keys are pressed
bool loadMedia()
{
    
    
    //Loading success flag
    bool success = true;
    
    //Load default surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] = loadSurface( "default.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ] == NULL )
    {
        printf( "Failed to load default image!\n" );
        success = false;
    }
    
    //Load up surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] = loadSurface( "up.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ] == NULL )
    {
        printf( "Failed to load up image!\n" );
        success = false;
    }
    
    //Load down surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] = loadSurface( "down.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ] == NULL )
    {
        printf( "Failed to load down image!\n" );
        success = false;
    }
    
    //Load left surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] = loadSurface( "left.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ] == NULL )
    {
        printf( "Failed to load left image!\n" );
        success = false;
    }
    
    //Load right surface
    gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] = loadSurface( "right.png" );
    if( gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ] == NULL )
    {
        printf( "Failed to load right image!\n" );
        success = false;
    }
    
    return success;
}

int main( int argc, char* argv[] )
{
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;
    SDL_Surface* imageSurface = NULL;
    
    SDL_Event windowEvent;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }
    
    //Create window
    window = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
    
    // Check that the window creation was successful
    if( window == NULL )
    {
        printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        return 1;
    }
    
    // Check if png could be initialized
    if ( !( IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) ) {
        printf("Could not initialize SDL image! SDL Error: %s\n", IMG_GetError());
        return 1;
    }

    //Get window surface
    screenSurface = SDL_GetWindowSurface( window );
    
    //Fill the surface white
    SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0xFF, 0xFF, 0xFF ) );
    
    // Load the media
    if (!loadMedia()) {
        printf("Failed to load Media!\n");
        return 1;
    }
    
    
    // Makes the window stay open until the user closes it
    while ( true ) {
        if ( SDL_PollEvent(  &windowEvent ) ) {
            if ( windowEvent.type == SDL_QUIT) {
                break;
            } else if (windowEvent.type == SDL_KEYDOWN) {
                switch (windowEvent.key.keysym.sym) {
                    case SDLK_UP:
                        imageSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                        break;
                        
                    case SDLK_DOWN:
                        imageSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                        break;
                        
                    case SDLK_LEFT:
                        imageSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                        break;
                        
                    case SDLK_RIGHT:
                        imageSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                        break;
                        
                    default:
                        imageSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                        break;
                }
            }
        }
        // Applies imageSurface to screenSurface
        SDL_BlitSurface( imageSurface, NULL, screenSurface, NULL);
        
        //Update the surface
        SDL_UpdateWindowSurface( window );
    }
    
    // Memory Leak prevention
    SDL_FreeSurface(screenSurface);
    SDL_FreeSurface(imageSurface);
    
    screenSurface = NULL;
    imageSurface = NULL;
    
    //Destroy window
    SDL_DestroyWindow( window );
    
    //Quit SDL subsystems
    SDL_Quit();
    
    return 0;
}
