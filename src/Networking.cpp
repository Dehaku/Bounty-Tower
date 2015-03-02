#include "Networking.h"
#include "globalvars.h"
#include <iostream>
#include <iomanip>
#include <stdio.h>

#include <string>




namespace network
{
    int mainPort = 23636;
    bool packetDeletion = false;
    bool servWait = false;
    bool cliWait = false;
    bool server = false;
    bool client = false;
    bool chatting = false;
    std::string name;
}

int displayPort()
{
    return network::mainPort;
}


Identity::Identity()
{
    wrongVersion = "Wrong Version";
    connection = "Connection";
    connectionSuccessful = "Connection Successful";
    textMessage = "Text Message";
    drawStuffs = "Draw Stuffs";
    grid = "Grid";
    peers = "Peers";
    clientMouse = "Client Mouse Position";
}
Identity ident;


sf::IpAddress server("127.0.0.1");
bool TcpFirstRun = true;
sf::TcpListener Servlistener;
sf::TcpSocket Servsocket;
sf::TcpSocket Clisocket;
std::list<sf::TcpSocket*> clients;
sf::SocketSelector selector;


BoolPacket::BoolPacket()
{
    toDelete = false;
}
std::vector<BoolPacket> packetContainer;


/////////////////////////////////////////////////// /////////
/// Launch a server, wait for an incoming connection,
/// send a message and wait for the answer.
///
////////////////////////////////////////////////////////////

ServerController::ServerController()
{
    waiting = false;
    conID = 100;
}
ServerController servCon;

ClientController::ClientController()
{
    mode = "Local";
    waiting = false;
    connected = false;
    chatting = false;
    name = "";
    ID = -1;
}
ClientController cliCon;

Peer::Peer()
{
    name = "";
    ID = servCon.conID++;
}
Peers peers;

void DealPackets()
{
    if(gvars::debug) std::cout << "DealPacket Begins" << packetContainer.size() << std::endl;
    int PackLimit = packetContainer.size();
    for(int i = 0; i != PackLimit; i++)
    {
                    //packetContainer[i].Packet
                    std::string GotIdent;
                    packetContainer[i].packet >> GotIdent;
                    if(gvars::debug) std::cout << "GotIdent: \n" << GotIdent << std::endl;
                    if(GotIdent != "")
                    {
                        cliCon.waiting = false;
                        if(gvars::debug) std::cout << "Message received from server " << ", Type:" << GotIdent << std::endl;
                        if(GotIdent == ident.wrongVersion)
                        {
                            std::string ver;
                            packetContainer[i].packet >> ver;

                            std::cout << "You have the wrong version. \n";
                            std::cout << "Servers Version: " << ver << ", Your Version: " << gvars::version << std::endl;
                            std::cout << "You should acquire the same version as the server. \n";
                            cliCon.connected = false;
                        }

                        if(GotIdent == ident.connectionSuccessful)
                        {
                            std::cout << "Your now connected to " << cliCon.server << std::endl;
                            cliCon.connected = true;
                        }
                        if(GotIdent == ident.textMessage)
                        {
                            if(gvars::debug) std::cout << "Dealing with Text Message, Pack: " << i << std::endl;
                            std::string Text;
                            packetContainer[i].packet >> Text;
                            cliCon.chatHistory.push_back(Text);
                            std::cout << "* " << Text;
                            sf::Packet SendPacket;
                            SendPacket << ident.textMessage << Text;

                            for (std::list<sf::TcpSocket*>::iterator zit = clients.begin(); zit != clients.end(); ++zit)
                            {
                                if(gvars::debug) std::cout << "Running through clients \n";
                                sf::TcpSocket& clientz = **zit;
                                clientz.send(SendPacket);
                            }

                            if(gvars::debug) std::cout << "Done with Text Message Packet:" << i << std::endl;

                        }
                        if(GotIdent == ident.drawStuffs)
                        {
                            //NeedsToDraw = true;
                        }
                        if(GotIdent == ident.connection)
                        {
                            Peer peer;
                            packetContainer[i].packet >> peer.name;
                            peers.connected.push_back(peer);
                        }
                        if(GotIdent == ident.clientMouse)
                        {

                            std::string Name;
                            packetContainer[i].packet >> Name;
                            if(gvars::debug) std::cout << "Dealing with ClientMouse from," << Name << ", Pack: " << i << std::endl;
                            for(int i = 0; i != peers.connected.size(); i++)
                            {
                                if(Name == peers.connected[i].name)
                                {
                                    //packetContainer[i].Packet >> peers.Connected[i].MousePos.x >> peers.Connected[i].MousePos.y;

                                    sf::Uint32 x;
                                    sf::Uint32 y;
                                    packetContainer[i].packet >> x >> y;
                                    peers.connected[i].mousePos.x = x;
                                    peers.connected[i].mousePos.y = y;


                                }
                            }

                        }

                    }
                    packetContainer[i].toDelete = true;

    }
    bool DoneDeleting = false;
    std::vector<BoolPacket>::iterator EndLimit = packetContainer.end();



    std::vector<NestClass> Nested;




    int DeleteAmount = 0;
    for(std::vector<BoolPacket>::iterator i = packetContainer.begin(); i != EndLimit; i++)
    {
        bool CatchAll = false;
        if(i->toDelete == true)
        {
            DeleteAmount++;

            NestClass NC;
            NC.nestIter = i;

            Nested.push_back(NC);
            //CatchAll = true;
            //EndLimit--;
        }
    }
    //packetContainer.erase(packetContainer.begin(),packetContainer.begin()+DeleteAmount);

    if(gvars::debug) std::cout << "Nested: " << Nested.size() << std::endl;
    network::packetDeletion = true;
    for(int i = Nested.size()-1; i != -1; i--)
    {
        if(gvars::debug) std::cout << "Removed: " << i << std::endl;
        packetContainer.erase( Nested[i].nestIter );

    }
    Nested.clear();
    network::packetDeletion = false;

    //Global.PackWait = false;

    /*
    for(std::vector<std::vector<BoolPacket>::iterator>::iterator i = Nested.end(); i != Nested.begin(); i--)
    {
        packetContainer.erase(i);
    }
    */



    /*
    while(DoneDeleting == false)
    {
        std::vector<BoolPacket>::iterator i;
        DoneDeleting = true;
        // try{
        for(i = packetContainer.begin(); i != EndLimit; i++)
        {
            bool CatchAll = false;

            if(i->ToDelete == true)
            {
                DoneDeleting = false;
                packetContainer.erase(i);
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
                    BP.packet = GotPacket;
                    int Delay = 0;
                    while(network::packetDeletion == true)
                    {
                        std::cout << " " << Delay++;
                    }

                    packetContainer.push_back(BP);
                    //std::cout << "Packet Size: " << sizeof(GotPacket);
                    //std::cout << ", PC: " << packetContainer.size();


                    if(gvars::debug) std::cout << "Client is done. \n";
                }
                else
                {
                    std::cout << "IsReady(Client) is false \n";
                }
            }
        }
    }
    //if(gvars::debug)
        std::cout << "Do we make it here? \n";
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
        cliCon.waiting = false;
        if(gvars::debug) std::cout << "Message received from server " << ", Type:" << GotIdent << std::endl;
        if(GotIdent == ident.wrongVersion)
        {
            std::string ver;
            GotPacket >> ver;

            std::cout << "You have the wrong version. \n";
            std::cout << "Servers Version: " << ver << ", Your Version: " << gvars::version << std::endl;
            std::cout << "You should acquire the same version as the server. \n";
            cliCon.connected = false;
        }
        if(GotIdent == ident.connectionSuccessful)
        {
            std::cout << "Your now connected to " << cliCon.server << std::endl;
            cliCon.connected = true;
        }
        if(GotIdent == ident.textMessage)
        {
            std::string Text;
            GotPacket >> Text;
            cliCon.chatHistory.push_back(Text);
            std::cout << Text;
        }
        if(GotIdent == ident.drawStuffs)
        {
            //NeedsToDraw = true;
        }
        if(GotIdent == ident.clientMouse)
        {
            std::string Name;
            GotPacket >> Name;
            bool Exists = false;
            for(int i = 0; i != peers.connected.size(); i++)
            {
                if(Name == peers.connected[i].name)
                    Exists = true;
            }
            if(!Exists)
            {
                Peer peer;
                peer.name = Name;
                peer.mousePos = sf::Vector2f(5,5);
                peers.connected.push_back(peer);
            }
            //std::cout << "Dealing with ClientMouse from," << Name << ", Pack: " << i << std::endl;
            for(int i = 0; i != peers.connected.size(); i++)
            {
                if(Name == peers.connected[i].name)
                {
                    sf::Uint32 x;
                    sf::Uint32 y;
                    GotPacket >> x >> y;
                    peers.connected[i].mousePos.x = x;
                    peers.connected[i].mousePos.y = y;

                }
            }
        }

    }

    //Global.CliWait = false;

}

bool chatCommand(std::string input)
{
    std::vector<std::string> elements;
    bool finished = false;
    sf::Color errorColor(100,100,100);
    sf::Color warmColor(255,150,150);
    sf::Color goodColor = sf::Color::White;
    size_t tStart = 0;
    size_t tEnd = 0;

    while(finished == false)
    {

        tEnd = input.find(" ",tStart);
        std::string injection;
        injection.append(input,tStart,tEnd-tStart);
        elements.push_back(injection);
        tStart = tEnd+1;
        if(tEnd == input.npos)
            finished = true;
    }
    std::cout << "input: " << input << std::endl;
    for(auto &i : elements)
    {
        std::cout << "elements: " << i << std::endl;
    }
    if(elements[0] == "/connect")
    {
        std::cout << "Connect chat command detected. \n";
        if(network::name == "")
        {
            chatBox.addChat("Server: Error, please give yourself a name with /setname before attempting to connect.", errorColor);
        }
        try
        {
            int test = std::stoi(elements[2]);
        }
        catch (std::exception &e)
        {
            chatBox.addChat("Command: /connect [IP Address] [Port]", errorColor);
            return false;
        }

        if (Clisocket.connect(elements[1], std::stoi(elements[2])) == sf::Socket::Done)
        {
            std::cout << "Connected to server " << elements[1] << std::endl;
        }

            sf::Packet packet;

            packet << ident.connection << network::name;
            Clisocket.send(packet);
            packet.clear();
            packet << ident.clientMouse << network::name << gvars::mousePos.x << gvars::mousePos.y;
            Clisocket.send(packet);
            packet.clear();
            packet << ident.textMessage << network::name + randomWindowName();
            Clisocket.send(packet);

            chatBox.addChat("Server: Connected to " + elements[1] + "(" + elements[2] + ")", goodColor);


        return true;
    }
    else if(elements[0] == "/setname")
    {
        chatBox.addChat("Server: " + network::name + " has changed their name to " + elements[1], goodColor);
        network::name = elements[1];
        if(elements[1] == "Lithi" || elements[1] == "Biocava" || elements[1] == "Sneaky" || elements[1] == "SneakySnake")
            chatBox.addChat("Server: Ooo, Ooo, I like you!", warmColor);
        if(elements[1] == "Argwm" || elements[1] == "Dehaku")
            chatBox.addChat("Server: Hey, that's my masters name!", warmColor);
        return true;
    }
    else if(elements[0] == "/repeat")
    {
        try
        {
            int test = std::stoi(elements[1]);
        }
        catch (std::exception &e)
        {
            chatBox.addChat("Invalid argument: " + elements[1] + " in command " + input, errorColor);
            chatBox.addChat("Command: /repeat [numberoftimes] [series of words or numbers]", errorColor);
            return false;
        }
        std::string repeatingLine;
        for(int i = 0; i != elements.size(); i++)
        {
            if(i != 0 && i != 1)
            {
                repeatingLine.append(elements[i] + " ");
            }
        }
        for(int i = 0; i != std::stoi(elements[1]); i++)
        {
            chatBox.addChat("Server: Repeating; " + repeatingLine, goodColor);
        }
        return true;
    }

    chatBox.addChat("Unrecognized command: " + input, errorColor);
    return false;
}




















