//#pragma once
#include "TokenBucketAlgorithm.h"

/**
 * @brief Construct a new token bucket algo::token bucket algo object
 * Initialize variables and structures.
 *
 * @param b_sz bucket maximum size
 * @param b_tr bucket tocken rate
 * @param q_sz queue size
 * @param q_par queue maximum packet arrival eate
 * @param p_mxsz packets maximum size
 */
TokenBucketAlgorithm::TokenBucketAlgorithm(uint64_t b_sz, uint64_t b_tr,
    uint64_t q_sz, uint64_t q_par,
    uint64_t p_mxsz) :
    Bucket{ b_sz, b_sz, b_tr },
    Queue{ q_sz, q_sz, 0, q_par },
    ul64PacketMaxSize(p_mxsz)   {

    for (int i = 0; i < TICKS; i++)
        Tick[i] = { 0,0,0,"",0,0,0 };
}

#ifdef DEBUG
void TokenBucketAlgorithm::get_status() {
    std::cerr << "\t\tQueue.ul64FreeSpace:\t" << Queue.ul64FreeSpace;
    std::cerr << "\t\tBucket.ul64CurrentTokens:\t" << Bucket.ul64CurrentTokens;
    std::cerr << "\t\tQueue.ul64CurrentPackets:\t" << Queue.ul64CurrentPackets << "\n\n";
}
#endif

uint64_t TokenBucketAlgorithm::GetRandomPacketArrivalRate() const {
    std::random_device Rate;
    return Rate() % Queue.ul64MaxPacketArrivalRate + 1;
}

/**
 * @brief generate (allocate) new packet which represents an arrived
 * packet, with random size, and initializing the arrival time variable.
 */
void TokenBucketAlgorithm::GenRandomArrivedPacket(int i) {
#ifdef DEBUG
    std::cerr << "TokenBucketAlgorithm:GenRandomArrivedPacket() started\n";
#endif

    std::random_device R;
    ArrivedPacket = NewPacket();

    ArrivedPacket->ArrivalTime = time(nullptr);
    ArrivedPacket->ul64Size = R() % ul64PacketMaxSize + 1;

    ArrivedPacket->before = nullptr;
    ArrivedPacket->next = nullptr;

    iArrivedPackets++;

    Tick[i].sArrivedPackets.append(std::to_string(ArrivedPacket->ul64Size));
#ifdef DEBUG
    char ArrivalTime[26];
    ctime_r(&(ArrivedPacket->ArrivalTime), ArrivalTime);
    /*    ctime_s(ArrivalTime, sizeof ArrivalTime, &(ArrivedPacket->ArrivalTime));
        std::cerr << "\tPacket " << iArrivedPackets << " created of size " <<
            ArrivedPacket->ul64Size << " at " << ArrivalTime;
    */
    std::cerr << "TokenBucketAlgorithm:GenRandomArrivedPacket() finished\n\n";
#endif
}

void TokenBucketAlgorithm::DropPacket() {
#ifdef DEBUG
    std::cerr << "\tPacket " << iArrivedPackets << " being dropped, of size " <<
        ArrivedPacket->ul64Size << "\n";
#endif
    delete ArrivedPacket;
    iDroppedPackets++;
}

/**
 * @brief check if the queue is full or have a space to add the arrived packet,
 * if it's then add it to the queue by assigning the last_packet_in_queue to
 * the arrived packet. If it's not then drop the packet by freeing the new
 * allocated space for it.
 */
void TokenBucketAlgorithm::AddPacketToQueue() {
#ifdef DEBUG
    std::cerr << "TokenBucketAlgorithm:AddPacketToQueue() started\n";
#endif
    if (Queue.ul64FreeSpace < ArrivedPacket->ul64Size) {
        DropPacket();
    }
    else {
        if (Queue.ul64CurrentPackets == 0) {
            LastPacketInQueue = ArrivedPacket;
            FirstPacketInQueue = ArrivedPacket;
        }
        else {
            ArrivedPacket->next = LastPacketInQueue;
            LastPacketInQueue->before = ArrivedPacket;
            LastPacketInQueue = ArrivedPacket;

        }
#ifdef DEBUG
        std::cerr << "\tPacket " << iArrivedPackets << " added to queue, of size " <<
            ArrivedPacket->ul64Size << "\n";
#endif
        Queue.ul64CurrentPackets++;
        Queue.ul64FreeSpace -= LastPacketInQueue->ul64Size;
    }
#ifdef DEBUG
    std::cerr << "TokenBucketAlgorithm:AddPacketToQueue() finished\n\n";
#endif
}

/**
 * @brief send the first packet in the queue (FIFO queue) if there's enouph tockens
 * in the bucket equal to the packet size, then return TRUE.
 * But, if there's not enouph tokens, return false to stop trying and wait for the
 * next tick to try again.
 *
 * @return true --> packet sent
 * @return false --> not enouph token to send packet
 */
bool TokenBucketAlgorithm::SendPacket() {
#ifdef DEBUG
    std::cerr << "TokenBucketAlgorithm:SendPacket() started\n";
#endif
    if (FirstPacketInQueue != nullptr &&
        Bucket.ul64CurrentTokens < FirstPacketInQueue->ul64Size ||
        Queue.ul64CurrentPackets == 0) {
#ifdef DEBUG
        std::cerr << "\tTokenBucketAlgorithm:SendPacket() failed, not enouph tokens\
, or queue is empty.\n";
#endif
        return false;
    }
#ifdef DEBUG
    std::cerr << "\tPacket being sent of size " <<
        ArrivedPacket->ul64Size << "\n";
#endif
    if (FirstPacketInQueue != nullptr) {
        struct Packet* temp = FirstPacketInQueue;
        FirstPacketInQueue = FirstPacketInQueue->before;

        /* modify queue available space and bucket tockens count */
        Queue.ul64FreeSpace += temp->ul64Size;
        Bucket.ul64CurrentTokens -= temp->ul64Size;

        delete temp;
        iDepartedPackets++;
        Queue.ul64CurrentPackets--;

#ifdef DEBUG
        get_status();
        std::cerr << "TokenBucketAlgorithm:SendPacket() finished\n\n";
#endif
    }
    return true;
}

void TokenBucketAlgorithm::OneTick(int e) {
    uint64_t rate = GetRandomPacketArrivalRate();
    bool bCantSend = false;
    int x = e - 1;

    Tick[e].StartedAt = time(nullptr);

    Bucket.ul64CurrentTokens += Bucket.ul64TokenRate;

    for (uint64_t i = 0; i < rate; i++) {
        if (i > 0)
            Tick[e].sArrivedPackets.append(",");

        GenRandomArrivedPacket(e);
        if (Queue.ul64FreeSpace > 0) {
            AddPacketToQueue();
        }
#ifdef DEBUG
        get_status();
#endif
        if (!bCantSend) {
            if (!SendPacket()) {
                bCantSend = true;
            }
        }
    }

    Tick[e].ul64BucketTockens = Bucket.ul64CurrentTokens;
    Tick[e].ul64QueuePackets = Queue.ul64CurrentPackets;
    Tick[e].iArrivedPackets = iArrivedPackets - Tick[x].iArrivedPackets;
    Tick[e].iDepartedPackets = iDepartedPackets - Tick[x].iDepartedPackets;
    Tick[e].iDroppedPackets = iDroppedPackets - Tick[x].iDroppedPackets;

    Tick[e].sArrivedPackets = std::to_string(Tick[e].iArrivedPackets) +
        "(" + Tick[e].sArrivedPackets + ")";
}

/**
 * @brief Run the simulation of the algorithm, initialize the startup time
 * and the first tick. Then start a loop of the number of TICKS, in each tick
 * call OneTick() and add the new info to the appropriate tick row of the table.
 */
void TokenBucketAlgorithm::Run() {
    StartupTime = time(nullptr);
    Tick[0] = { StartupTime, 0, 0, "", 0, 0, 0 };

    for (int i = 1; i < TICKS + 1; i++)
        OneTick(i);
}

void TokenBucketAlgorithm::print() {/*
    std::cout << "Tick #\tArrived packets\t\tDeparted packets\t\tDropped packets\t\t" <<
        "Packets in queue\t\tNo. of tokens\r\n";*/
    for (int i = 1; i < TICKS + 1; i++) {
        std::cout << "Tick #\t" << i <<
            "\nArrived packets:\t" << Tick[i].sArrivedPackets <<
            "\nDeparted packets:\t" << Tick[i].iDepartedPackets <<
            "\nDropped packets:\t" << Tick[i].iDroppedPackets <<
            "\nPackets in Queue:\t" << Tick[i].ul64QueuePackets <<
            "\nNo. of tokens:\t" << Tick[i].ul64BucketTockens << std::endl << std::endl;/*
        std::cout << i << "\t\t" << Tick[i].sArrivedPackets <<
            "\t" << Tick[i].iDepartedPackets <<
            "\t" << Tick[i].iDroppedPackets <<
            "\t" << Tick[i].ul64QueuePackets <<
            "\t" << Tick[i].ul64BucketTockens << std::endl;*/
    }
}

TokenBucketAlgorithm::~TokenBucketAlgorithm() {
    struct Packet* ptr;
    if (Queue.ul64CurrentPackets > 0) {
        for (ptr = LastPacketInQueue->next;; ptr = ptr->next) {
            delete ptr->before;
            if (ptr == FirstPacketInQueue)  break;
        }
        delete ptr;
    }
}