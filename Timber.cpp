#include<sstream>
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

    // Variables to control time itself 
    Clock clock;

    //Timebar
    RectangleShape timebar;
    float timebarStartWidth = 400;
    float timebarStartHeight = 80;

    timebar.setSize(Vector2f(timebarStartWidth, timebarStartHeight));
    timebar.setFillColor(Color::Red);
    timebar.setPosition(1920 / 2 - timebarStartWidth / 2, 980);

    //Game Time
    Time gameTotalTime;
    float timeRemainning = 6.0f;
    float timebarWidthPerSecond = timebarStartWidth / timeRemainning;
    float timebarwidthPerFrame;

    //Track game is running or paused
    bool paused = true;

    //Score
    int score = 0;


    //Setting up Text
    Text messageText;
    Text scoreText;

    //Loading Font
    Font font;
    font.loadFromFile("Fonts/KOMIKAP_.ttf");

    //Set Font of our text
    messageText.setFont(font);
    scoreText.setFont(font);

    //Assign actual message to be displayed in game
    messageText.setString("PRESS ENTER TO START THE GAME!!!");
    scoreText.setString("Score = 0");

    //Setting characteristics if text
    
    //size
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    //color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    //Position of text

    /*We want to position it in the exact midpoint of the screen. Initially, this might not seem like a problem, but then we have to remember that the origin of everything we draw is at the top left-hand corner. So, if we simply divide the screen width and height by two and use the results in mesageText.setPosition..., then the top left of the text will be in the center of the screen and it will spread out untidily to the right.
    */
    FloatRect textRect = messageText.getLocalBounds();

    //Orgin will be recalculated if text length changes
    messageText.setOrigin(
        textRect.left + textRect.width / 2.0f, 
        textRect.top + textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f); //which is center of screen
    scoreText.setPosition(20, 20);

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

        if (Keyboard::isKeyPressed(Keyboard::Enter))
        {
            paused = false;
            //Reset the time and score
            score = 0;
            timeRemainning = 6.0f;
        }

        /*
        **************************************** Update the scene
        ****************************************
        */

        if (!paused)
        {
            // Measure time
            Time deltatime = clock.restart(); // restarts the clock and returns the elapsed time

            //Calculate remaining time
            timeRemainning -= deltatime.asSeconds();

            //Resize the timebar
            timebarwidthPerFrame = timebarWidthPerSecond * timeRemainning;
            timebar.setSize(Vector2f(timebarwidthPerFrame, timebarStartHeight));

            if (timeRemainning <= 0.0f)
            {
                //pause the game
                paused = true;
                //chnage the message
                messageText.setString("GameOver!!!");
                //Reposition the text to its center based on message new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin((textRect.left + textRect.width) / 2, 
                    (textRect.top + textRect.height)/ 2);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

            }


            //Setup the bee movement

            if (!beeActive)  //Not moving
            {
                // How fast is the bee 
                srand((int)time(0));
                beeSpeed = (rand() % 200) + 200;

                // How high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spritebee.setPosition(2000, height);
                beeActive = true;
            }
            else   //Moving
            {
                spritebee.setPosition(spritebee.getPosition().x - beeSpeed * deltatime.asSeconds(), spritebee.getPosition().y);

                // Has the bee reached the left-hand edge of the screen? 
                if (spritebee.getPosition().x < -100)
                {
                    // Set it up ready to be a whole new bee 
                    beeActive = false;
                }
            }

            //Manage CLoud
            //cloud1

            if (!cloud1Active)
            {
                // How fast is the cloud 
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                //How high the cloud
                srand((int)time(0) * 10);
                float height = (rand() % 150);
                spritecloud1.setPosition(-200, height);
                cloud1Active = true;
            }
            else
            {
                spritecloud1.setPosition(spritecloud1.getPosition().x + (deltatime.asSeconds() * cloud1Speed), spritecloud1.getPosition().y);

                // Has the cloud reached the right hand edge of the screen?
                if (spritecloud1.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud
                    cloud1Active = false;
                }
            }

            // Cloud 2
            if (!cloud2Active)
            {

                // How fast is the cloud 
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                // How high is the cloud 
                srand((int)time(0) * 20);
                float height = (rand() % 300) - 150;
                spritecloud2.setPosition(-200, height);
                cloud2Active = true;
            }
            else
            {

                spritecloud2.setPosition(spritecloud2.getPosition().x +
                    (cloud2Speed * deltatime.asSeconds()), spritecloud2.getPosition().y);

                // Has the cloud reached the right hand edge of the screen? 
                if (spritecloud2.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud 
                    cloud2Active = false;
                }
            }

            if (!cloud3Active)
            {
                // How fast is the cloud 
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                // How high is the cloud 
                srand((int)time(0) * 30);
                float height = (rand() % 450) - 150;
                spritecloud3.setPosition(-200, height);
                cloud3Active = true;
            }
            else
            {
                spritecloud3.setPosition(spritecloud3.getPosition().x + (cloud3Speed * deltatime.asSeconds()), spritecloud3.getPosition().y);

                // Has the cloud reached the right hand edge of the screen? 
                if (spritecloud3.getPosition().x > 1920)
                {
                    // Set it up ready to be a whole new cloud 
                    cloud3Active = false;
                }
            }
            //StringStream
            //its in update becaz it will be destroyed and new will be created in each frame
            std::stringstream ss; 

            //Update the scoretext       
            ss << "score = " << score;
            scoreText.setString(ss.str());
        } //End if(!paused)

        /*
        **************************************** Draw the scene
        ****************************************
        */


        // Clear everything from the last frame 
        window.clear();

        // Draw our game scene here

        //Drawing the sprite
        window.draw(spriteBackground);
        window.draw(spriteTree);
        window.draw(spritebee);
        window.draw(spritecloud1);
        window.draw(spritecloud2);
        window.draw(spritecloud3);
        //Drawing the scoretext
        window.draw(scoreText);
        //Draw the timebar
        window.draw(timebar);
        if (paused)
        {
            window.draw(messageText);
        }
        // Show everything we just drew 
        window.display();

    }

    return 0;
}

