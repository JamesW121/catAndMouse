//
// Disclaimer:
// ----------
//
// This code will work only if you selected window, graphics and audio.
//
// Note that the "Run Script" build phase will copy the required frameworks
// or dylibs to your application bundle so you can execute it on any OS X
// computer.
//
// Your resource files (images, sounds, fonts, ...) are also copied to your
// application bundle. To get the path to these resources, use the helper
// function `resourcePath()` from ResourcePath.hpp
//
/*
Data to be sent:
angle, player position, projectile position, item position

*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <vector>

#include "alienattack.h"
#include "attack.h"
#include "chest.h"
#include "damageTrap.h"
#include "entity.h"
#include "helper.h"
#include "network.h"

    int spriteCounter = 0, spriteNum = 4, spriteLength = 215, spriteWidth = 215, movementSpeed = 10;


//setup player sprites
void setupPlayer(Human & player, sf::Texture& texture, int x, int y){
    player.getSprite().setTexture(texture);
    player.getSprite().setTextureRect(sf::IntRect(((spriteCounter) % 2)*spriteLength, (spriteCounter / 2)*spriteWidth, ((spriteCounter) % 2 + 1)*spriteLength, (spriteCounter / 2 + 1)*spriteWidth));
    player.getSprite().setScale(60.0 / (double)(spriteLength), 60.0 / (double)(spriteWidth)); 
    player.getSprite().setOrigin(sf::Vector2f(spriteLength/2, spriteWidth/2));
    player.getSprite().move(x,y);

}


int main()
{
    
    sf::Clock clock;
    sf::Clock clock2;
    sf::Clock clock3;
    sf::Clock clock4;
	sf::Clock packetSendClock;
    float playerMovementSpeed = 8;
    
    int counterWalking = 0;
    int counter = 0;
    int counter2 = 0;
    int counter3 = 0;
    int counter4 = 0;
    int counter5 = 0;

// Data to send; s = sent, b = bullet, p = player
float sbRot;
sf::Vector2f spPos, sbPos;
int spRot, sbDir;	

//Data to receive
//Player: rpPos, rpRot
//Projectile: rbPos, rbRot, rbDir
float rbRot;
sf::Vector2f rpPos, rbPos;
int rpRot, rbDir;   



    sf::RenderWindow window(sf::VideoMode(800, 800), "Maze", sf::Style::Titlebar | sf::Style::Close);
    sf::Texture texture,tx2,marineTexture;
    sf::Image image;
    sf::Sprite spr, spr2;
    sf::View view(sf::FloatRect(0, 0, 800, 800));
    window.setView(view);
    
    Network network;  
    network.setup();

    std::cout << "Loading texture...\n";
    
    if (!image.loadFromFile("img/firstmap.png")) {
        return EXIT_FAILURE;
    }

    if (!texture.loadFromFile("img/mapspr.png")){
    	return EXIT_FAILURE;
	}

    if (!tx2.loadFromFile("img/triangle.png")) {
        return EXIT_FAILURE;
    }
    
    if (!marineTexture.loadFromFile("img/alien.png")) {
        return EXIT_FAILURE;
    }
    spr.setTexture(texture);

    spr.move(0, 0);
    std::cout << "Creating Instances...\n";
    Human player = Human(sf::Vector2i(5,5),7,3);
    
    
    Human player2 = Human(sf::Vector2i(5,9),7,3);
    bigMap maze = bigMap(30);

	//Item
	Item item_test=Item("img/circle.png","Damage Trap");
	Item item_test2=Item("img/circle.png","Sword");
	//Chest ch=Chest("It looks safe");
	chest ch=chest("Spritesheets/Crate1.png","Testing",3);
	chest ch2=chest("Spritesheets/Crate1.png","It's a chest",3);
	ch.setItem(&item_test2);
	ch2.setItem(&item_test);
	damageTrap dt1=damageTrap("Spritesheets/landmine.png","Land Mine: 20 Damage",2.0,20);

	std::vector<interactable*> itemsList;
	itemsList.push_back(&ch2);
	itemsList.push_back(&dt1);
	itemsList.push_back(&ch);
	itemsList.push_back(&item_test2);
	itemsList.push_back(&item_test);

//	for(unsigned i=0;i<3;i++)
//		itemsList[i]->getSprite().setScale(0.6,0.6);
//	ch.getSprite().setScale(3,3);
	std::cout<<"Before setRect"<<std::endl;
	ch.setRect(0,0,32,32);
	ch2.setRect(0,0,32,32);
	dt1.setRect(0,0,32,32);
//	dt1.getSprite().setScale(2,2);
	ch.setPosition(1000,1000);
	ch2.setPosition(300,200);
	item_test.setIsLoaded(false);
	item_test2.setIsLoaded(false);
//	dt1.setPosition(0,0);
	dt1.setIsLoaded(false);
	//Item ends
    
   
    std::vector<projectile2>::const_iterator iter, iter2;
    std::vector<projectile2> myBullets, enemyBullets;
    std::vector<float>bulletAngles;
    // Projectile Object
    class projectile2 myBullet, enemyBullet;
    float bulletAngle;
    std::cout << "Initializing...\n";
    maze.load(image);

    if(network.isMarine()){

    setupPlayer(player2, marineTexture, 280, 440);
    setupPlayer(player, tx2, 1720, 2140);   
   
}
else
{
  setupPlayer(player, marineTexture, 280, 440);
    setupPlayer(player2, tx2, 1720, 2140);
    


}

	 player.updateCoor();
    player2.updateCoor(); 
    
    //	player.getSprite().setTexture(tx2);
    //	player.getSprite().setScale(0.6, 0.6);
    //	player.getSprite().setOrigin(sf::Vector2f(40, 40));
    //	player.getSprite().move(40, 40);
    //	updateSprite(player.getSprite(), marineTexture, window, 215, 215, 4);
    //for (int i = 0; i < 15; i++)
    //    for (int j = 0; j < 15; j++)
    //        std::cout << i << ' ' << j << ' ' << maze.getWall(i, j) << std::endl;
 
  
    sf::Clock clock_original;
    sf::Vector2f oldMovement = player.getPos();

    while (window.isOpen())
    {
        //		std::cout << "Drawing...\n";
       
	sf::Vector2f playerPos = player.getPos();
	sf::Event event;
	sf::Time time=clock.getElapsedTime();
	
		
        while (window.pollEvent(event))
        {
			//Trap
			if(dt1.getIsLoaded()&&dt1.getIsDeployed()){
				if(player.distanceToInteractable(&dt1)<10){
					dt1.activate(&player);
					//player.hp=player.hp-dt1.getDamagePoint;
//					itemsList.pop_back();
				}
			}
           
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code)
                {
					case sf::Keyboard::Num1:
						dt1.placeTrap(player);
						
						break;
					case sf::Keyboard::E:
						player.react(itemsList);
						break;
					case sf::Keyboard::LShift:
						if(player.getSpeed()==8)
							player.setSpeed(15);
						else
							player.setSpeed(8);
						break;
                    case sf::Keyboard::W:
                        if (checkAccess(player, 0, maze))
                            player.walk(0);
                        break;
                    case sf::Keyboard::S:
                        if (checkAccess(player, 1, maze))
                            player.walk(1);
                        break;
                    case sf::Keyboard::A:
                        if (checkAccess(player, 2, maze))
                            player.walk(2);
                        break;
                    case sf::Keyboard::D:
                        if (checkAccess(player, 3, maze))
                            player.walk(3);
                        break;
                    case sf::Keyboard::H:
						std::cout<< "HP: " <<player.hp<<std::endl;
						break;
                    default:

                     
                        break;
                }//end switch
                view.setCenter(getCenter(player.getPos(), image.getSize()));
                window.setView(view);
            }//end if (keypressed)
            spriteCounter = updateSprite(player.getSprite(), window, clock_original, spriteLength, spriteWidth, spriteNum, spriteCounter);
            //window.draw(player.getSprite());
            
            updateRotation(player, view, window);
            
        }
        
        maze.updateShade(player.getCoor(), player.getSight());
        
        window.clear();



	//getting player position and rotation

		float playerDirection = player.getSprite().getRotation();		
		spPos = player.getPos();
		spRot = playerDirection;





    sf::Time elapsed1 = clock.getElapsedTime();
    sf::Time elapsed2 = clock2.getElapsedTime();
    sf::Time elapsed3 = clock3.getElapsedTime();
    sf::Time elapsed4 = clock4.getElapsedTime();



	sf::Vector2f player2Pos;
	int player2Dir;
	
if(packetSendClock.getElapsedTime().asMilliseconds()>200){


	network.sendAllData(spPos, spRot, sbPos, sbDir, sbRot);
	packetSendClock.restart();
}

	//reset attack positions after send
	sbPos = sf::Vector2f(0,0);
        sbDir = 0;
        sbRot = 0.0;



      network.receiveAllData(rpPos, rpRot, rbPos, rbDir, rbRot);

	//set new received positions
      player2Pos = rpPos;
      player2Dir = rpRot;



	player2.getSprite().setRotation(player2Dir);
	if(player2Pos.x != 0){ 
	if(player2Pos.x != oldMovement.x || player2Pos.y != oldMovement.y){

      
        if(maze.getDetect(int(player2Pos.x)/80,int(player2Pos.y)/80)==2)
 	 {    player2.setPos(player2Pos);
	}
 	    else player2.setPos(sf::Vector2f(2400,2400));
   }

	  oldMovement = player2Pos;

}	

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
              //  if(network.isMarine()){
                //only do projectile attack if is Marine
                
		
		myBullet.rect.setPosition(player.getPos().x,player.getPos().y);
                myBullet.direction = player.direction;
                myBullets.push_back(myBullet);
		bulletAngle = myBullet.getAngle(player,view,window);
	
		// store bullet values to be sent
		sbPos = myBullet.rect.getPosition();
		sbDir = myBullet.direction;
		sbRot = bulletAngle;

           //}
        }






     //Receive attacking info
       // if(!network.isMarine())
        //{

                 sf::Vector2f attackPos = sf::Vector2f(0,0);
                 int direction;
                 float angle;
        

		//values for the bullets
		attackPos = rbPos;
		direction = rbDir;
		angle = rbRot;

                if(attackPos.x != 0){
                	enemyBullet.rect.setPosition(attackPos.x, attackPos.y);
               	 	enemyBullet.direction = direction;	
		 	enemyBullets.push_back(enemyBullet);
                	bulletAngles.push_back(angle);
		

		}
       


        //}
/*
        // Bullet hit Enemy
        counter = 0;
        for (iter = projectileArray2.begin(); iter != projectileArray.end(); iter++)
        {
            counter2 = 0;
           
            if (projectileArray2[counter].rect.getGlobalBounds().intersects(player2.rect.getGlobalBounds()))
            {
                projectileArray2[counter].destroy = true;
                
                player2.hp -= projectileArray2[counter].attackDamage;
                if (player2.hp <= 0)
                {
                    player2.alive = false;
                }
            }
            counter2++;
            counter++;
        }
       
*/

      /*  // Delete Dead Enemy
        counter = 0;
        
        if (player2.alive == false)
        {
            cout << "Player win" << endl;
            return 0;
        }
        
        counter++;
        */
        // Delete Projectile

/*
        counter = 0;
        for (iter2 = projectileArray2.begin(); iter2 != projectileArray2.end(); iter2++)
        {
            if (projectileArray2[counter].destroy == true)
            {
//                projectileArray2.erase(projectileArray2[counter]);
                break;
            }
            
            counter++;
        }
*/
		
		//Draw All In Game Objects
        window.draw(spr);
		for(unsigned int i=0;i<itemsList.size();i++){
			itemsList[i]->draw(window);
		}
        window.draw(player.getSprite());
        window.draw(player2.getSprite());
        
        for(int i=0;i<maze.getSize();i++){
        	for(int j=0;j<maze.getSize();j++)
	        	window.draw(maze.getShade(i,j));
		}
      

          

 
        //Draw my projectiles
        int counter = 0;
        for (iter = myBullets.begin(); iter != myBullets.end(); iter++)
        {

           myBullets[counter].update(player,view,window); // Update Projectile     

    //       window.draw(myBullets[counter].rect);

	//check if bullet hit enemy

	if (myBullets[counter].rect.getGlobalBounds().intersects(player2.rect.getGlobalBounds()))
            {
                myBullets[counter].destroy = true;
              std::cout <<"Player hit" << std::endl; 
         //       player2.hp -= projectileArray2[counter].attackDamage;
           /*    
	    if (player2.hp <= 0)
                {
                    player2.alive = false;
                }  
	  */
	    }
               window.draw(myBullets[counter].rect);


		counter++;

	}
 

	      


	//Draw enemy projectiles


	counter = 0;	
	for (iter2 = enemyBullets.begin(); iter2 != enemyBullets.end(); iter2++)	
	{

		float angle = bulletAngles[counter];

        enemyBullets[counter].rect.move(cos((3.14159/180)*angle)* movementSpeed, sin((3.14159/180)*angle)*movementSpeed);
	window.draw(enemyBullets[counter].rect);
	counter++;

	}



        player.update();
        player2.update();
        
        window.display();
    }
    
    return 0;
}
