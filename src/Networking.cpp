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


Identity::Identity()
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
Identity Ident;


sf::TcpListener Servlistener;
sf::TcpSocket Servsocket;
sf::TcpSocket Clisocket;
sf::IpAddress server("127.0.0.1");

std::list<sf::TcpSocket*> clients;

sf::SocketSelector selector;
bool TcpFirstRun = true;

BoolPacket::BoolPacket()
{
    ToDelete = false;
}
std::vector<BoolPacket> PacketContainer;


/////////////////////////////////////////////////// /////////
/// Launch a server, wait for an incoming connection,
/// send a message and wait for the answer.
///
////////////////////////////////////////////////////////////

ServerController::ServerController()
{
    Waiting = false;
    ConID = 100;
}
ServerController ServCon;

ClientController::ClientController()
{
    Mode = "Local";
    Waiting = false;
    Connected = false;
    Chatting = false;
    Name = "";
    ID = -1;
}
ClientController CliCon;

Peer::Peer()
{
    Name = "";
    ID = ServCon.ConID++;
}
Peers peers;

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


void runTcpServer(unsigned short port)
{
    // Create a server socket to accept new connections
    //Code ripped to Main.cpp, Code 555000999
    // Wait for a connection
    if(TcpFirstRun)
    {
        TcpFirstRun = false;


    /*
    sf::TcpSocket* client = new sf::TcpSocket;
    if (Servlistener.accept(*client) != sf::Socket::Done)
    {
        std::cout << "Infinite? \n";
        return;
    }


    //std::cout << "Client connected: " << client.getRemoteAddress() << std::endl;
    selector.add(*client);
    clients.push_back(client);
    */

    }


    while(selector.wait())
    {
        std::cout << "Wait Successful! \n";
        if(selector.isReady(Servlistener))
        {
            std::cout << "Listener is ready!=======================================================(NoticeMe)======================================== \n";
            sf::TcpSocket* client = new sf::TcpSocket;
            if (Servlistener.accept(*client) == sf::Socket::Done)
            {
                selector.add(*client);
                clients.push_back(client);
            }
            else
            {
                std::cout << "Deleting a Client! Is this normal? \n";
                delete client;
            }
            std::cout << "Listener is done, Moving on. \n";
        }
        else
        {
            for (std::list<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
            {
                //std::cout << "Running through clients \n";
                sf::TcpSocket& client = **it;
                if(selector.isReady(client))
                {
                    if(gvars::debug) std::cout << "Client is ready! \n";
                    sf::Packet GotPacket;
                    if(client.receive(GotPacket) != sf::Socket::Done)
                    {
                        std::cout << "Not Ready";
                        return;
                    }


                    BoolPacket BP;
                    BP.Packet = GotPacket;
                    int Delay = 0;
                    while(network::packetDeletion == true)
                    {
                        std::cout << " " << Delay++;
                    }

                    PacketContainer.push_back(BP);
                    //std::cout << "Packet Size: " << sizeof(GotPacket);
                    //std::cout << ", PC: " << PacketContainer.size();


                    if(gvars::debug) std::cout << "Client is done. \n";
                }
                else
                {
                    std::cout << "IsReady(Client) is false \n";
                }
            }
        }
    }
    if(gvars::debug) std::cout << "Do we make it here? \n";
    //DealPackets();
    if(gvars::debug) std::cout << "And if so, How about here? \n";
    //Global.ServWait = false;

}



void runTcpClient(unsigned short port)
{
    // Create a socket for communicating with the server
    // Connect to the server
    //Code ripped to Main.cpp, Code 555000998

    if(gvars::debug) std::cout << "Waiting on Message! \n";

    sf::Packet GotPacket;
    if (Clisocket.receive(GotPacket) != sf::Socket::Done)
        return;

    std::string GotIdent;
    GotPacket >> GotIdent;
    if(gvars::debug) std::cout << "GotIdent: " << GotIdent << std::endl;
    if(GotIdent != "")
    {
        CliCon.Waiting = false;
        if(gvars::debug) std::cout << "Message received from server " << ", Type:" << GotIdent << std::endl;
        if(GotIdent == Ident.WrongVersion)
        {
            std::string ver;
            GotPacket >> ver;

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
            std::string Text;
            GotPacket >> Text;
            CliCon.ChatHistory.push_back(Text);
            std::cout << Text;
        }
        if(GotIdent == Ident.DrawStuffs)
        {
            //NeedsToDraw = true;
        }
        if(GotIdent == Ident.ClientMouse)
        {
            std::string Name;
            GotPacket >> Name;
            bool Exists = false;
            for(int i = 0; i != peers.Connected.size(); i++)
            {
                if(Name == peers.Connected[i].Name) Exists = true;
            }
            if(!Exists)
            {
                Peer peer;
                peer.Name = Name;
                peer.MousePos = sf::Vector2f(5,5);
                peers.Connected.push_back(peer);
            }
            //std::cout << "Dealing with ClientMouse from," << Name << ", Pack: " << i << std::endl;
            for(int i = 0; i != peers.Connected.size(); i++)
            {
                if(Name == peers.Connected[i].Name)
                {
                    sf::Uint32 x;
                    sf::Uint32 y;
                    GotPacket >> x >> y;
                    peers.Connected[i].MousePos.x = x;
                    peers.Connected[i].MousePos.y = y;

                }
            }
        }

    }

    //Global.CliWait = false;

}


