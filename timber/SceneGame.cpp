#include "SceneGame.h"
#include "ResourceMgr.h"
#include "RandomMgr.h"
#include "Framework.h"

float beeSpeed = 0.f;
bool beeActive = false;

const int countClouds = 3;
bool activeClouds[countClouds];
float speedClouds[countClouds];

void SceneGame::Init()
{
	spriteBackground.setTexture(*ResourceMgr::instance()->GetTexture("MAINBGTEX"));
	spritePlayer.setTexture(*ResourceMgr::instance()->GetTexture("MAINPLAYERTEX"));
	spriteCloud.setTexture(*ResourceMgr::instance()->GetTexture("MAINCLOUDTEX"));
	spriteBee.setTexture(*ResourceMgr::instance()->GetTexture("MAINBEETEX"));
	spriteBee.setPosition(500, 500);
	spriteCloud.setPosition(300, 300);
}

void SceneGame::Release()
{
}

void SceneGame::Start()
{
}

void SceneGame::End()
{
}

void SceneGame::Update(float dt)
{
	if (!beeActive)
	{
		beeSpeed = RandomMgr::Get(200, 200);
		beeSpeed *= -1.f;
		float y = RandomMgr::Get(500, 500);
		spriteBee.setPosition(2000, y);
		beeActive = true;
	}
	else
	{
		float deltaX = beeSpeed * dt;
		Vector2f currPos = spriteBee.getPosition();
		currPos.x += deltaX;
		spriteBee.setPosition(currPos);

		// ȭ�� ������ �������� �׽�Ʈ
		if (currPos.x < -100)
		{
			beeActive = false;
		}
	}
	
}

void SceneGame::Draw(sf::RenderWindow* window)
{
	window->draw(spriteBackground);
	window->draw(spriteBee);
}
