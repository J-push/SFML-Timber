#include "SceneTitle.h"
#include "ResourceMgr.h"


void SceneTitle::Init()
{
	test.setFont(*ResourceMgr::instance()->GetFont("MAINFONT"));
	test.setString("TITLE");
	test.setFillColor(Color::White);
	test.setCharacterSize(100);
	test.setPosition(0, 0);

	test.setFont(*ResourceMgr::instance()->GetFont("MAINFONT"));
	test.setString("MAIN");
	test.setFillColor(Color::White);
	test.setCharacterSize(100);
	test.setPosition(0, 0);
	
}

void SceneTitle::Release()
{

}

void SceneTitle::Start()
{
	test.setCharacterSize(200);
	test.setPosition(0, 0);
}

void SceneTitle::End()
{

}

void SceneTitle::Update(float dt)
{
	if (Keyboard::isKeyPressed(Keyboard::Return))
	{
		mgr.ChangeScene(Scenes::MENU);
	}
}

void SceneTitle::Draw(sf::RenderWindow* window)
{
	window->draw(test);
}
