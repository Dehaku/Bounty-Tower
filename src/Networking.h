#ifndef NETWORKING_H_INCLUDED
#define NETWORKING_H_INCLUDED

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

namespace network
{
    extern int mainPort;
    extern bool packetDeletion;
}

int displayPort();



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

    Identity();
};
extern Identity Ident;



class BoolPacket
{
    public:
    sf::Packet Packet;
    bool ToDelete;
    BoolPacket();
};

extern std::vector<BoolPacket> PacketContainer;

class ServerController
{
    public:
    bool Waiting;
    int ConID;
    std::vector<std::string> ChatHistory;


    ServerController();
};
extern ServerController ServCon;

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

    ClientController();
};
extern ClientController CliCon;

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
    Peer();
};

class Peers
{
    public:
    std::vector<Peer> Connected;
};
extern Peers peers;

class NestClass
{
    public:
    std::vector<BoolPacket>::iterator NestIter;
};

void DealPackets();

void runTcpServer(unsigned short port);

void runTcpClient(unsigned short port);

#endif // NETWORKING_H_INCLUDED
