#include "StdAfx.h"
#include "RogueGame.h"
#include "GraphicsManager.h"
#include "Camera.h"
#include "MapManager.h"
#include "FEBattleField.h"
#include "FEEnemyUnit.h"
#include "NumWalker.h"
#include "RandomWalker.h"
#include "SampleFEAI.h"
#include "GeneticAI.h"
#include "AIBreeder.h"

RogueGame::RogueGame(void)
{
}

RogueGame::~RogueGame(void)
{
}

void RogueGame::initialize(void)
{
	MapManager* mapmgr = MapManager::getInstance();
	GraphicsManager* graph = GraphicsManager::getInstance();
	FEStatViewer* sampleStatViewer = new FEStatViewer();
	FEBattleField* sampleBattleField = new FEBattleField(2, 7, 7, sampleStatViewer);
	int map[49] = {	0, 0, 0, 0, 0, 0, 0,
					0, 1, 1, 1, 0, 1, 0,		
					0, 0, 0, 0, 0, 0, 0,		
					0, 1, 0, 1, 0, 1, 0,		
					0, 0, 0, 0, 0, 0, 0,		
					0, 1, 0, 1, 0, 1, 0,		
					0, 0, 0, 0, 0, 0, 0};	
	sampleBattleField->InitTerrain(map, 7, 7);
	//sampleBattleField->setAI(new SampleFEAI(), 2);

	Item* armingSword = new Item(SWORD, 0, 4, 90, 5, 1, 1);
	Item* recurveBow = new Item(BOW, 0, 7, 80, 10, 2, 2);
	Item* impailer = new Item(LANCE, 0, 8, 90, 10, 1, 1);
	Item* cutlass = new Item(SWORD, 0, 6, 100, 30, 1, 1);
	Item* battleAxe = new Item(AXE, 0, 8, 80, 15, 1, 1);
	Item* fireTome = new Item(ANIMA, 0, 5, 90, 5, 1, 2);

	StatBlock* knight_stats = new StatBlock(20, 6, 0, 8, 5, 7, 5, 5, 4, Proficiency());
	StatBlock* archer_stats = new StatBlock(15, 5, 0, 5, 6, 8, 9, 5, 5, Proficiency());
	StatBlock* cavalier_stats = new StatBlock(18, 6, 0, 5, 4, 6, 6, 5, 8, Proficiency());
	StatBlock* swashbuckler_stats = new StatBlock(16, 6, 0, 4, 4, 15, 12, 10, 6, Proficiency());
	StatBlock* fighter_stats = new StatBlock(18, 8, 0, 5, 4, 5, 6, 5, 5, Proficiency());
	StatBlock* mage_stats = new StatBlock(14, 0, 7, 3, 6, 8, 8, 5, 5, Proficiency());

	FEUnit* lancelot = new FEUnit('K', 4, 1, knight_stats, armingSword, "Lancelot");
	FEUnit* robin = new FEUnit('A', 4, 1, archer_stats, recurveBow, "Robin");
	FEUnit* arthur = new FEUnit('C', 4, 1, cavalier_stats, impailer, "Arthur");
	FEUnit* galahad = new FEUnit('S', 4, 1, swashbuckler_stats, cutlass, "Galahad");
	FEUnit* bedevere = new FEUnit('F', 4, 1, fighter_stats, battleAxe, "Bedevere");
	FEUnit* merlin = new FEUnit('W', 4, 1, mage_stats, fireTome, "Merlin");

	StatBlock** statSheet = new StatBlock*[6];
	statSheet[0] = knight_stats;
	statSheet[1] = archer_stats;
	statSheet[2] = cavalier_stats;
	statSheet[3] = swashbuckler_stats;
	statSheet[4] = fighter_stats;
	statSheet[5] = mage_stats;

	//sampleBattleField->setAI(new GeneticAI(statSheet, 6), 2);
	LinkedList<spawnPoint>* spawns = new LinkedList<spawnPoint>();
	spawns->insert(new spawnPoint(3, 7, 'K', 4, 1, knight_stats, armingSword, "Lancelot"));
	spawns->insert(new spawnPoint(4, 7, 'A', 4, 1, archer_stats, recurveBow, "Robin"));
	spawns->insert(new spawnPoint(5, 7, 'C', 4, 1, cavalier_stats, impailer, "Arthur"));
	spawns->insert(new spawnPoint(6, 7, 'S', 4, 1, swashbuckler_stats, cutlass, "Galahad"));
	spawns->insert(new spawnPoint(4, 6, 'F', 4, 1, fighter_stats, battleAxe, "Bedevere"));
	spawns->insert(new spawnPoint(5, 6, 'W', 4, 1, mage_stats, fireTome, "Merlin"));

	spawns->insert(new spawnPoint(3, 15, 'K', 1, 2, knight_stats, armingSword, "Black Knight"));
	spawns->insert(new spawnPoint(4, 15, 'A', 1, 2, archer_stats, recurveBow, "Denise"));
	spawns->insert(new spawnPoint(5, 15, 'C', 1, 2, cavalier_stats, impailer, "Frenchman"));
	spawns->insert(new spawnPoint(6, 15, 'S', 1, 2, swashbuckler_stats, cutlass, "Herbert"));
	spawns->insert(new spawnPoint(4, 14, 'F', 1, 2, fighter_stats, battleAxe, "Giant"));
	spawns->insert(new spawnPoint(5, 14, 'W', 1, 2, mage_stats, fireTome, "Tim"));

	//AIBreeder* sampleBreeder = new AIBreeder(20, statSheet, 6, sampleBattleField, spawns, "lastGeneration");
	//sampleBreeder->breedGenerations(3); //do this in 5 stages

	Cell** leftPatrol = new Cell*[5];
	leftPatrol[0] = sampleBattleField->getCell(3, 15);
	leftPatrol[1] = sampleBattleField->getCell(1, 15);
	leftPatrol[2] = sampleBattleField->getCell(1, 12);
	leftPatrol[3] = sampleBattleField->getCell(1, 15);
	leftPatrol[4] = leftPatrol[0];
	Cell** rightPatrol = new Cell*[5];
	rightPatrol[0] = sampleBattleField->getCell(6, 15);
	rightPatrol[1] = sampleBattleField->getCell(8, 15);
	rightPatrol[2] = sampleBattleField->getCell(8, 12);
	rightPatrol[3] = sampleBattleField->getCell(8, 15);
	rightPatrol[4] = rightPatrol[0];

	FEUnit* black = new FEUnit('K', 1, 2, knight_stats, armingSword, "Black Knight");//Left patrol
	black->setPatrolPath(leftPatrol, 5);
	FEUnit* denise = new FEUnit('S', 1, 2, swashbuckler_stats, cutlass, "Denise");//Right patrol
	denise->setPatrolPath(rightPatrol, 5);


	FEUnit* herbert = new FEUnit('A', 1, 2, archer_stats, recurveBow, "Herbert");
	FEUnit* frenchman = new FEUnit('C', 1, 2, cavalier_stats, impailer, "Frenchman");
	FEUnit* giant = new FEUnit('F', 1, 2, fighter_stats, battleAxe, "Giant");
	FEUnit* tim = new FEUnit('W', 1, 2, mage_stats, fireTome, "Tim");

	sampleBattleField->setAI(new GeneticAI(statSheet, 6, "bestAI.csv"), 2);
	sampleBattleField->setAI(nullptr, 1);

	sampleBattleField->enter(lancelot, 3, 7);
	sampleBattleField->enter(arthur, 4, 7);
	sampleBattleField->enter(galahad, 5, 7);
	sampleBattleField->enter(bedevere, 6, 7);
	sampleBattleField->enter(robin, 4, 6);
	sampleBattleField->enter(merlin, 5, 6);

	sampleBattleField->enter(black, 3, 15);
	sampleBattleField->enter(herbert, 4, 15);
	sampleBattleField->enter(frenchman, 5, 15);
	sampleBattleField->enter(denise, 6, 15);
	sampleBattleField->enter(giant, 4, 14);
	sampleBattleField->enter(tim, 5, 14);

	//sampleBattleField->enter(new FEUnit('K', 4, 0, 6, 1, 6, 4, 100, 0, 8, "Lancelot"), 2, 2);
	//sampleBattleField->enter(new FEUnit('A', 1, 1, 4, 2, 4, 0, 100, 0, 6, "Galehad "), 2, 4);
	//sampleBattleField->enter(new FEUnit('K', 4, 0, 6, 1, 6, 4, 100, 0, 8, "Robin   "), 4, 2);
	//sampleBattleField->enter(new FEUnit('A', 1, 1, 4, 2, 4, 0, 100, 0, 6, "William "), 4, 4);
	//sampleBattleField->getCell(2, 2)->tryToMoveToCell(sampleBattleField->getCell(4, 3), FALSE);
	Camera* cam = new Camera(sampleBattleField, 0, 0, -1, -1, 9, 9);
	graph->insert(cam);
	Camera* cam2 = new Camera(sampleStatViewer, 9, 0, 0, 0, 12, 8);
	graph->insert(cam2);
	Camera* cam3 = new Camera(sampleConsole, 12, 8, 0, 0, 30, 14);
	graph->insert(cam3);
	mapmgr->activateMap(sampleBattleField);
	mapmgr->registerForInput(sampleBattleField);

}
