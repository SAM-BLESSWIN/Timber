#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

//Global Scope
//function declaration
void updateBranches(int seed);

const int MAX_BRANCHES = 6;
Sprite branches[MAX_BRANCHES];  //array to hold 6 sprites

//which side player or branch is?
enum side
{
    LEFT,
    RIGHT,
    NONE
};

side branchpositions[MAX_BRANCHES]; //array to hold 6 branch side


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

    //Prepare 6 branches Texture
    Texture texturebranch;
    texturebranch.loadFromFile("Graphics/branch.png");

    //set texture for each branch sprite
    for (int i = 0; i < MAX_BRANCHES; i++)
    {
        branches[i].setTexture(texturebranch);
        branches[i].setPosition(-2000, -2000);

        //set sprite orgin to dead center
        //we are changing orgin because can spin it aound without changing position
        branches[i].setOrigin(220, 20);
    }

    //Setting Player Texture
    Texture playerTexture;
    playerTexture.loadFromFile("Graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(playerTexture);
    spritePlayer.setPosition(580, 720);

    //Player starts from left side
    side playerside = side::LEFT;

    //Setting Gravestone Texture
    Texture textureRIP;
    textureRIP.loadFromFile("Graphics/rip.png"); 
    Sprite spriteRIP; 
    spriteRIP.setTexture(textureRIP); 
    spriteRIP.setPosition(600, 860);

    // Prepare the axe 
    Texture textureAxe;
    textureAxe.loadFromFile("Graphics/axe.png"); 
    Sprite spriteAxe; 
    spriteAxe.setTexture(textureAxe); 
    spriteAxe.setPosition(700, 830);

    //Line the axe with the tree 
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log 
    Texture textureLog;
    textureLog.loadFromFile("Graphics/log.png"); 
    Sprite spriteLog; 
    spriteLog.setTexture(textureLog); 
    spriteLog.setPosition(810, 720); //bottom of tree

    //some other information need for log
    bool logActive = false;
    float logspeedX = 1000;
    float logspeedY = -1500;

    //control the player input
    bool acceptInput = false;

    //Prepare the sound
    
    //player chopping sound
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("SFX/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    //ran out of time sound
    SoundBuffer timeoutBuffer;
    timeoutBuffer.loadFromFile("SFX/out_of_time.wav");
    Sound Timeout;
    Timeout.setBuffer(timeoutBuffer);

    //player death sound
    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("SFX/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    while (window.isOpen())
    {
        /*
        **************************************** Handle the players input
        ****************************************
        */

        //various operating system events : a key press, key release, mouse movement, mouse click, game controller action, or something that happened to the window itself(resized, moved, and so on)
        Event event;
       
        //pollEvent function puts data into the event object that describes an operating system event
        //while loop is because there might be many events stored in a queue. 
        while (window.pollEvent(event))
        {
            if (event.type == Event::KeyReleased && !paused) 
            {
                //to make listen to keypress again
                acceptInput = true;
                //hide the axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

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

            //Make all branches disappear from second position
            for (int i = 1; i < MAX_BRANCHES; i++)
            {
                branchpositions[i] = side::NONE;
            }

            //make the gravestone hide
            spriteRIP.setPosition(675, 2000);
            
            //Reset player position
            spritePlayer.setPosition(580, 720);

            acceptInput = true;
        }
        //since each loop will run 1000 times, single key press will always run many time
        //so we handle it with a boolean
        if (acceptInput) 
        {
            if (Keyboard::isKeyPressed(Keyboard::Right))
            {
                //set player position to right
                playerside = side::RIGHT;
                //scored
                score++;
                //Add more time to the amount of time remmaing
                timeRemainning += (2 / score) + 0.15f;
                //set axe position
                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                //set player position
                spritePlayer.setPosition(1200, 720);
                //Update the branches
                updateBranches(score);
                //set the log flying to the left
                spriteLog.setPosition(810, 720);
                logspeedX = -5000;
                logActive = true;

                //Not detect input
                acceptInput = false;

                //play sound
                chop.play();
            }
            if (Keyboard::isKeyPressed(Keyboard::Left))
            {
                playerside = side::LEFT;
                score++;
                timeRemainning += (2 / score) + 0.15f;
                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                spritePlayer.setPosition(580, 720);
                updateBranches(score);
                //set the log flying to right
                spriteLog.setPosition(810, 720);
                logspeedX = 5000;
                logActive = true;

                acceptInput = false;
                chop.play();
            }
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

                //play sound
                Timeout.play();
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
            //its in update becaz string is immutable, so we will be creating new stringstream object in each frame
            std::stringstream ss; 

            //Update the scoretext       
            ss << "score = " << score;
            scoreText.setString(ss.str());

            //update the branch sprites
            for (int i = 0; i < MAX_BRANCHES; i++)
            {
                //std::cout << i << "\n"; //debug
                float height = i * 150;
                if (branchpositions[i] == side::LEFT)
                {
                    //move the sprite to left side
                    branches[i].setPosition(610, height);
                    //flip the sprite
                    branches[i].setRotation(180);
                }
                else if (branchpositions[i]==side::RIGHT)
                {
                    //move the sprite to right side
                    branches[i].setPosition(1330, height);
                    //set zero rotation
                    branches[i].setRotation(0);
                }
                else
                {
                    //hide the branch
                    branches[i].setPosition(3000, height);
                }
            }
            //std::cout << "Done" << "\n"; //debug

            //Handle log flying
            if (logActive)
            {
                spriteLog.setPosition
                (
                    spriteLog.getPosition().x + (logspeedX * deltatime.asSeconds()),
                    spriteLog.getPosition().y + (logspeedY * deltatime.asSeconds())
                );

                //has the log reached the right hand edge
                if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x>2000)
                {
                    //set it up ready as a new log
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            //has the player hit by the branch
            if (branchpositions[5] == playerside)
            {
                //death
                paused = true;
                acceptInput = false;

                //set the gravestone
                spriteRIP.setPosition(525, 760);
                
                //hide the player
                spritePlayer.setPosition(2000, 660);

                //change the text message
                messageText.setString("SQUISHED!!!");
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin((textRect.left + textRect.width) / 2,
                    (textRect.top + textRect.height) / 2);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                //play sound
                death.play();
            }

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
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);
        for (int i = 0; i < MAX_BRANCHES; i++)
        {
            window.draw(branches[i]);
        }
        window.draw(spritebee);
        window.draw(spritecloud1);
        window.draw(spritecloud2);
        window.draw(spritecloud3);

        
        //Drawing the scoretext
        window.draw(scoreText);
        //Draw the timebar
        window.draw(timebar);
        //Draw the message if paused
        if (paused)
        {
            window.draw(messageText);
        }

        // Show everything we just drew 
        window.display();
    }
    return 0;
}

void updateBranches(int seed)
{
    //move all the branches down one place 
    for (int j = MAX_BRANCHES - 1; j > 0; j--)
    {
        branchpositions[j] = branchpositions[j - 1];
      //  std::cout << branchpositions[j]<<"\n";
    }

    //spawn a new branch at position 0
    srand((int)time(0) + seed);
    int r = (rand() % 5);

    switch (r)
    {
    case 0:
        branchpositions[0] = side::LEFT;
        break;
    case 1:
        branchpositions[0] = side::RIGHT;
        break;
    default:
        branchpositions[0] = side::NONE;
        break;
    }
}

