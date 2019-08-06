#include "gtest/gtest.h"

#include "game.h"
#include "TerranBaseFixture.h"

#define COMMANDWAIT 5

using namespace Funtest;

TEST_F(TerranBaseFixture, TerranTrain)
{
  // Test Command Center
  commandCenter.train(UnitTypes::Terran_SCV);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(commandCenter.isTraining(), true);
  EXPECT_EQ(commandCenter.isIdle(), false);
  EXPECT_EQ(commandCenter.getTrainingQueue().size(), 1);
  commandCenter.train(UnitTypes::Terran_SCV);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(commandCenter.getTrainingQueue().size(), 2);
  commandCenter.cancelTrain();
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(commandCenter.getTrainingQueue().size(), 1);
  commandCenter.cancelTrain();
  funGame.advance(COMMANDWAIT);
}

TEST_F(TerranBaseFixture, TerranLift)
{
  commandCenter.lift();
  funGame.advance(100);
  EXPECT_EQ(commandCenter.isLifted(), true);
  commandCenter.land(TilePosition{ 0, 0 });
  funGame.advance(100);
  EXPECT_EQ(commandCenter.isLifted(), false);
}

TEST_F(TerranBaseFixture, TerranAddon)
{
  auto type = UnitTypes::Terran_Comsat_Station;
  commandCenter.buildAddon(type);
  funGame.advance(type.buildTime() + COMMANDWAIT);
  EXPECT_EQ(self.completedUnitCount(type), 1);
}

TEST_F(TerranBaseFixture, TerranResearch)
{
  // Test Researching at Academy
  academy.research(TechTypes::Stim_Packs);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(academy.isResearching(), true);
}

TEST_F(TerranBaseFixture, TerranUpgrade)
{
  // Test Upgrading at Engineering Bay
  engineeringBay.upgrade(UpgradeTypes::Terran_Infantry_Weapons);
  funGame.advance(COMMANDWAIT);
  EXPECT_EQ(engineeringBay.isUpgrading(), true);
}

TEST_F(TerranBaseFixture, TerranAddonMove)
{
  // Test building an addon when we have to move
  factory.build(UnitTypes::Terran_Machine_Shop, TilePosition{ 4, 4 } + factory.getTilePosition());
  funGame.advance(200);
  EXPECT_EQ(self.allUnitCount(UnitTypes::Terran_Machine_Shop), 1);
}

TEST_F(TerranBaseFixture, TerranBunker)
{
  //Test information around a bunker, and test loading/unloading.
  EXPECT_EQ(bunker.getLoadedUnits().size(), 0);
  Position bunkerPosition = bunker.getPosition();
  funGame->createUnit(self, UnitTypes::Terran_Marine, bunkerPosition + Position{ 0, bunker.getType().height() }, 2);
  funGame.advance(2);
  auto marines = funGame->getUnitsInRadius(bunkerPosition, 500, Filter::IsOwned && Filter::GetType == UnitTypes::Terran_Marine);
  EXPECT_EQ(marines.size(), 2);
  for (auto& u : marines)
  {
    if (!u.isLoaded())
    {
      bunker.load(u);
      break;
    }
  }
  funGame.advance(20);
  EXPECT_EQ(bunker.getLoadedUnits().size(), 1);
  for (auto& u : marines)
  {
    if (!u.isLoaded())
    {
      bunker.load(u);
      break;
    }
  }
  funGame.advance(30);
  EXPECT_EQ(bunker.getLoadedUnits().size(), 2);
  bunker.unloadAll();
  funGame.advance(30);
  EXPECT_EQ(bunker.getLoadedUnits().size(), 0);
}