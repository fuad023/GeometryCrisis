#pragma once

#include <windows.h>
#include <mmsystem.h>

#include "AFK.h"

class Assets
{
public:
    
    Image menuDefault;
    Image menuPlay;
    Image menuCredits;
    Image menuQuit;
    Image menuLogo;

    Image credits;

    void load()
    {
        menuDefault = Image(Vec2(820, 768), "assets\\texture\\menu\\default.png");
        menuPlay    = Image(Vec2(820, 768), "assets\\texture\\menu\\play.png");
        menuCredits = Image(Vec2(820, 768), "assets\\texture\\menu\\credits.png");
        menuQuit    = Image(Vec2(820, 768), "assets\\texture\\menu\\quit.png");
        menuLogo    = Image(Vec2(683, 768), "assets\\texture\\menu\\logo2.png");
        menuLogo.setPos(Vec2(1024.5, 384));

        credits     = Image(Vec2(820, 768), "assets\\texture\\credits\\creditsFinal.png");
    }

    void playMenuMusic()
    {
        PlaySound("assets\\music\\C418 - Sweden - Minecraft Volume Alpha.wav", NULL, SND_FILENAME | SND_ASYNC);
    }

    void playGameMusic()
    {
        PlaySound("assets\\music\\Sewerslvt - Yandere Complex.wav", NULL, SND_FILENAME | SND_ASYNC);
    }

	void playDeathMusic()
	{
		PlaySound("assets\\music\\DarkSouls_You Died.wav", NULL, SND_FILENAME | SND_ASYNC);
	}
};