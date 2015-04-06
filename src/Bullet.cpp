#include "Bullet.h"

void Bullet::moveBullet()
{
    std::vector<Vec3f> predictions = positions;
    Vec3f predPos = pos;
    float predAngle = angle;
    for(int z = 0; z != lifetime; z++)
    {
        for(int i = 0; i != speed; i++)
        {
            sf::Vector2f newPos = math::angleCalc(sf::Vector2f(predPos.x,predPos.y),predAngle,1);
            predPos = Vec3f(newPos.x,newPos.y,predPos.z);


            if(aabb(predPos.x,predPos.y,GRID_SIZE,GRID_SIZE*95,GRID_SIZE,GRID_SIZE*95))
                if(!tiles[abs_to_index(predPos.x/GRID_SIZE)][abs_to_index(predPos.y/GRID_SIZE)][abs_to_index(predPos.z/GRID_SIZE)].walkable)
            {
                Vec3f tempPos(predictions[predictions.size()-1]);
                Vec3f secondVelo(tempPos.x - predPos.x, tempPos.y - predPos.y, tempPos.z - predPos.z);
                Vec3f secondPos(tempPos.x + secondVelo.x, tempPos.y + secondVelo.y);

                Vec3f tempVelocity(predPos.x - secondPos.x, predPos.y - secondPos.y, predPos.z - secondPos.z);

                std::string Face = tileFace(predPos.x,predPos.y,predPos.z,GRID_SIZE,tiles);
                if(Face == "UP" || Face == "DOWN")
                    tempVelocity.y = -tempVelocity.y;
                else if(Face == "LEFT" || Face == "RIGHT")
                    tempVelocity.x = -tempVelocity.x;

                tempPos.x += tempVelocity.x;
                tempPos.y += tempVelocity.y;
                tempPos.z += tempVelocity.z;

                predAngle = math::constrainAngle(math::angleBetweenVectors(newPos,sf::Vector2f(tempPos.x,tempPos.y)));
                predictions.push_back(predPos);
            }
        }
        if(z == lifetime-1)
            predictions.push_back(predPos);
    }

    if(true == false)
    {
        //sf::Vector2f newPos = math::angleCalc(sf::Vector2f(pos.x,pos.y),angle,1);
        sf::Vector2f newPos = sf::Vector2f(pos.x + velocity.x, pos.y + velocity.y);//math::angleCalc(sf::Vector2f(pos.x,pos.y),angle,1);
        pos = Vec3f(newPos.x,newPos.y,pos.z);
        effects.createCircle(pos.x,pos.y,3,sf::Color(150,150,150),1,sf::Color(0,0,0));

        if(aabb(pos.x,pos.y,GRID_SIZE,GRID_SIZE*95,GRID_SIZE,GRID_SIZE*95) &&
           !tiles[abs_to_index(pos.x/GRID_SIZE)][abs_to_index(pos.y/GRID_SIZE)][abs_to_index(pos.z/GRID_SIZE)].walkable)
        {

            Vec3f tempPos(positions[positions.size()-1]);
            Vec3f tempVelocity(pos.x - tempPos.x, pos.y - tempPos.y, pos.z - tempPos.z);
            //Vec3f tempVelocity(tempPos.x - pos.x, tempPos.y - pos.y, tempPos.z - pos.z);

            std::string Face = tileFace(pos.x,pos.y,pos.z,GRID_SIZE,tiles);
            if(Face == "UP" || Face == "DOWN")
                velocity.y = -velocity.y;
                //faceAngle = -90;
            if(Face == "LEFT" || Face == "RIGHT")
                velocity.x = -velocity.x;

            positions.push_back(pos);
        }
    }

    for(int i = 0; i != speed; i++)
    {
        sf::Vector2f newPos = math::angleCalc(sf::Vector2f(pos.x,pos.y),angle,1);
        pos = Vec3f(newPos.x,newPos.y,pos.z);
        effects.createCircle(pos.x,pos.y,3,sf::Color(150,150,150),1,sf::Color(0,0,0));

        if(aabb(pos.x,pos.y,GRID_SIZE,GRID_SIZE*95,GRID_SIZE,GRID_SIZE*95) &&
           !tiles[abs_to_index(pos.x/GRID_SIZE)][abs_to_index(pos.y/GRID_SIZE)][abs_to_index(pos.z/GRID_SIZE)].walkable)
        {
            Vec3f tempPos(positions[positions.size()-1]);
            Vec3f secondVelo(tempPos.x - pos.x, tempPos.y - pos.y, tempPos.z - pos.z);
            Vec3f secondPos(tempPos.x + secondVelo.x, tempPos.y + secondVelo.y);

            Vec3f tempVelocity(pos.x - secondPos.x, pos.y - secondPos.y, pos.z - secondPos.z);

            std::string Face = tileFace(pos.x,pos.y,pos.z,GRID_SIZE,tiles);
            if(Face == "UP" || Face == "DOWN")
                tempVelocity.y = -tempVelocity.y;
            else if(Face == "LEFT" || Face == "RIGHT")
                tempVelocity.x = -tempVelocity.x;

            std::cout << "Face: " << Face << std::endl;

            tempPos.x += tempVelocity.x;
            tempPos.y += tempVelocity.y;
            tempPos.z += tempVelocity.z;

            angle = math::constrainAngle(math::angleBetweenVectors(newPos,sf::Vector2f(tempPos.x,tempPos.y)));
            positions.push_back(pos);
        }
    }

    for(int i = 0; i != predictions.size(); i++)
    {
        if(i == 0)
        {
            sf::Vector2f startPos(predictions[i].x,predictions[i].y);
            //effects.createLine(startPos.x,startPos.y,pos.x,pos.y,1,sf::Color::Cyan);
        }
        else if(i == predictions.size()-1)
        {
            sf::Vector2f startPos(predictions[i].x,predictions[i].y);
            sf::Vector2f endPos(predictions[i-1].x,predictions[i-1].y);
            effects.createLine(startPos.x,startPos.y,endPos.x,endPos.y,1,sf::Color::Red);
        }
        else
        {
            sf::Vector2f startPos(predictions[i].x,predictions[i].y);
            sf::Vector2f endPos(predictions[i-1].x,predictions[i-1].y);
            effects.createLine(startPos.x,startPos.y,endPos.x,endPos.y,1,sf::Color::Cyan);
        }
    }

    for(int i = 0; i != positions.size(); i++)
    {
        if(i == positions.size()-1)
        {
            sf::Vector2f startPos(positions[i].x,positions[i].y);
            effects.createLine(startPos.x,startPos.y,pos.x,pos.y,1,sf::Color(150,150,150));
        }
        else
        {
            sf::Vector2f startPos(positions[i].x,positions[i].y);
            sf::Vector2f endPos(positions[i+1].x,positions[i+1].y);
            effects.createLine(startPos.x,startPos.y,endPos.x,endPos.y,1,sf::Color(150,150,150));
        }
    }

    if((lifetime % 50) == 50)
    {
        angle = randz(0,360);
        positions.push_back(pos);
    }



    lifetime--;
    if(lifetime <= 0)
        toDelete = true;
}

Bullet::Bullet()
{
    pos = Vec3f(0,0,0);
    angle = 0;
    lifetime = -1;
    speed = 5;
    penetration = 10;
    health = 5;
    maxrichochet = 5;
    toDelete = false;
    owner = nullptr;
    parent = nullptr;
}

std::vector<Bullet> bullets;

void removeBullets()
{
    bool done = false;
    while (done == false)
    {
        bool yet = false;
        for (auto it = bullets.begin(); it != bullets.end(); ++it)
        {
            if (it->toDelete)
            {
                bullets.erase(it);
                yet = true;
                break;
            }
        }
        if (yet == false)
        {
            done = true;
        }
    }
}
