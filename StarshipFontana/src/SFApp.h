#ifndef SFAPP_H
#define SFAPP_H

#include <memory>   // Pull in std::shared_ptr
#include <iostream> // Pull in std::cerr, std::endl
#include <list>     // Pull in list
#include <sstream>


using namespace std;

#include "SFCommon.h"
#include "SFEvent.h"
#include "SFAsset.h"
#include "SFBoundingBox.h"
/**
 * Represents the StarshipFontana application.  It has responsibilities for
 * * Creating and destroying the app window
 * * Processing game events
 */
class SFApp {
public:
  SFApp(std::shared_ptr<SFWindow>);
  virtual ~SFApp();
  void    OnEvent(SFEvent &);
  int     OnExecute();
  void    OnUpdateWorld();
  void    OnRender();
  void    FireProjectile();
private:
  shared_ptr<SFBoundingBox> bbox;
  bool                    is_running;

  shared_ptr<SFWindow>       sf_window;

  shared_ptr<SFAsset>        player;
  shared_ptr<SFAsset>        Background;
  shared_ptr<SFBoundingBox>  app_box;
  list<shared_ptr<SFAsset> > projectiles;
  list<shared_ptr<SFAsset> > alienprojectiles;
  list<shared_ptr<SFAsset> > aliens;
  list<shared_ptr<SFAsset> > coins;
  list<shared_ptr<SFAsset> > Walls;

  int hits;
  int fire;
  int alienfire;
  int timer;
  int cointer;
};
#endif