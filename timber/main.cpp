#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>
#include <sstream>

using namespace sf;
using namespace std;

enum class side
{
    LEFT,
    RIGHT,
    NONE
};

void GoCenterText(Text& textMessage)
{
    FloatRect textRect = textMessage.getLocalBounds();
    textMessage.setOrigin(
        textRect.left + textRect.width * 0.5f,
        textRect.top + textRect.height * 0.5f
    );
}
    
void UpdateBranches(side sides[], int length, mt19937& gen)
{
    for (int i = length - 1; i >= 0; --i)
    {
        sides[i] = sides[i - 1];
    }

    int rnd = gen() % 5;
    switch (rnd)
    {
    case 0:
        sides[0] = side::LEFT;
        break;
    case 1:
        sides[0] = side::RIGHT;
        break;
    default:
        sides[0] = side::NONE;
        break;
    }
}

int main()
{
    random_device rd;
    mt19937 gen(rd());

    //해상도
    VideoMode vm(1920, 1080);

    //창그려주기
    RenderWindow window(vm, "Timber!", Style::Default);
    //Default 창모드, fullscreen 풀모드

    Font fontKOMIMAP;
    fontKOMIMAP.loadFromFile("fonts/KOMIKAP_.ttf");

    Text textMessage;
    textMessage.setFont(fontKOMIMAP);
    Text textScore;
    textScore.setFont(fontKOMIMAP);

    textMessage.setString("Press Enter to start!");
    textScore.setString("Score = 0");

    textMessage.setCharacterSize(75);
    textScore.setCharacterSize(100);

    textMessage.setFillColor(Color::White);
    textScore.setFillColor(Color::White);

    textScore.setPosition(20, 20);

    GoCenterText(textMessage);

    textMessage.setPosition(1920 * 0.5f, 1080 * 0.5f);

    Texture textureBackground;
    textureBackground.loadFromFile("graphics/background.png");

    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");

    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");

    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");

    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);

    const int countClouds = 3;
    Sprite spriteClouds[countClouds];
    for (int i = 0; i < countClouds; i++)
    {
        spriteClouds[i].setTexture(textureCloud);
    }

    Sprite spriteTree;
    spriteTree.setTexture(textureTree);

    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    
    for (int i = 0; i < countClouds; ++i)
    {
        spriteClouds[i].setTexture(textureCloud);
        spriteClouds[i].setPosition(0, 150 * i);
        spriteClouds[i].setOrigin(0, 0);
    }

    const int countBranches = 6;
    Sprite spriteBranches[countBranches];
    side sideBranches[countBranches];

    for (int i = 0; i < countBranches; ++i)
    {
        spriteBranches[i].setTexture(textureBranch);
        spriteBranches[i].setPosition(-2000, 2000);
        spriteBranches[i].setOrigin(220, 40);

        sideBranches[i] = side::NONE;
    }
  
    spriteBackground.setPosition(0, 0);
    spriteTree.setPosition(810, 0);
    spriteBee.setPosition(2000, 0);

    bool beeActive = false;
    float beeSpeed = 0.f;

    bool activeClouds[countClouds];
    float speedClouds[countClouds];

    for (int i = 0; i < countClouds; i++)
    {
        activeClouds[i] = false;
        speedClouds[i] = 0.f;
    }

    RectangleShape timerBar;
    float timerBarWidth = 400;
    float timerBarHeight = 80;
    Vector2f timerBarSize = Vector2f(timerBarWidth, timerBarHeight);
    timerBar.setSize(timerBarSize);
    Vector2f timerPos = Vector2f(1920 * 0.5f - timerBarWidth * 0.5F, 980.f);
    timerBar.setPosition(timerPos);
    timerBar.setFillColor(Color::Red);

    bool isPause = true;
    int score = 100;
    const float timeMax = 6.0f;
    float timeRemaining = timeMax;
    float timerBarWidthPerSecond = timerBarWidth / timeMax;

    // Prepare the player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    // The player starts on the left
    side playerSide = side::LEFT;

    // Prepare the gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // Prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    // Line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // Prepare the flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);

    // 브금생성
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);

    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound oot;
    oot.setBuffer(ootBuffer);

    // Some other useful log related variables
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    bool acceptInput = false;

    Clock clock;

    //GameScene
    while (window.isOpen())
    {
        Time dt = clock.restart();

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                // 입력 처리
            case Event::Closed:
                window.close();
                break;
            case Event::KeyPressed:
                switch (event.key.code)
                {
                case Keyboard::Escape:
                    window.close();
                    break;
                case Keyboard::Return:
                {
                    isPause = false;
                    score = 0;
                    timeRemaining = timeMax;
                    acceptInput = true;

                    for (int i = 0; i < countBranches; ++i)
                    {
                        sideBranches[i] = side::NONE;
                    }

                    spriteRIP.setPosition(675, 2000);
                    spritePlayer.setPosition(580, 720);
                }
                break;
                case Keyboard::Left:
                    if (acceptInput && !isPause)
                    {
                        //소리발생
                        chop.play();

                        playerSide = side::LEFT;
                        ++score;
                        timeRemaining += (2.f / score) + 0.15f;
                        if (timeRemaining > timeMax)
                        {
                            timeRemaining = timeMax;
                        }

                        spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);
                        spritePlayer.setPosition(580, 720);

                        UpdateBranches(sideBranches, countBranches, gen);

                        spriteLog.setPosition(810, 720);
                        logSpeedX = 5000;
                        logActive = true;

                        acceptInput = false;
                    }
                    break;
                case Keyboard::Right:
                    if (acceptInput && !isPause)
                    {
                        chop.play();

                        playerSide = side::RIGHT;
                        ++score;
                        timeRemaining += (2.f / score) + 0.15f;
                        if (timeRemaining > timeMax)
                        {
                            timeRemaining = timeMax;
                        }

                        spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);
                        spritePlayer.setPosition(1200, 720);

                        UpdateBranches(sideBranches, countBranches, gen);

                        spriteLog.setPosition(810, 720);
                        logSpeedX = -5000;
                        logActive = true;

                        acceptInput = false;
                    }
                    break;
                default:
                    break;
                }
                break;
            case Event::KeyReleased:
                if (event.key.code == Keyboard::Left || event.key.code ==
                    Keyboard::Right)
                {
                    if (!isPause)
                    {
                        acceptInput = true;

                        spriteAxe.setPosition(2000,
                            spriteAxe.getPosition().y);
                    }
                }
                break;
            default:
                break;
            }
        }

        // 업데이트
        if (!isPause)
        {
            //하늘
            for (int i = 0; i < countClouds; i++)
            {
                if (!activeClouds[i])
                {
                    speedClouds[i] = gen() % 300 + 300;
                    speedClouds[i] *= -1.f;
                    float y = gen() % 200 + 200;
                    float x = (gen() % 200) + 2000;
                    spriteClouds[i].setPosition(x, y);
                    activeClouds[i] = true;
                }
                else
                {
                    float deltaX = speedClouds[i] * dt.asSeconds();
                    Vector2f currPos = spriteClouds[i].getPosition();
                    currPos.x += deltaX;
                    spriteClouds[i].setPosition(currPos);

                    if (currPos.x < -300)
                    {
                        activeClouds[i] = false;
                    }
                }
            }

            // 벌
            if (!beeActive)
            {
                // 벌 초기화
                beeSpeed = gen() % 200 + 200;
                beeSpeed *= -1.f;
                float y = gen() % 500 + 500;
                spriteBee.setPosition(2000, y);
                beeActive = true;
            }
            else
            {
                // 벌 이동 + 화면 밖으로 나갔는지 테스트
                float deltaX = beeSpeed * dt.asSeconds();
                Vector2f currPos = spriteBee.getPosition();
                currPos.x += deltaX;
                spriteBee.setPosition(currPos);

                // 화면 밖으로 나갔는지 테스트
                if (currPos.x < -100)
                {
                    beeActive = false;
                }
            }
            stringstream ss;
            ss << "Score = " << score;
            textScore.setString(ss.str());

            timeRemaining -= dt.asSeconds();
            timerBarSize.x = timeRemaining * timerBarWidthPerSecond;
            timerBar.setSize(timerBarSize);

            if (timeRemaining < 0.f)
            {
                oot.play();

                timerBarSize.x = 0.f;
                timerBar.setSize(timerBarSize);

                isPause = true;

                textMessage.setString("Out of time!!");
                GoCenterText(textMessage);
            }
            for (int i = 0; i < countBranches; ++i)
            {
                float height = 150 * i;

                switch (sideBranches[i])
                {
                case side::LEFT:
                    spriteBranches[i].setPosition(610, height);
                    spriteBranches[i].setRotation(180);
                    break;
                case side::RIGHT:
                    spriteBranches[i].setPosition(1330, height);
                    spriteBranches[i].setRotation(0);
                    break;
                default:
                    spriteBranches[i].setPosition(-2000, -2000);
                    break;
                }
            }

            if (logActive)
            {
                Vector2f logPos = spriteLog.getPosition();
                logPos.x += logSpeedX * dt.asSeconds();
                logPos.y += logSpeedY * dt.asSeconds();
                spriteLog.setPosition(logPos);

                if (logPos.x < -100 || logPos.x > 2000)
                {
                    logActive = false;
                    spriteLog.setPosition(810, 720);
                }
            }

            if (sideBranches[countBranches - 1] == playerSide)
            {
                isPause = true;
                acceptInput = false;

                spritePlayer.setPosition(2000, 660);
                spriteRIP.setPosition(525, 760);

                textMessage.setString("SQUISHED!");
                GoCenterText(textMessage);
                death.play();
            }
        }
        // 그리기
        // world
        window.clear();

        window.draw(spriteBackground);
        for (int i = 0; i < countClouds; i++)
        {
            window.draw(spriteClouds[i]);
        }

        window.draw(spriteTree);
        for (int i = 0; i < countBranches; i++)
        {
            window.draw(spriteBranches[i]);
        }
        window.draw(spritePlayer);
        window.draw(spriteAxe);
        window.draw(spriteLog);
        window.draw(spriteRIP);
        window.draw(spriteBee);

        // ui
        window.draw(textScore);
        if (isPause)
        {
            window.draw(textMessage);
        }
        window.draw(timerBar);
        window.display();
    }
    return 0;
}
