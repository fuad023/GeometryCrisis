#include "GameEngine.h"

GameEngine *GameEngine::instance = NULL;

void GameEngine::init(const std::string& path)
{
    initConfig(path);
    initWindow();
    initAssets();

    // set up initial values
    //spawnPlayer();
    initMenuScene();
    m_spawnIntervalEnemy = m_enemyConfig.SI;
    m_spawnIntervalBullet = m_bulletConfig.SI;
}
void GameEngine::initConfig(const std::string& config)
{
    std::ifstream fin(config);
    std::string tag;
    while (fin && fin >> tag) 
    {
        if (tag == "Window")
        {
            fin >> m_windowConfig.width
                >> m_windowConfig.height
                >> m_windowConfig.frameLimit
                >> m_windowConfig.mode;
        }

        if (tag == "Player")
        {
            fin >> m_playerConfig.SR    // Shape Radius         double    
                >> m_playerConfig.CR    // Collision Radius     double    
                >> m_playerConfig.S     // Speed                double 
                >> m_playerConfig.FR    // Fill Red             int    
                >> m_playerConfig.FG    // Fill Green           int    
                >> m_playerConfig.FB    // Fill Blue            int    
                >> m_playerConfig.OR    // Outline Red          int    
                >> m_playerConfig.OG    // Outline Green        int    
                >> m_playerConfig.OB    // Outline Blue         int    
                >> m_playerConfig.OT    // Outline Thickness    double    
                >> m_playerConfig.V;    // Shape Vertices       int    
        }

        if (tag == "Enemy")
        {
            fin >> m_enemyConfig.SR     // Shape Radius         double    
                >> m_enemyConfig.CR     // Collision Radius     double    
                >> m_enemyConfig.SMIN   // Min Speed            double
                >> m_enemyConfig.SMAX   // Max Speed            double
                >> m_enemyConfig.OR     // Outline Red          int
                >> m_enemyConfig.OG     // Outline Green        int
                >> m_enemyConfig.OB     // Outline Blue         int
                >> m_enemyConfig.OT     // Outline Thickness    double
                >> m_enemyConfig.VMIN   // Min Vertices         int
                >> m_enemyConfig.VMAX   // Max Vertices         int
                >> m_enemyConfig.L      // Small Lifespan       int
                >> m_enemyConfig.SI;    // Spawn Interval       int             
        } 

        if (tag == "Bullet")
        {
            fin >> m_bulletConfig.SR    // Shape Radius         double
                >> m_bulletConfig.CR    // Collision Radius     double
                >> m_bulletConfig.S     // Speed                double
                >> m_bulletConfig.FR    // Fill Red             int
                >> m_bulletConfig.FG    // Fill Green           int 
                >> m_bulletConfig.FB    // Fill Blue            int    
                >> m_bulletConfig.OR    // Outline Red          int    
                >> m_bulletConfig.OG    // Outline Green        int    
                >> m_bulletConfig.OB    // Outline Blue         int    
                >> m_bulletConfig.OT    // Outline Thickness    double    
                >> m_bulletConfig.V     // Shape Vertices       int
                >> m_bulletConfig.L     // Lifespan             int
                >> m_bulletConfig.SI;   // Spawn Interval       int   
        }
    }  
}
void GameEngine::initWindow()
{
    m_window.init(m_windowConfig.width, m_windowConfig.height, "GeometryCrisis");
    m_window.create( m_windowConfig.mode );
}
void GameEngine::initAssets() { assets.load(); }
void GameEngine::spawnPlayer()
{
    auto entity        = m_entityManager.addEntity("player");
    entity->cTransform = std::make_shared<CTransform> ( Vec2(m_windowConfig.width/2, m_windowConfig.height/2), Vec2(m_playerConfig.S), Vec2(0.0), 0.0 );
    entity->cShape     = std::make_shared<CShape> ( m_playerConfig.SR, m_playerConfig.V, Vec4(m_playerConfig.FR, m_playerConfig.FG, m_playerConfig.FB, 255), Vec4(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB, 255), m_playerConfig.OT );
    entity->cCollision = std::make_shared<CCollision> ( m_playerConfig.CR );  
    entity->cInput     = std::make_shared<CInput> ();
    entity->cScore     = std::make_shared<CScore> (0);

    m_player = entity;
}

void GameEngine::sAttack(const Vec2& direction)
{
    if ( (m_currentFrame - m_lastBulletSpawnTime >= m_spawnIntervalBullet) && m_player->cInput->shoot )
    {
        spawnBullet(m_player, direction);
    }
}
void GameEngine::spawnBullet(std::shared_ptr<Entity> player, const Vec2& target)
{
    // velocity direction and magnitude calculation
    Vec2 velMag, velDir = target - player->cTransform->pos;
    velDir.normalize();

    velMag.assign(velDir).scale(m_bulletConfig.S).abs();
    velDir.unit(velDir);

    auto e = m_entityManager.addEntity("bullet");
    e->cTransform = std::make_shared<CTransform> (m_player->cTransform->pos, velMag, velDir, 0.0);
    e->cShape     = std::make_shared<CShape>     (m_bulletConfig.SR, m_bulletConfig.V, Vec4(m_bulletConfig.FR, m_bulletConfig.FG, m_bulletConfig.FB, 255), Vec4(m_bulletConfig.OR, m_bulletConfig.OG, m_bulletConfig.OB, 255), m_bulletConfig.OT);
    e->cCollision = std::make_shared<CCollision> (m_bulletConfig.CR); 
    e->cLifeSpan  = std::make_shared<CLifeSpan>  (m_bulletConfig.L);
    
    m_lastBulletSpawnTime = m_currentFrame;
}
double frame1 = 0, frame2 = 0;
void GameEngine::idleFunc()
{
    frame2 = glutGet(GLUT_ELAPSED_TIME);
    if (frame2 - frame1 > 1000.0/m_windowConfig.frameLimit) { frame1 = frame2; glutPostRedisplay(); }
}
void GameEngine::GameLoop()
{
    m_entityManager.update();

    if (gameScene == 1 || gameScene == 2) // menuScene and creditsScene background
    {
        sEnemySpawner();
        sCollision();
        sMovement();
        sLifespan();
    }


    if (gameScene == 0) // playScene
    {
        if (m_lost) { sLost(); }
        else if (!m_paused)
        {
			initSpecialWeapon();
            sAttack( m_mouseLocation );
            sEnemySpawner();
            sCollision();
            sMovement();
            sLifespan();
            //sDifficulty();
			++m_Timer;
        }
    }

    sRender();
    ++m_currentFrame; 
    glutSwapBuffers(); 
}

void GameEngine::sEnemySpawner()
{
    if ( m_currentFrame - m_lastEnemySpawnTime >= m_spawnIntervalEnemy) {
        spawnEnemy();
    }

}
void GameEngine::spawnPosVel(int& xPos, int& yPos, int& speed, int& vertices, Vec2& velDir, Vec2& velMag)
{
    srand(time(0));
    std::random_device r;
    std::mt19937 generator(r());
    int yMin = - (2*m_enemyConfig.CR); int yMax =   (2*m_enemyConfig.CR) + m_windowConfig.height;
    int xMin = - (2*m_enemyConfig.CR); int xMax =   (2*m_enemyConfig.CR) + m_windowConfig.width;
            
    int rd = rand() % 2;
    if (rd)
    {
        std::uniform_int_distribution<int> ydiff(yMin, yMax);
        yPos = ydiff(generator);
        
        do {                                                         // xPos out position
            std::uniform_int_distribution<int> xdiff(xMin, xMax);
            xPos = xdiff(generator);
        } while ( (-m_enemyConfig.CR) <= xPos && xPos <= (m_enemyConfig.CR + m_windowConfig.width) );
    }

    else
    {
        std::uniform_int_distribution<int> xdiff(xMin, xMax);
        xPos = xdiff(generator);

        do {                                                         // yPos out position
            std::uniform_int_distribution<int> ydiff(yMin, yMax);
            yPos = ydiff(generator);
        } while ( (-m_enemyConfig.CR) <= yPos && yPos <= (m_enemyConfig.CR + m_windowConfig.height) );
    }

/*
    if (rd)
    {
        int yMin = - (2*m_enemyConfig.CR);
        int yMax =   (2*m_enemyConfig.CR) + m_windowConfig.height;
        int yDiff = yMax - yMin + 1;
        yPos = ( rand() % yDiff ) + yMin;
        
        do {                                                         // xPos out
            int xMin = - (2*m_enemyConfig.CR);
            int xMax =   (2*m_enemyConfig.CR) + m_windowConfig.width;
            int xDiff = xMax - xMin + 1;
            xPos = ( rand() % xDiff ) + xMin;
        } while ( (-m_enemyConfig.CR) <= xPos && xPos <= (m_enemyConfig.CR + m_windowConfig.width) );
    }

    else
    {
        int xMin = - (2*m_enemyConfig.CR);
        int xMax =   (2*m_enemyConfig.CR) + m_windowConfig.width;
        int xDiff = xMax - xMin + 1;
        xPos = ( rand() % xDiff ) + xMin;

        do {                                                         // yPos out
            int yMin = - (2*m_enemyConfig.CR);
            int yMax =   (2*m_enemyConfig.CR) + m_windowConfig.height;
            int yDiff = yMax - yMin + 1;
            yPos = ( rand() % yDiff ) + yMin;
        } while ( (-m_enemyConfig.CR) <= yPos && yPos <= (m_enemyConfig.CR + m_windowConfig.height) );
    }
*/

    int diffVer = m_enemyConfig.VMAX - m_enemyConfig.VMIN + 1;      
    vertices = ( rand() % diffVer ) + m_enemyConfig.VMIN;        // entity vertices
    //std::uniform_int_distribution<int> diffVer(m_enemyConfig.VMIN, m_enemyConfig.VMAX);
    //vertices = diffVer(generator);

    int diffSpeed = m_enemyConfig.SMAX - m_enemyConfig.SMIN + 1;
    speed = ( rand() % diffSpeed ) + m_enemyConfig.SMIN;         // entity speed

    int degree = rand() % 360;
    //std::uniform_int_distribution<int> dig(1, 360);
    //int degree = dig(generator);
    velDir = velMag.unit(-degree);
    velMag.scale(speed).abs();                                   // velocity magnitude
    velDir.unit(velDir);                                         // velocity direction

}
void GameEngine::spawnEnemy()
{
    int xPos, yPos, vertices, speed;
    Vec2 velDir, velMag;
    spawnPosVel(xPos, yPos, speed, vertices, velDir, velMag); 

    auto e = m_entityManager.addEntity("enemy");
    e->cTransform = std::make_shared<CTransform> ( Vec2(xPos, yPos), velMag, velDir, 0.0 ); e->cTransform->speed = speed;
    if (vertices == 3) e->cShape = std::make_shared<CShape> ( m_enemyConfig.SR, vertices, Vec4(244, 100, 119, 255*alphaOffset), Vec4(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB, 255*alphaOffset), m_enemyConfig.OT );
    if (vertices == 4) e->cShape = std::make_shared<CShape> ( m_enemyConfig.SR, vertices, Vec4(156, 237,  64, 255*alphaOffset), Vec4(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB, 255*alphaOffset), m_enemyConfig.OT );
    if (vertices == 5) e->cShape = std::make_shared<CShape> ( m_enemyConfig.SR, vertices, Vec4(  9,  84, 124, 255*alphaOffset), Vec4(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB, 255*alphaOffset), m_enemyConfig.OT );
    if (vertices == 6) e->cShape = std::make_shared<CShape> ( m_enemyConfig.SR, vertices, Vec4(201, 205, 219, 255*alphaOffset), Vec4(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB, 255*alphaOffset), m_enemyConfig.OT );
    if (vertices == 7) e->cShape = std::make_shared<CShape> ( m_enemyConfig.SR, vertices, Vec4(177,  73, 229, 255*alphaOffset), Vec4(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB, 255*alphaOffset), m_enemyConfig.OT );
    //if (vertices == 8) e->cShape = std::make_shared<CShape> ( m_enemyConfig.SR, vertices, Vec4( 51,  23, 117, 255), Vec4(m_enemyConfig.OR, m_enemyConfig.OG, m_enemyConfig.OB, 255), m_enemyConfig.OT );
    e->cCollision = std::make_shared<CCollision> (m_enemyConfig.CR);
    e->cScore = std::make_shared<CScore> (100*vertices);
    
    // record when the most recent enemy was spawned
    m_lastEnemySpawnTime = m_currentFrame;
}
void GameEngine::spawnVel(std::shared_ptr<Entity> entity, Vec2& velDir, Vec2& velMag, double degree)
{
    int speed = entity->cTransform->speed;      // entity speed
    
    velDir = velMag.unit(-degree);
    velMag = velDir;
    velMag.scale(speed).abs();                  // velocity magnitude
    velDir.unit(velDir);                        // velocity direction

}
void GameEngine::spawnSmallEnemies(std::shared_ptr<Entity> eBig)
{
    double angle = 360;
    int vertices = eBig->cShape->circle.getVertex();
    int count = 0;
    double diff = (1.0/vertices) * angle;
    
    while (count < vertices)
    {        
        Vec2 velDir, velMag;
        spawnVel(eBig, velDir, velMag, angle);
            
        auto eSmall = m_entityManager.addEntity("enemySmall");
        eSmall->cTransform = std::make_shared<CTransform> ( eBig->cTransform->pos, velMag, velDir, 0.0 );
        eSmall->cShape     = std::make_shared<CShape>     ( eBig->cShape->circle.getRadius()/2, vertices, eBig->cShape->FILL, eBig->cShape->OUTLINE, eBig->cShape->circle.getOutlineThickness() );
        eSmall->cCollision = std::make_shared<CCollision> ( eBig->cCollision->r/2 );
        eSmall->cScore     = std::make_shared<CScore>     ( eBig->cScore->score*2 );
        eSmall->cLifeSpan  = std::make_shared<CLifeSpan>  ( m_enemyConfig.L );
        angle -= diff;
        ++count;
    }
}

void GameEngine::sCollision()
{
    for ( auto b : m_entityManager.getEntities("bullet") ) { // dave did this call by value, not reference
        for ( auto e : m_entityManager.getEntities("enemy") ) {
            if ( b->isActive() && e->isActive() && collisionEntity(b, e) ) { spawnSmallEnemies(e); b->destroy(); e->destroy(); m_player->cScore->score += e->cScore->score; }
        }
        for ( auto e : m_entityManager.getEntities("enemySmall") ) {
            if ( b->isActive() && e->isActive() && collisionEntity(b, e) ) { b->destroy(); e->destroy(); m_player->cScore->score += e->cScore->score; }
        }
    }
	for (auto b : m_entityManager.getEntities("smallP")) {
		for (auto e : m_entityManager.getEntities("enemy")) {
			if (b->isActive() && e->isActive() && collisionEntity(b, e)) { spawnSmallEnemies(e); b->destroy(); e->destroy(); }
		}
	}

    // enemy m_window border bound
    for (auto& e : m_entityManager.getEntities("enemy") ) {
        collisionBounceBoarder(e);
    }

    // m_player m_window border bound
    if (gameScene == 0) // playScene
    {
        if ( m_player->cInput ) {
            collisionInputBound(m_player);
        }
        for ( auto e : m_entityManager.getEntities("enemy") ) {
            if ( m_player->isActive() && e->isActive() && collisionEntity(m_player, e) ) 
            { 
				m_score = m_player->cScore->score; m_lost = true; m_lostTimer = glutGet(GLUT_ELAPSED_TIME) / 1000.0; assets.playDeathMusic();
            }
        }
    }
}
bool GameEngine::collisionEntity (std::shared_ptr<Entity> head, std::shared_ptr<Entity> tail)
{
    Vec2 headPos(head->cTransform->pos.x, head->cTransform->pos.y);
    Vec2 tailPos(tail->cTransform->pos.x, tail->cTransform->pos.y);
    double distSq = headPos.distSq(tailPos);
    double distRadiusSq = (head->cCollision->r + tail->cCollision->r) * (head->cCollision->r + tail->cCollision->r);
    return ( (distRadiusSq >= distSq) ?  true : false ); // (r1 + r2)^2  >= (x1 - x2)^2 + (y1 - y2)^2
}
void GameEngine::collisionBounceBoarder( std::shared_ptr<Entity> e )
{    
    if ( e->cTransform->pos.x + e->cCollision->r >= m_windowConfig.width )            // Right side
    {                                                                                   
        if (e->cTransform->velAxis.x > 0) { e->cTransform->velAxis.x *= -1; }       
    }                                                                                                        
    if ( e->cTransform->pos.x - e->cCollision->r <= 0 )                               // Left side
    {                                                                                                                    
        if (e->cTransform->velAxis.x < 0) { e->cTransform->velAxis.x *= -1; }       
    }                                                                                                                     
    if ( e->cTransform->pos.y + e->cCollision->r >= m_windowConfig.height )           // Down side
    {                                                                                                                 
        if (e->cTransform->velAxis.y > 0) { e->cTransform->velAxis.y *= -1; }           
    }                                                                                                                   
        if ( e->cTransform->pos.y - e->cCollision->r <= 0 )                           // Up side
    {                                                                                                                    
        if (e->cTransform->velAxis.y < 0) { e->cTransform->velAxis.y *= -1; }
    }
}
void GameEngine::collisionInputBound( std::shared_ptr<Entity> entity )
{
    if ( entity->cTransform->pos.x + entity->cCollision->r >= m_windowConfig.width )   // Right side
    {                                                                                                                   
        entity->cInput->right = false;
    }                                                                                                        
    if ( entity->cTransform->pos.x - entity->cCollision->r <= 0 )                      // Left side
    {                                                                                                                   
        entity->cInput->left = false;
    }                                                                                                                     
    if ( entity->cTransform->pos.y + entity->cCollision->r >= m_windowConfig.height )  // Down side
    {                                                                                                                   
        entity->cInput->down = false;
    }                                                                                                                   
    if ( entity->cTransform->pos.y - entity->cCollision->r <= 0 )                      // Up side
    {                                                                                                                   
        entity->cInput->up = false;
    }
}

void GameEngine::sLost()
{
    double timeDelay = 3.0;
    bool stop = true;
    double timer = glutGet(GLUT_ELAPSED_TIME)/1000.0 - m_lostTimer;
    if (timer <= timeDelay)
    {
        stop = false;
    }
    if (!stop)
    {
        k_died = timer / timeDelay;
        double k = 1.0 - (0.9*k_died);
        //std::cout << k << " " << k_died << std::endl;
        for (auto& e : m_entityManager.getEntities())
        {
            if (!(e->tag() == "player"))
            {
                e->cShape->FILL.a    = 255*k;
                e->cShape->OUTLINE.a = 255*k;

                e->cShape->circle.setFillColour( e->cShape->FILL );
                e->cShape->circle.setOutlineColour( e->cShape->OUTLINE );
            }
        }
    }
}

double easeInOutBack(double x)
{
	const double c1 = 1.70158;
	const double c2 = c1 * 1.525;

	return x < 0.5
		? (pow(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2
		: (pow(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}
double easeInOutElastic(double x) {
	const double c5 = (2 * M_PI) / 4.5;

	if (x == 0) return 0;
	if (x == 1) return 1;

	if (x < 0.5) {
		return -(std::pow(2, 20 * x - 10) * std::sin((20 * x - 11.125) * c5)) / 2;
	}
	else {
		return (std::pow(2, -20 * x + 10) * std::sin((20 * x - 11.125) * c5)) / 2 + 1;
	}
}
double easeOutBounce(double x) {
	const double n1 = 7.5625;
	const double d1 = 2.75;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5 / d1) * x + 0.75;
	}
	else if (x < 2.5 / d1) {
		return n1 * (x -= 2.25 / d1) * x + 0.9375;
	}
	else {
		return n1 * (x -= 2.625 / d1) * x + 0.984375;
	}
}
double easeInOutBounce(double x) {
	return x < 0.5
		? (1 - easeOutBounce(1 - 2 * x)) / 2
		: (1 + easeOutBounce(2 * x - 1)) / 2;
}
void GameEngine::sMovement()
{
    // Entity movement mechanism
    for (auto e : m_entityManager.getEntities()) {
        if ( !(e->tag() == "smallP") ) {
            e->cTransform->pos += e->cTransform->velMag * e->cTransform->velAxis;                
        }
    }

	for (auto& e : m_entityManager.getEntities("smallP")) {
		Vec2 pos;
		double y = easeInOutBack(e->cTransform->x);
		double theta = e->cTransform->theta + y*360;
		pos.x = 55 * cosDegree(theta);
		pos.y = 55 * sinDegree(theta);
		e->cTransform->pos = m_player->cTransform->pos + pos;

			    if (e->cTransform->x > 1.0) e->cTransform->dir = -1;
		else if (e->cTransform->x < 0.0) e->cTransform->dir = 1;

		          if (e->cTransform->dir ==  1) e->cTransform->x += 0.02;
			 else if (e->cTransform->dir == -1) e->cTransform->x -= 0.01;

		//std::cout << y << " " << e->cTransform->x << "\n";
	}

    // player movement mechanism
    if (gameScene == 0) // playScene
    {
        inputMovement(m_player);
		double z = 0;
		if (m_player->cInput->shoot && !m_lost)
		{
			double x = ((m_currentFrame - m_lastBulletSpawnTime) / (double)m_spawnIntervalBullet);
			z = x;
		}
		double y = (z*4.0) + m_playerConfig.SR;
		m_player->cShape->circle.setRadius(y);
    }
}
void GameEngine::inputMovement( std::shared_ptr<Entity> entity )
{
    // keyboard movement
    if (entity->cInput->up || entity->cInput->left || entity->cInput->down || entity->cInput->right)
    {
        if (entity->cInput->up)     { entity->cTransform->velAxis.y = -1; }
        if (entity->cInput->left)   { entity->cTransform->velAxis.x = -1; }
        if (entity->cInput->down)   { entity->cTransform->velAxis.y =  1; }
        if (entity->cInput->right)  { entity->cTransform->velAxis.x =  1; }
        
        // Fixes the amplitude in diagonal movement
        if ( (entity->cInput->up   && ( entity->cInput->left || entity->cInput->right )) || 
             (entity->cInput->down && ( entity->cInput->left || entity->cInput->right )) )
        {
            entity->cTransform->velAxis.diagonalFix();
        }

        // Sample movement speed and rotation update
        entity->cTransform->pos += entity->cTransform->velMag * entity->cTransform->velAxis;

        if (!entity->cInput->up)    { entity->cTransform->velAxis.y =  0; }
        if (!entity->cInput->left)  { entity->cTransform->velAxis.x =  0; }
        if (!entity->cInput->down)  { entity->cTransform->velAxis.y =  0; }
        if (!entity->cInput->right) { entity->cTransform->velAxis.x =  0; }
    }
}


void GameEngine::sLifespan()
{
    for (auto& e : m_entityManager.getEntities())
    {
        if (e->cLifeSpan && e->cShape)
        {
            if( e->cLifeSpan->remaining-- ) 
            {
                double k = (double) e->cLifeSpan->remaining / e->cLifeSpan->total;
                if (gameScene == 0) // playScene
                {
                    e->cShape->FILL.a    = 255*k;
                    e->cShape->OUTLINE.a = 255*k;
                }
                if (gameScene == 1) // menuScene
                {
                    e->cShape->FILL.a    = 255*0.67*k;
                    e->cShape->OUTLINE.a = 255*0.67*k;
                }

                e->cShape->circle.setFillColour( e->cShape->FILL );
                e->cShape->circle.setOutlineColour( e->cShape->OUTLINE );
            }
            else { e->destroy(); }
        }
    }
}


void GameEngine::sKeyPress(unsigned char Key, int MousePosX, int MousePosY)
{
    if (gameScene == 0) // playScene
    {
        if (!m_lost) // game being played
        {
            switch (Key)
            { 
                case 'a' : { m_player->cInput->left  = true; break; }
                case 'd' : { m_player->cInput->right = true; break; }
                case 'w' : { m_player->cInput->up    = true; break; }
                case 's' : { m_player->cInput->down  = true; break; }
                case ' ' : { m_player->cInput->shoot = !m_player->cInput->shoot; break; }
                case 'p' : { m_paused = !m_paused; break; }
				case 'm' : spawnEnemy(); break;
				case 't' : 
					for (auto b : m_entityManager.getEntities("smallP"))
					{
						if (b->isActive()) { b->destroy(); } break;
					}
					break;

                case 27  : { glutDestroyWindow(m_window.display); std::exit(0); break; }
            }
        }
        else // game lost
        {
            switch (Key)
            { 
                case 'a' : 
                {
                    retry = true; menu = false; break;
                }
                case 'd' : 
                {
                    menu = true; retry = false; break;
                }
                case ' ' :
                {
                    if (retry) { gameScene = 0; m_lost = false; retry = false; initPlayScene(); break; }
                    if (menu)  { gameScene = 1; m_lost = false; menu  = false; initMenuScene(); break; }
                }
            }
        }
        if (m_paused) // game being paused
        {
            switch (Key)
            { 
                case 'a' : 
                {
                    retry = true; menu = false; break;
                }
                case 'd' : 
                {
                    menu = true; retry = false; break;
                }
                case ' ' :
                {
                    if (retry) {                retry = false; m_paused = true; break; }
                    if (menu)  { gameScene = 1; menu  = false; initMenuScene(); break; }
                }
            }
        }
    }
	
        
    if (gameScene == 1) // menuScene
    { 
        switch (Key)
        {
            case 'w' : { if (menuIndex > 0) { --menuIndex; } break; }
            case 's' : {                      ++menuIndex;   break; }
            case ' ' : 
            {
                // playButton
                if (menuIndex % menuButton == 0) { gameScene = 0; initPlayScene(); }

                // creditsButton
                if (menuIndex % menuButton == 1) { gameScene = 2; }

                //quitButton
                if (menuIndex % menuButton == 2) { glutDestroyWindow(m_window.display); std::exit(0); }
                break;
            }
        }
        if (menuIndex % menuButton == 0) { newGame = true; } else { newGame = false; }
        if (menuIndex % menuButton == 1) { credits = true; } else { credits = false; }
        if (menuIndex % menuButton == 2) { exit = true; }    else { exit = false; }
    }

    if (gameScene == 2) // creditsScene
    {
        switch (Key)
        {
            case 27 : { gameScene = 1; break; }
        }
    }

}
void GameEngine::sKeyRelease(unsigned char Key, int MousePosX, int MousePosY)
{
    if (gameScene == 0) // playScene
    {
        switch (Key)
        {
            case 'a' : { m_player->cInput->left  = false; break; }
            case 'd' : { m_player->cInput->right = false; break; }
            case 'w' : { m_player->cInput->up    = false; break; }
            case 's' : { m_player->cInput->down  = false; break; }
        }
    }
}

void GameEngine::sMouseState(int button, int state, int x, int y)
{
    // menuScene
    if (gameScene == 1) 
    {
        // playButton action
        if ( (x >= 78 && x <= 555) && (y >= 290 && y <= 335) ) { gameScene = 0; newGame = false; initPlayScene(); }

        // creditsButton action
        if ( (x >= 78 && x <= 535) && (y >= 350 && y <= 395) ) { gameScene = 2; credits = false; }

        // quitButton action
        if ( (x >= 78 && x <= 515) && (y >= 410 && y <= 440) ) { glutDestroyWindow(m_window.display); std::exit(0); }
    }

    // playScene
    if (gameScene == 0) 
    {
        if (m_lost)
        {
            // retryButton action
            if ( (x > m_window.width/2 - 62 -100 && x < m_window.width/2 + 62 -100) && (y > m_window.height/2 -34 && y < m_window.height/2 -0) )
            { gameScene = 0; m_lost = false; retry = false; initPlayScene(); }
            
            // menuButton action
            if ( (x > m_window.width/2 - 62 +100 && x < m_window.width/2 + 62 +100) && (y > m_window.height/2 -34 && y < m_window.height/2 -0) )
            { gameScene = 1; m_lost = false; menu = false; initMenuScene(); }
        }
        else if (m_paused)
        {
            // resumeButton action
            if ( (x > m_window.width/2 - 62 -100 && x < m_window.width/2 + 62 -100) && (y > m_window.height/2 -34 && y < m_window.height/2 -0) )
            { retry = false; m_paused = false; }
            
            // menuButton action
            if ( (x > m_window.width/2 - 62 +100 && x < m_window.width/2 + 62 +100) && (y > m_window.height/2 -34 && y < m_window.height/2 -0) )
            { gameScene = 1; menu = false; initMenuScene(); }
        }
		else {
			if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
				std::cout << "GLUT_RIGHT_BUTTON " << m_SpecialWeaponCap << " " << m_powerMoveInterval << '\n'; spawnSpecialWeapon();
			}
		}
    }

}
/*
void GameEngine::sMouseDrag(int x, int y)
{
    
}
*/
void GameEngine::sMousePos(int x, int y) // for highlignting buttons
{ //std::cout << x << "  " << y << std::endl;
    if (gameScene == 1) // menuScene
    {
        // playButton highlight
        if ( (x >= 78 && x <= 555) && (y >= 290 && y <= 335) ) { newGame = true; } else newGame = false;

        // cheditsButton highlight
        if ( (x >= 78 && x <= 535) && (y >= 350 && y <= 395) ) { credits = true; } else credits = false;

        // quitButton highlight
        if ( (x >= 78 && x <= 515) && (y >= 410 && y <= 440) ) { exit = true; }    else exit = false;
    }

    if (gameScene == 0) // playScene
    {
        m_mouseLocation = Vec2(x,y);
        //else { m_mouseLocation = Vec2(0.0); } eta kaj korbe na, cz passive function hole kaj korto

        if (m_lost)
        {
            // retryButton highlight
            if ( (x > m_window.width/2 - 62 -100 && x < m_window.width/2 + 62 -100) && (y > m_window.height/2 -34 && y < m_window.height/2 -0) )
            { retry = true; }
            else retry = false;

            // menuButton highlight
            if ( (x > m_window.width/2 - 62 +100 && x < m_window.width/2 + 62 +100) && (y > m_window.height/2 -34 && y < m_window.height/2 -0) )
            { menu = true; }
            else menu = false;
        }
        if (m_paused)
        {
            // retryButton highlight
            if ( (x > m_window.width/2 - 62 -100 && x < m_window.width/2 + 62 -100) && (y > m_window.height/2 -34 && y < m_window.height/2 -0) )
            { retry = true; }
            else retry = false;

            // menuButton highlight
            if ( (x > m_window.width/2 - 62 +100 && x < m_window.width/2 + 62 +100) && (y > m_window.height/2 -34 && y < m_window.height/2 -0) )
            { menu = true; }
            else menu = false;
        }
    }
}

void GameEngine::Rotate(const Vec2 pos, double degree)
{
	glPushMatrix();

	glTranslatef(pos.x, pos.y, 0.0);

	glRotatef(degree, 0, 0, 1.0);

	glTranslatef(-pos.x, -pos.y, 0.0);
}
void GameEngine::UnRotate()
{
	glPopMatrix();
}


void GameEngine::initSpecialWeapon()
{
	if (m_SpecialWeaponCap > m_powerMoveInterval) m_SpecialWeaponCap = m_powerMoveInterval;
	else ++m_SpecialWeaponCap;

	int timeDelay = 1*60;
    bool stop = true;
	int timer = m_currentFrame - m_SpecialWeaponEffectTimer;
    if (timer <= timeDelay)
    {
        stop = false;
    }
    if (!stop)
    {
        k_died = (double)timer / timeDelay;
        for (auto& e : m_entityManager.getEntities("smallP"))
        {
            if (e->cShape)
            {
				//std::cout << k_died << std::endl;
				e->cShape->FILL.a    = 191 * k_died;
				e->cShape->OUTLINE.a = 191 * k_died;

                e->cShape->circle.setFillColour( e->cShape->FILL );
                e->cShape->circle.setOutlineColour( e->cShape->OUTLINE );
            }
        }
    }
}

void GameEngine::spawnSpecialWeapon()
{
	if (m_powerMoveInterval <= m_SpecialWeaponCap)
	{
		std::cout << "spawnSpecialWeapon" << '\n';
		for (auto b : m_entityManager.getEntities("smallP")) b->destroy();
		smallPlayer();
		m_SpecialWeaponCap = 0;
		m_SpecialWeaponEffectTimer = m_currentFrame;
	}
}
void GameEngine::smallPlayer()
{
	for (int i = 0; i < 3; ++i)
	{
		auto smallP = m_entityManager.addEntity("smallP");
		smallP->cTransform = std::make_shared<CTransform>(Vec2(0), Vec2(0), Vec2(0.0), 0.0);
		smallP->cShape = std::make_shared<CShape>(m_playerConfig.SR / 2, 3, Vec4(255), Vec4(m_playerConfig.OR, m_playerConfig.OG, m_playerConfig.OB, 255), m_playerConfig.OT / 1.5);
		smallP->cCollision = std::make_shared<CCollision>(m_playerConfig.CR / 2);
		smallP->cTransform->theta = i*120;
	}
}

void GameEngine::sDifficulty()
{
	if (m_currentFrame % m_windowConfig.frameLimit == 0) ++m_difficultyFrame;
    //m_difficultyFrame = glutGet(GLUT_ELAPSED_TIME)/1000.0 - m_playDiff;

    if ( m_difficultyFrame > 30 && m_difficultyOne )
    {
        std::cout << "Difficulty 1 activated!" << std::endl;
                                 m_enemyConfig.SMAX += 1; m_enemyConfig.SI -= 4; m_enemyConfig.L -= 3; m_bulletConfig.SI -= 10;
        m_difficultyOne = false; m_difficultyTwo = true;
    }
    else if ( m_difficultyFrame > 60 && m_difficultyTwo )
    {
        std::cout << "Difficulty 2 activated!" << std::endl;
                                 m_enemyConfig.SMAX += 1; m_enemyConfig.SI -= 4; m_enemyConfig.L -= 3; m_bulletConfig.SI -= 8;
        m_difficultyTwo = false; m_difficultyThree = true;
    }
    else if ( m_difficultyFrame > 90 && m_difficultyThree )
    {
        std::cout << "Difficulty 3 activated!" << std::endl;
        m_enemyConfig.SMIN += 1; m_enemyConfig.SMAX += 1; m_enemyConfig.SI -= 4; m_enemyConfig.L -= 3; m_bulletConfig.SI -= 6;
        m_difficultyThree = false; m_difficultyFour = true;
    }
    else if ( m_difficultyFrame > 120 && m_difficultyFour )
    {
        std::cout << "Difficulty 4 activated!" << std::endl;
        m_enemyConfig.SMIN += 1; m_enemyConfig.SMAX += 1; m_enemyConfig.SI -= 4; m_enemyConfig.L -= 3; m_bulletConfig.SI -= 4;
        m_difficultyFour = false; m_difficultyFive = true; m_difficultyFrame2 = m_difficultyFrame;
    }
    else if ( m_difficultyFrame > 150 && m_difficultyFive )
    {
        std::cout << "Difficulty 5 activated: Utimate Difficulty!" << std::endl;
        m_difficultyFive = false; m_ultimateDifficulty = true;
    }
    else if ( (m_difficultyFrame - m_difficultyFrame2 > 5) && m_ultimateDifficulty )
    {
        m_enemyConfig.SMIN  = 6; m_enemyConfig.SMAX = 10; m_enemyConfig.SI -= 1; m_enemyConfig.L -= 1; m_bulletConfig.SI  = 2;
        m_difficultyFrame2 = m_difficultyFrame;
    }
    //std::cout << m_enemyConfig.SMAX << " " << m_enemyConfig.SMIN << " " << m_enemyConfig.SI << " " << m_bulletConfig.SI << std::endl;
    
    m_spawnIntervalEnemy = m_enemyConfig.SI;
    m_spawnIntervalBullet = m_bulletConfig.SI;
}