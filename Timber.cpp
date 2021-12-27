#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;

int main()
{
    // Create a video mode object 
    VideoMode vm(1920, 1080);
    // Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);


    //Make a background sprite
    // Create a texture to hold a graphic on the GPU 
    Texture textureBackground;
    // Load a graphic into the texture 
    textureBackground.loadFromFile("Graphics/background.png");
    // Create a sprite 
    Sprite spriteBackground;
    // Attach the texture to the sprite 
    spriteBackground.setTexture(textureBackground);
    // Set the spriteBackground to cover the screen 
    spriteBackground.setPosition(0,0);

    // Make a tree sprite 
    Texture textureTree;
    textureTree.loadFromFile("Graphics/tree.png"); 
    Sprite spriteTree; 
    spriteTree.setTexture(textureTree); 
    spriteTree.setPosition(810, 0);

    //Make a bee sprite
    Texture texturebee;
    texturebee.loadFromFile("Graphics/bee.png");
    Sprite spritebee;
    spritebee.setTexture(texturebee);
    spritebee.setPosition(0,800);

    //Make a cloud sprite
    Texture texturecloud;
    texturecloud.loadFromFile("Graphics/cloud.png");
    Sprite spritecloud1;
    Sprite spritecloud2;
    Sprite spritecloud3;
    spritecloud1.setTexture(texturecloud);
    spritecloud2.setTexture(texturecloud);
    spritecloud3.setTexture(texturecloud);
    spritecloud1.setPosition(0, 0);
    spritecloud2.setPosition(0, 250);
    spritecloud3.setPosition(0, 500);

    // Is the bee currently moving? 
    bool beeActive = false;

    // How fast can the bee fly 
    float beeSpeed = 0.0f;

    // Are the clouds currently on screen? 
    bool cloud1Active = false;
    bool cloud2Active = false; 
    bool cloud3Active = false;

    // How fast is each cloud? 
    float cloud1Speed = 0.0f; 
    float cloud2Speed = 0.0f; 
    float cloud3Speed = 0.0f;


    while (window.isOpen())
    {
        /*
        **************************************** Handle the players input
        ****************************************
        */

        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        /*
        **************************************** Update the scene
        ****************************************
        */


        /*
        **************************************** Draw the scene
        ****************************************
        */


        // Clear everything from the last frame 
        window.clear();

        // Draw our game scene here
        window.draw(spriteBackground);
        window.draw(spriteTree);
        window.draw(spritebee);
        window.draw(spritecloud1);
        window.draw(spritecloud2);
        window.draw(spritecloud3);

        // Show everything we just drew 
        window.display();

    }

    return 0;
}

