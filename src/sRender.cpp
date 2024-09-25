#include "GameEngine.h"

void GameEngine::sRender()
{
    m_window.clear();

    if (gameScene == 1) // menuScene
    {
        background();

             if (newGame)                       { m_window.Render(assets.menuPlay);    }
        else if (credits)                       { m_window.Render(assets.menuCredits); }
        else if (exit)                          { m_window.Render(assets.menuQuit);    }
        else if (!newGame && !credits && !exit) { m_window.Render(assets.menuDefault); }
        m_window.Render(assets.menuLogo);
    }

    if (gameScene == 2) // creditsScene
    {
        background();
        m_window.Render(assets.credits);
        m_window.Render(assets.menuLogo);
    }

    if (gameScene == 0) // playScene
    {
        for (auto& e : m_entityManager.getEntities())
        {
            if ( (!e->cInput) && e->cShape && e->cTransform)
            {
                e->cTransform->angle += 3;
                
                e->cShape->circle.setPos( e->cTransform->pos );
                if (m_paused)
                {
                    e->cShape->circle.setFillColour   ( {e->cShape->FILL.r,    e->cShape->FILL.g,    e->cShape->FILL.b,    e->cShape->FILL.a*0.1} );
                    e->cShape->circle.setOutlineColour( {e->cShape->OUTLINE.r, e->cShape->OUTLINE.g, e->cShape->OUTLINE.b, e->cShape->OUTLINE.a*0.1} );
                }
                else
                {
                    e->cShape->circle.setFillColour   ( {e->cShape->FILL.r,    e->cShape->FILL.g,    e->cShape->FILL.b,    e->cShape->FILL.a} );
                    e->cShape->circle.setOutlineColour( {e->cShape->OUTLINE.r, e->cShape->OUTLINE.g, e->cShape->OUTLINE.b, e->cShape->OUTLINE.a} );
                }
                //e->cShape->circle.setRotation( e->cTransform->angle );
                Rotate(e->cTransform->pos, e->cTransform->angle);
                m_window.Render( e->cShape->circle );
                UnRotate();
            }
        }

        m_player->cTransform->angle += 3;
        m_player->cShape->circle.setPos( m_player->cTransform->pos );
        if (m_paused)
        {
            m_player->cShape->circle.setFillColour   ( {m_player->cShape->FILL.r,    m_player->cShape->FILL.g,    m_player->cShape->FILL.b,    m_player->cShape->FILL.a*0.1} );
            m_player->cShape->circle.setOutlineColour( {m_player->cShape->OUTLINE.r, m_player->cShape->OUTLINE.g, m_player->cShape->OUTLINE.b, m_player->cShape->OUTLINE.a*0.1} );
        }
        else
        {
            m_player->cShape->circle.setFillColour   ( {m_player->cShape->FILL.r,    m_player->cShape->FILL.g,    m_player->cShape->FILL.b,    m_player->cShape->FILL.a} );
            m_player->cShape->circle.setOutlineColour( {m_player->cShape->OUTLINE.r, m_player->cShape->OUTLINE.g, m_player->cShape->OUTLINE.b, m_player->cShape->OUTLINE.a} );
        }
        Rotate(m_player->cTransform->pos, m_player->cTransform->angle);
        m_window.Render(m_player->cShape->circle);
        UnRotate();

        std::string score = "Score: " + std::to_string(m_player->cScore->score);
        m_text.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
        m_text.setPos(Vec2(8, 25)); m_text.setText(score); m_text.setColour( Vec4(255) );
        m_window.Render(m_text);

        std::string time = "Time: " + std::to_string((int)m_Timer/m_windowConfig.frameLimit);
        m_text.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
        m_text.setPos(Vec2(m_window.width-110, 25)); m_text.setText(time); m_text.setColour( Vec4(255) );
        m_window.Render(m_text);

		std::string Bankai = "StarTroops: " + std::to_string((m_SpecialWeaponCap * 100) / m_powerMoveInterval) + " %";
		m_text.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
		m_text.setPos(Vec2(8, m_window.height-14)); m_text.setText(Bankai); m_text.setColour(Vec4(255));
		m_window.Render(m_text);

        if (m_lost)
        {
            m_text.setPos(Vec2(m_window.width/2 - 57, m_window.height/2 - 70)); m_text.setText("YOU DIED");
            m_text.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
            m_text.setColour( Vec4(255*0.67*k_died, 0, 0) );
            m_window.Render(m_text);

            if (retry)
            {
                Quad test(Vec2(m_window.width/2 -100, m_window.height/2 - 17), Vec2(120, 30));
                test.setFillColour(Vec4(255*k_died)); test.setOutlineColour(Vec4(255*k_died));
                m_window.Render(test);
            }

            m_text.setPos(Vec2(m_window.width/2 - 27 -100, m_window.height/2 - 10)); m_text.setText("Retry");
            m_text.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
            m_text.setColour( Vec4(255*k_died) ); if (retry) { m_text.setColour( Vec4(0) ); }
            m_window.Render(m_text);

            if (menu)
            {
                Quad test(Vec2(m_window.width/2 +100, m_window.height/2 - 17), Vec2(120, 30));
                test.setFillColour(Vec4(255*k_died)); test.setOutlineColour(Vec4(255*k_died));
                m_window.Render(test);
            }

            m_text.setPos(Vec2(m_window.width/2 - 28 +100, m_window.height/2 - 10)); m_text.setText("Menu");
            m_text.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
            m_text.setColour( Vec4(255*k_died) ); if (menu) { m_text.setColour( Vec4(0) ); }
            m_window.Render(m_text);
        }

        if (m_paused)
        {
            m_text.setPos(Vec2(m_window.width/2 - 67, m_window.height/2 - 80)); m_text.setText("Game Paused");
            m_text.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
            m_text.setColour( Vec4(255) );
            m_window.Render(m_text);

            Quad test1(Vec2(m_window.width/2 -100, m_window.height/2 - 7), Vec2(120, 30));
            test1.setFillColour(Vec4(255, 255, 255, 255*0.1)); test1.setOutlineColour(Vec4(255, 255, 255, 255*0.1));
            if (retry) { test1.setFillColour(Vec4(255)); test1.setOutlineColour(Vec4(255)); }
            m_window.Render(test1);

            m_text.setPos(Vec2(m_window.width/2 - 40 -100, m_window.height/2 - 0)); m_text.setText("Resume");
            m_text.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
            m_text.setColour( Vec4(255, 255, 255, 255*0.67) ); if (retry) { m_text.setColour( Vec4(0) ); }
            m_window.Render(m_text);

            Quad test2(Vec2(m_window.width/2 +100, m_window.height/2 - 7), Vec2(120, 30));
            test2.setFillColour(Vec4(255, 255, 255, 255*0.1)); test2.setOutlineColour(Vec4(255, 255, 255, 255*0.1));
            if (menu) { test2.setFillColour(Vec4(255)); test2.setOutlineColour(Vec4(255)); }
            m_window.Render(test2);

            m_text.setPos(Vec2(m_window.width/2 - 28 +100, m_window.height/2 - 0)); m_text.setText("Menu");
            m_text.setFont(GLUT_BITMAP_TIMES_ROMAN_24);
            m_text.setColour( Vec4(255, 255, 255, 255*0.67) ); if (menu) { m_text.setColour( Vec4(0) ); }
            m_window.Render(m_text);
        }
    }
}

void GameEngine::background()
{
    if (m_entityManager.getEntities("enemy").size() > 20)
    {
        auto e = m_entityManager.getEntities("enemy").front();
        if (e->isActive())
        {
            spawnSmallEnemies(e);
            e->destroy();
        }
    }

    for (auto& e : m_entityManager.getEntities())
    {
        if ( (!e->cInput) && e->cShape && e->cTransform)
        {

            e->cShape->circle.setPos( e->cTransform->pos );  
            e->cTransform->angle += 3;
            //e->cShape->circle.setRotation( e->cTransform->angle );
            Rotate(e->cTransform->pos, e->cTransform->angle);
            m_window.Render( e->cShape->circle );
            UnRotate();
        }
    }
}