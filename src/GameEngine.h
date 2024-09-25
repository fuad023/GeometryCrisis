#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <fstream>
#include <string>
#include <random>
#include <ctime>
#include "EntityManager.h"

struct WindowConfig { unsigned int width, height, frameLimit; bool mode; };
struct PlayerConfig { double SR, CR, S, OT; int FR, FG, FB, OR, OG, OB, V; };
struct EnemyConfig  { double SR, CR, SMIN, SMAX, OT; int OR, OG, OB, VMIN, VMAX, L, SI; };
struct BulletConfig { double SR, CR, S, OT; int FR, FG, FB, OR, OG, OB, V, L, SI; };

class GameEngine
{
    static GameEngine *instance;
    Assets assets;

    Window                  m_window;
    bool                    m_Fullscreen = false;
    EntityManager           m_entityManager;
    std::shared_ptr<Entity> m_player;

    WindowConfig            m_windowConfig;
    PlayerConfig            m_playerConfig;                                              
    EnemyConfig             m_enemyConfig;                               
    BulletConfig            m_bulletConfig;
    Text                    m_text;

    int                     m_score               = 0;
    int                     m_currentFrame        = 0;
	int						m_Timer				  = 0;
    int                     m_lastEnemySpawnTime  = 0;
    int                     m_lastBulletSpawnTime = 0;
    int                     m_spawnIntervalEnemy;
	int                     m_SpecialWeaponCap    = 0;
    int                     m_powerMoveInterval   = 60*20;
	int                     m_SpecialWeaponEffectTimer = 0;
    int                     m_frameSpecialWeapon  = 0;
    int                     m_spawnIntervalBullet = 100;
    bool                    m_running             = true;
    bool                    m_paused              = false;
    Vec2                    m_mouseLocation       = Vec2(0.0);

    int                     m_difficultyFrame     = 0;
    int                     m_difficultyFrame2    = 0;
    bool                    m_difficultyOne       = true;
    bool                    m_difficultyTwo       = false;
    bool                    m_difficultyThree     = false;
    bool                    m_difficultyFour      = false;
    bool                    m_difficultyFive      = false;
    bool                    m_ultimateDifficulty  = false;
    int                     m_lostTimer           = 0;
    bool                    m_lost                = false;
    double                  k_died                = 0;
    double                  k_intro               = 0;

    void initConfig (const std::string& config);
    void initWindow ();
    void initAssets();

    void sEnemySpawner          ();
    void spawnPosVel            ( int& xPos, int& yPos, int& speed, int& vertices, Vec2& velDir, Vec2& velMag );
    void spawnEnemy             ();
    void spawnVel               ( std::shared_ptr<Entity> entity, Vec2& velDir, Vec2& velMag, double angle );
    void spawnSmallEnemies      ( std::shared_ptr<Entity> entity );

    void sCollision             ();
    bool collisionEntity        ( std::shared_ptr<Entity> head, std::shared_ptr<Entity> tail ); // helper function for Collision System
    void collisionBounceBoarder ( std::shared_ptr<Entity> entity );
    void collisionInputBound    ( std::shared_ptr<Entity> entity );
    void sMovement              ();
    void inputMovement          ( std::shared_ptr<Entity> entity );
    void sLifespan              ();
    void sRender                ();
    void sDifficulty            ();
    void sLost                  ();


    void sKeyPress(unsigned char Key, int MousePosX, int MousePosY);
    void sKeyRelease(unsigned char Key, int MousePosX, int MousePosY);
    void sMouseState(int button, int state, int x, int y);
    void sMousePos(int x, int y);
    //void sMouseDrag(int x, int y);
    void idleFunc();
    void GameLoop();



    void spawnPlayer            ();
	void smallPlayer            ();
    void sAttack                ( const Vec2& direction );
    void spawnBullet            ( std::shared_ptr<Entity> entity, const Vec2& mousePos );
    void initSpecialWeapon      ();
    void spawnSpecialWeapon     ();


    void Rotate(const Vec2 pos, double degree);
    void UnRotate();
    static void setKeybordFunction(unsigned char key, int x, int y) { instance->sKeyPress(key, x, y); }
    static void setKeybordUpFunction(unsigned char key, int x, int y) { instance->sKeyRelease(key, x, y); }
    static void setMouseState(int button, int state, int x, int y) { instance->sMouseState(button, state, x, y); }
    static void setMousePos(int x, int y) { instance->sMousePos(x, y); }
    //static void setMouseDrag(int x, int y) { instance->sMouseDrag(x, y); }
    static void setIdleFunc() { instance->idleFunc(); }
    static void setDisplayFunction() { instance->GameLoop(); }

    int    gameScene   = 1;
    bool   newGame     = false;
    bool   credits     = false;
    bool   exit        = false;
    bool   retry       = false;
    bool   menu        = false;
    int    menuIndex   = 101;
    int    menuButton  = 3;
    double alphaOffset = 0.0;
    void initMenuScene() 
    {
        for (auto& e : m_entityManager.getEntities()) { e->destroy(); }
        alphaOffset = 0.099;
        assets.playMenuMusic();
    }
    void initPlayScene() 
    {
        for (auto& e : m_entityManager.getEntities()) { e->destroy(); }
		spawnPlayer();
		m_SpecialWeaponCap = 0;
		m_difficultyFrame = 0;
        alphaOffset = 1.0;
        k_died = 0;
        assets.playGameMusic();
		initAssets();
		m_Timer = 0;
		m_paused = false;
		m_lost = false;
    }
    void background();

public:
    GameEngine() {}
    GameEngine(int argc, char* argv[]) { instance = this; glutInit(&argc, argv); }
    
    void init(const std::string& path);
    void run()
    {
        glutDisplayFunc(setDisplayFunction);
        glutKeyboardFunc(setKeybordFunction);
        glutKeyboardUpFunc(setKeybordUpFunction);
        glutMouseFunc(setMouseState);
        glutPassiveMotionFunc(setMousePos);
        //glutMotionFunc(setMouseDrag);
        glutIdleFunc(setIdleFunc);
        glutMainLoop();
    }
    
};


#endif //GAME_ENGINE_H