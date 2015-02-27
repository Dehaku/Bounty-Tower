#include "Networking.h"
#include "globalvars.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <list>
#include <string>




namespace network
{
    int mainPort = 23636;
    bool packetDeletion = false;
}

int displayPort()
{
    return network::mainPort;
}

class Identity
{
    public: // You could say this is redundent, But I don't care for Enums, And this prevents idiotic programming errors.
    std::string WrongVersion;
    std::string Connection;
    std::string ConnectionSuccessful;
    std::string TextMessage;
    std::string DrawStuffs;
    std::string Grid;
    std::string Peers;
    std::string ClientMouse;

    Identity()
    {
        WrongVersion = "Wrong Version";
        Connection = "Connection";
        ConnectionSuccessful = "Connection Successful";
        TextMessage = "Text Message";
        DrawStuffs = "Draw Stuffs";
        Grid = "Grid";
        Peers = "Peers";
        ClientMouse = "Client Mouse Position";
    }


};
Identity Ident;

sf::TcpListener Servlistener;
sf::TcpSocket Servsocket;
sf::TcpSocket Clisocket;
sf::IpAddress server("127.0.0.1");

std::list<sf::TcpSocket*> clients;

sf::SocketSelector selector;
bool TcpFirstRun = true;

class BoolPacket
{
    public:
    sf::Packet Packet;
    bool ToDelete;
    BoolPacket()
    {
        ToDelete = false;
    }
};

std::vector<BoolPacket> PacketContainer;




class MyTestClass
{
    public:
    int Monkey;
    int Add(int ValOne, int ValTwo)
    {
        int ReturnVal = ValOne+ValTwo;
        return ReturnVal;
    }
    MyTestClass()
    {
        Monkey = 69;
    }
};



/////////////////////////////////////////////////// /////////
/// Launch a server, wait for an incoming connection,
/// send a message and wait for the answer.
///
////////////////////////////////////////////////////////////


class Player
{
    public:
    std::string Name;
    sf::Vector2f Pos;
    int Jumped;
    int JumpLimit;
    float JumpValue;

    Player()
    {

    }
};



class ServerController
{
    public:
    bool Waiting;
    int ConID;
    std::vector<std::string> ChatHistory;


    ServerController()
    {
        Waiting = false;
        ConID = 100;
    }
};
ServerController ServCon;



class ClientController
{
    public:
    std::string Mode;
    bool Waiting;
    bool Connected;
    bool Chatting;
    sf::IpAddress Server;
    std::string Name;
    int ID;
    std::string ChatString;
    std::vector<std::string> ChatHistory;
    std::vector<sf::Packet> Packets;

    ClientController()
    {
        Mode = "Local";
        Waiting = false;
        Connected = false;
        Chatting = false;
        Name = "";
        ID = -1;
    }
};
ClientController CliCon;




class Peer
{
    public:
    std::string Name;
    int ID;
    sf::IpAddress IP;
    sf::Vector2f Pos;
    sf::Vector2f MousePos;
    sf::Vector2f Mom;
    sf::Sprite Img;
    unsigned short Port;
    Peer()
    {
        Name = "";
        ID = ServCon.ConID++;
        //Pos = sf::Vector2f(Random(50,700),50);
        //Img.setTexture( *imagemanager.GetImage("BoxMan.bmp"));
    }
};

class Peers
{
    public:
    std::vector<Peer> Connected;
};
Peers peers;

class NestClass
{
    public:
    std::vector<BoolPacket>::iterator NestIter;
};


void DealPackets()
{
    if(gvars::debug) std::cout << "DealPacket Begins" << PacketContainer.size() << std::endl;
    int PackLimit = PacketContainer.size();
    for(int i = 0; i != PackLimit; i++)
    {
                    //PacketContainer[i].Packet
                    std::string GotIdent;
                    PacketContainer[i].Packet >> GotIdent;
                    if(gvars::debug) std::cout << "GotIdent: \n" << GotIdent << std::endl;
                    if(GotIdent != "")
                    {
                        CliCon.Waiting = false;
                        if(gvars::debug) std::cout << "Message received from server " << ", Type:" << GotIdent << std::endl;
                        if(GotIdent == Ident.WrongVersion)
                        {
                            std::string ver;
                            PacketContainer[i].Packet >> ver;

                            std::cout << "You have the wrong version. \n";
                            std::cout << "Servers Version: " << ver << ", Your Version: " << gvars::version << std::endl;
                            std::cout << "You should acquire the same version as the server. \n";
                            CliCon.Connected = false;
                        }

                        if(GotIdent == Ident.ConnectionSuccessful)
                        {
                            std::cout << "Your now connected to " << CliCon.Server << std::endl;
                            CliCon.Connected = true;
                        }
                        if(GotIdent == Ident.TextMessage)
                        {
                            if(gvars::debug) std::cout << "Dealing with Text Message, Pack: " << i << std::endl;
                            std::string Text;
                            PacketContainer[i].Packet >> Text;
                            CliCon.ChatHistory.push_back(Text);
                            std::cout << Text;
                            sf::Packet SendPacket;
                            SendPacket << Ident.TextMessage << Text;

                            for (std::list<sf::TcpSocket*>::iterator zit = clients.begin(); zit != clients.end(); ++zit)
                            {
                                if(gvars::debug) std::cout << "Running through clients \n";
                                sf::TcpSocket& clientz = **zit;
                                clientz.send(SendPacket);
                            }

                            if(gvars::debug) std::cout << "Done with Text Message Packet:" << i << std::endl;

                        }
                        if(GotIdent == Ident.DrawStuffs)
                        {
                            //NeedsToDraw = true;
                        }
                        if(GotIdent == Ident.Connection)
                        {
                            Peer peer;
                            PacketContainer[i].Packet >> peer.Name;
                            peers.Connected.push_back(peer);
                        }
                        if(GotIdent == Ident.ClientMouse)
                        {

                            std::string Name;
                            PacketContainer[i].Packet >> Name;
                            if(gvars::debug) std::cout << "Dealing with ClientMouse from," << Name << ", Pack: " << i << std::endl;
                            for(int i = 0; i != peers.Connected.size(); i++)
                            {
                                if(Name == peers.Connected[i].Name)
                                {
                                    //PacketContainer[i].Packet >> peers.Connected[i].MousePos.x >> peers.Connected[i].MousePos.y;

                                    sf::Uint32 x;
                                    sf::Uint32 y;
                                    PacketContainer[i].Packet >> x >> y;
                                    peers.Connected[i].MousePos.x = x;
                                    peers.Connected[i].MousePos.y = y;


                                }
                            }

                        }

                    }
                    PacketContainer[i].ToDelete = true;

    }
    bool DoneDeleting = false;
    std::vector<BoolPacket>::iterator EndLimit = PacketContainer.end();



    std::vector<NestClass> Nested;




    int DeleteAmount = 0;
    for(std::vector<BoolPacket>::iterator i = PacketContainer.begin(); i != EndLimit; i++)
    {
        bool CatchAll = false;
        if(i->ToDelete == true)
        {
            DeleteAmount++;

            NestClass NC;
            NC.NestIter = i;

            Nested.push_back(NC);
            //CatchAll = true;
            //EndLimit--;
        }
    }
    //PacketContainer.erase(PacketContainer.begin(),PacketContainer.begin()+DeleteAmount);

    if(gvars::debug) std::cout << "Nested: " << Nested.size() << std::endl;
    network::packetDeletion = true;
    for(int i = Nested.size()-1; i != -1; i--)
    {
        if(gvars::debug) std::cout << "Removed: " << i << std::endl;
        PacketContainer.erase( Nested[i].NestIter );

    }
    Nested.clear();
    network::packetDeletion = false;

    //Global.PackWait = false;

    /*
    for(std::vector<std::vector<BoolPacket>::iterator>::iterator i = Nested.end(); i != Nested.begin(); i--)
    {
        PacketContainer.erase(i);
    }
    */



    /*
    while(DoneDeleting == false)
    {
        std::vector<BoolPacket>::iterator i;
        DoneDeleting = true;
        // try{
        for(i = PacketContainer.begin(); i != EndLimit; i++)
        {
            bool CatchAll = false;

            if(i->ToDelete == true)
            {
                DoneDeleting = false;
                PacketContainer.erase(i);
                Nested.push_back(i);
                CatchAll = true;
                EndLimit--;
                break;
            }
            if(CatchAll)
            {
                std::cout << "Caught somefin' Paul! \n";
                break;
            }
        }
        // }catch (std::exception& e){ std::cout << "Packet Container try-fail! \n"; }
    }
    */


}


