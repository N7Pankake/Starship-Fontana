#include "SFApp.h"

SFApp::SFApp(std::shared_ptr<SFWindow> window) : fire(0),alienfire(0),hits(0),cointer(0), timer(0), is_running(true), sf_window(window) {
  int canvas_w, canvas_h;

  SDL_GetRendererOutputSize(sf_window->getRenderer(), &canvas_w, &canvas_h);

  app_box = make_shared<SFBoundingBox>(Vector2(canvas_w, canvas_h), canvas_w, canvas_h);

  player  = make_shared<SFAsset>(SFASSET_PLAYER, sf_window);
  auto player_pos = Point2(canvas_w/2, 22);
  player->SetPosition(player_pos);


  Background = make_shared<SFAsset>(SFASSET_BACKGROUND, sf_window);
    auto Background_pos = Point2(canvas_w/2, canvas_h/2);
    Background->SetPosition(Background_pos);



    const int number_of_walls = 5;
    for(int i=0; i<number_of_walls; i++) {
        // place an alien at width/number_of_aliens * i
        auto Wall = make_shared<SFAsset>(SFASSET_WALL, sf_window);
        auto pos   = Point2(((canvas_w/number_of_walls) * i) + 60, 88);
        Wall->SetPosition(pos);
        Walls.push_back(Wall);
    }


  const int number_of_aliens = 10;
  for(int i=0; i<number_of_aliens; i++) {
    // place an alien at width/number_of_aliens * i
    auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
    auto pos   = Point2(((canvas_w/(number_of_aliens*2)+20) * i)+ 60, ((canvas_h/2)+190));
    alien->SetPosition(pos);
    aliens.push_back(alien);
  }
  const int number_of_aliens2 = 10;
   for(int i=0; i<number_of_aliens2; i++) {
     // place an alien at width/number_of_aliens * i
     auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
     auto pos   = Point2(((canvas_w/(number_of_aliens2*2)+20) * i)+ 60, ((canvas_h/2)+150));
     alien->SetPosition(pos);
     aliens.push_back(alien);
   }
   const int number_of_aliens3 = 10;
     for(int i=0; i<number_of_aliens3; i++) {
       // place an alien at width/number_of_aliens * i
       auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
       auto pos   = Point2(((canvas_w/(number_of_aliens3*2)+20) * i)+ 60, ((canvas_h/2)+110));
       alien->SetPosition(pos);
       aliens.push_back(alien);
     }
     const int number_of_aliens4 = 10;
       for(int i=0; i<number_of_aliens4; i++) {
         // place an alien at width/number_of_aliens * i
         auto alien = make_shared<SFAsset>(SFASSET_ALIEN, sf_window);
         auto pos   = Point2(((canvas_w/(number_of_aliens4*2)+20) * i)+ 60, ((canvas_h/2)+70));
         alien->SetPosition(pos);
         aliens.push_back(alien);
       }



  auto coin = make_shared<SFAsset>(SFASSET_COIN, sf_window);
  auto pos  = Point2((50), canvas_h/2);
  coin->SetPosition(pos);
  coins.push_back(coin);
}

SFApp::~SFApp() {
}


/**
 * Handle all events that come from SDL.
 * These are timer or keyboard events.
 */
void SFApp::OnEvent(SFEvent& event) {
  SFEVENT the_event = event.GetCode();
  switch (the_event) {
  case SFEVENT_QUIT:
    is_running = false;
    break;
  case SFEVENT_UPDATE:
    OnUpdateWorld();
    OnRender();
    break;
  case SFEVENT_PLAYER_LEFT:
    player->GoWest();
    break;
  case SFEVENT_PLAYER_RIGHT:
    player->GoEast();
    break;
  case SFEVENT_PLAYER_UP:
      player->GoNorth();
      break;
  case SFEVENT_PLAYER_DOWN:
      player->GoSouth();
      break;
  case SFEVENT_FIRE:
    fire ++;
    FireProjectile();
    break;
  }
}

int SFApp::OnExecute() {
  // Execute the app
  SDL_Event event;
  while (SDL_WaitEvent(&event) && is_running) {
    // wrap an SDL_Event with our SFEvent
    SFEvent sfevent((const SDL_Event) event);
    // handle our SFEvent
    OnEvent(sfevent);
  }
}

void SFApp::OnUpdateWorld() {


  // Update projectile positions
  for(auto p: projectiles) {
    p->GoNorthBullet();
  }

  for(auto ap: alienprojectiles) {
      ap->GoSouthBullet();
    }

  for (auto a: aliens)
  {
	  { alienfire = rand()%1000+1;
        		if(alienfire < 1.50)
        		{
        		auto apb = make_shared<SFAsset>(SFASSET_ALIENPROJECTILE, sf_window);
        		auto av = a->GetPosition();
        		apb->SetPosition(av);
        		alienprojectiles.push_back(apb);
        		}
	  }
  }

  for(auto c: coins) {
	  if (cointer <= 720)
	     {
		  cointer += 1;
	      c->GoEast();
	     }
	    	if(timer > 720)
	    	  {
	    	  	cointer += 1;
	    	  	c->GoWest();
	    	  }
	    	  	if(cointer >= 1440)
	    	  	 {
	    	  		cointer = 0 ;

	    	  	 }
  }

  // Update enemy positions (Space Invaders way)
//*WOrks perfect for 5 aliens
  for(auto a : aliens)

  {
	  if (timer <= 1440)
	  		   	  {
	  		     		timer += 1;
	  		   		 a->AlienMovementRight();
	  		   	  }
	  		   	  if(timer > 1440)
	  		   	   	  {
	  		   		timer += 1;
	  		   	   		 a->AlienMovementLeft();
	  		   	   	  }
	  		   	  if(timer >= 2880)
	  		   	   		 {
	  		   		a->AlienMovementSouth();
	  		   		  timer = 0 ;

	  		   	   		 }
  }

  /* Update enemy positions (Yar's Revenge Way (Inverted)) sometime in the future

    for(auto a : aliens){
  	  a->AlienMovementSouth();
  }*/

  // Detect collisions

  // Dectect Alien
  for(auto p : projectiles) {
    for(auto a : aliens) {
      if(p->CollidesWith(a)) {
        p->HandleCollision();
        a->HandleCollision();
      }
  }}

  //Player bullets vs Alien bullets
  for(auto p : projectiles) {
      for(auto ap : alienprojectiles) {
        if(p->CollidesWith(ap)) {
          p->HandleCollision();
          ap->HandleCollision();
        }
    }}

  //Player bullets vs Alien bullets
    for(auto p : projectiles) {
    	for(auto W : Walls) {
    	        if(p->CollidesWith(W)) {
    	          p->HandleCollision();
    	        }
    	    }}

    //Player  vs Alien bullets
        for(auto ap : alienprojectiles) {
        	        if(ap->CollidesWith(player)) {
        	          player->HandleCollision();
        	          ap->HandleCollision();
        	        }
        	    }


  //Alien Destroy Walls
  for(auto ap : alienprojectiles) {
      for(auto W : Walls) {
        if(ap->CollidesWith(W)) {
          ap->HandleCollision();
          hits += 1;
          if (hits == 10)
          {
        	  W->HandleCollision();
        	  hits = 0;
          }
        }
    }}



//Destroy Coin
  for(auto p : projectiles) {
      for(auto c : coins) {
        if(p->CollidesWith(c)) {
          p->HandleCollision();
          c->HandleCollision();
        }
    }}


// Alien - Player Collision
  for (auto a: aliens) {
        if(player->CollidesWith(a)) {
        player->HandleCollision();
        a->HandleCollision();
      }
  }

  //Remove Sprites (not making them invisible


  // remove dead aliens (the long way)
  list<shared_ptr<SFAsset>> tmp1;
  for(auto a : aliens) {
    if(a->IsAlive()) {
      tmp1.push_back(a);
    }
  }
  aliens.clear();
  aliens = list<shared_ptr<SFAsset>>(tmp1);


  // remove alienprojectiles (the long way)
    list<shared_ptr<SFAsset>> tmp5;
    for(auto ap : alienprojectiles) {
      if(ap->IsAlive()) {
        tmp5.push_back(ap);
      }
    }
    alienprojectiles.clear();
    alienprojectiles = list<shared_ptr<SFAsset>>(tmp5);

    // remove projectiles (the long way)
       list<shared_ptr<SFAsset>> tmp6;
       for(auto p : projectiles) {
         if(p->IsAlive()) {
           tmp6.push_back(p);
         }
       }
       projectiles.clear();
       projectiles = list<shared_ptr<SFAsset>>(tmp6);




  // remove dead coins (the long way)
    list<shared_ptr<SFAsset>> tmp2;
    for(auto c : coins) {
      if(c->IsAlive()) {
        tmp2.push_back(c);
      }
    }
    coins.clear();
    coins = list<shared_ptr<SFAsset>>(tmp2);

    // remove dead Walls (the long way)
      list<shared_ptr<SFAsset>> tmp3;
      for(auto W : Walls) {
        if(W->IsAlive()) {
          tmp3.push_back(W);
        }
      }
      Walls.clear();
      Walls = list<shared_ptr<SFAsset>>(tmp3);

      /* remove dead player (the long way) No idea how to do this atm.
      shared_ptr<SFAsset>  tmp8;
              if(player->IsAlive())
              {
              }
            player.clear();
            player= shared_ptr<SFAsset> (tmp8);*/


}

void SFApp::OnRender() {
  SDL_RenderClear(sf_window->getRenderer());

//draw background
  Background->OnRender();

  // draw the Wall
  for(auto W: Walls) {
      if(W->IsAlive()) {W->OnRender();}
    }

  for(auto p: projectiles) {
    if(p->IsAlive()) {p->OnRender();}
  }

  for(auto ap: alienprojectiles) {
      if(ap->IsAlive()) {ap->OnRender();}
    }

  for(auto a: aliens) {
    if(a->IsAlive()) {a->OnRender();}
  }

  for(auto c: coins) {
    c->OnRender();
  }
  // draw the player
   player->OnRender();
  // Switch the off-screen buffer to be on-screen
  SDL_RenderPresent(sf_window->getRenderer());
}

void SFApp::FireProjectile() {
  auto pb = make_shared<SFAsset>(SFASSET_PROJECTILE, sf_window);
  auto v  = player->GetPosition();
  pb->SetPosition(v);
  projectiles.push_back(pb);
}

