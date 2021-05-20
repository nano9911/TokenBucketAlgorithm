#pragma once
#include <iostream>
#include <random>
#include <array>
#include <string>
#include <ctime>

//#define DEBUG

constexpr auto TICKS = 2;

struct BucketStruct {
    /* maximum number of tokens in the bucket */
    uint64_t ul64MaxCapacity; /* defined at startup */
    uint64_t ul64CurrentTokens;
    /* number of tokens added at each tick */
    uint64_t ul64TokenRate; /* defined at startup */
};

struct QueueStruct {
    uint64_t ul64MaxCapacity; /* defined at startup */
    uint64_t ul64FreeSpace; /* defined at startup */
    uint64_t ul64CurrentPackets;
    /* maximum number of arriving packets at each tick */
    uint64_t ul64MaxPacketArrivalRate; /* defined at startup */
};

struct Packet {
    struct Packet* before;
    uint64_t ul64Size;
    time_t ArrivalTime;
    struct Packet* next;
};

struct Ticks {
    time_t StartedAt;
    uint64_t ul64BucketTockens;
    uint64_t ul64QueuePackets;
    std::string sArrivedPackets;
    int iArrivedPackets;
    int iDepartedPackets;
    int iDroppedPackets;
};


class TokenBucketAlgorithm {
private:
    struct BucketStruct Bucket = { 0,0,0 };
    struct QueueStruct Queue = { 0,0,0,0 };
    std::array<struct Ticks, TICKS + 1> Tick;

    struct Packet* FirstPacketInQueue = nullptr;
    struct Packet* LastPacketInQueue = nullptr;
    struct Packet* ArrivedPacket = nullptr;

    uint64_t ul64PacketMaxSize = 0; /* defined at startup */
    time_t StartupTime = 0;
    int iDroppedPackets = 0;
    int iDepartedPackets = 0;
    int iArrivedPackets = 0;

    uint64_t GetRandomPacketArrivalRate() const;

    struct Packet* NewPacket() {
        return new struct Packet;
    }
    void GenRandomArrivedPacket(int i);
    void DropPacket();
    void AddPacketToQueue();

    bool SendPacket();

public:
    TokenBucketAlgorithm(uint64_t b_sz, uint64_t b_tr,
        uint64_t q_sz, uint64_t q_par,
        uint64_t p_mxsz);
    void OneTick(int e);
    void Run();
    void print();

#ifdef DEBUG
    void get_status();
#endif

    ~TokenBucketAlgorithm();
};
