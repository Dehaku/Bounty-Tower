#ifndef NETWORKING_H_INCLUDED
#define NETWORKING_H_INCLUDED

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <list>
#include "util.h"
#include "Text.h"
#include "Tiles.h"
#include "Faction.h"
#include "Textures.h"

namespace network
{
    extern int mainPort;
    extern bool packetDeletion;
    extern bool servWait;
    extern bool cliWait;
    extern bool server;
    extern bool client;
    extern bool chatting;
    extern bool needTime;
    extern bool givingTime;
    extern std::string name;
    extern std::string connectedServer;
}

int displayPort();

extern sf::TcpListener servListener;
extern sf::TcpSocket servSocket;
extern sf::TcpSocket cliSocket;
extern sf::IpAddress server;

extern std::list<sf::TcpSocket*> clients;

extern sf::SocketSelector selector;
extern bool TcpFirstRun;

class Identity
{
    public: // You could say this is redundent, But I don't care for Enums, And this prevents idiotic programming errors.
    std::string wrongVersion;
    std::string connection;
    std::string connectionSuccessful;
    std::string textMessage;
    std::string drawStuffs;
    std::string grid;
    std::string peers;
    std::string clientMouse;
    std::string gridUpdate;
    std::string tilesUpdate;
    std::string ping;
    std::string pong;
    std::string updateRoster;

    Identity();
};
extern Identity ident;



class BoolPacket
{
    public:
    sf::Packet packet;
    bool toDelete;
    BoolPacket();
};

extern std::vector<BoolPacket> packetContainer;

class ServerController
{
    public:
    bool waiting;
    int conID;
    std::vector<std::string> chatHistory;
    void updateClients();


    ServerController();
};
extern ServerController servCon;

class ClientController
{
    public:
    std::string mode;
    bool waiting;
    bool connected;
    bool chatting;
    sf::IpAddress server;
    std::string name;
    int ID;
    std::string chatString;
    std::vector<std::string> chatHistory;
    std::vector<sf::Packet> packets;

    ClientController();
};
extern ClientController cliCon;

class Peer
{
    public:
    std::string name;
    int ID;
    sf::IpAddress IP;
    sf::Vector2f pos;
    sf::Vector2f mousePos;
    sf::Vector2f mom;
    sf::Sprite img;
    unsigned short port;
    int ping;
    Peer();
};

class Peers
{
    public:
    std::vector<Peer> connected;
};
extern Peers peers;

class NestClass
{
    public:
    std::vector<BoolPacket>::iterator nestIter;
};

void DealPackets();

void runTcpServer(unsigned short port);

void runTcpClient(unsigned short port);

void tcpSendtoAll(sf::Packet pack);

bool chatCommand(std::string input);

#endif // NETWORKING_H_INCLUDED
