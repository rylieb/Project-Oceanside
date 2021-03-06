#pragma once

#include "Scene.h"
#include "Node.h"

class Heap
{
public:
	Heap(Scene* scene, int start, int linkSize);
	~Heap();
	void Allocate(Node* node);
	Node* AllocateTemporaryActor(int actorID);
	void Deallocate(int actorID, int priority);
	void Deallocate(Node* node);
	void DeallocateTemporaryActor(int actorID);
	void LoadInitialRoom(int roomNumber);
	void UnloadRoom(Room& room, int transitionActorSceneID, Node* carryActor);
	void ChangeRoom(int newRoomNumber, int transitionActorSceneID, Node* carryActor, bool spawners);
	void PrintHeap(char setting) const;
	void DeleteHeap();
	Node* FindSuitableGap(Node* newNode) const;
	void Insert(Node* newNode, Node* oldNode);
	Node* GetHead() const;
	Node* GetTail() const;
	void PrintCurrentActorCount() const;
	void ClearTemporaryActors();
	void ResetHeap();
	std::pair<int, int> DeallocateRandomActor();
	int AllocateRandomActor();
	std::pair<int, int> ClearRandomActor();
	int GetRoomNumber() const;
	void Solve();
	int GetCurrentRoomNumber() const;
	void ResetLeaks();
	std::vector<std::pair<int, int>> GetAddressesAndPrioritiesOfType(int actorID, char type);
	std::vector<std::pair<int, int>> GetAllAddresses(char type);
	int GetOverlayAddress(int actorID);
	void SolveObservatory();
	void SolveGrave();
	void SolveGraveyard();
	std::vector<Node*> GetAllActorsOfID(int actorID);
	
private:
	void AllocateNewRoom(Room& newRoom, Room& oldRoom, int transitionActorSceneID);
	void DeallocateClearedActors();
	void AllocateSpawnerOffspring();
	void DeallocateReallocatingActors();

	Scene* scene = nullptr;
	Node* head = nullptr;
	Node* tail = nullptr;
	int currentRoomNumber = 0;
	int initiallyLoadedRoomNumber = 0;
	const int start_address = 0x0;
	const int END_ADDRESS = 0x5fffff;
	const int linkSize = 0x0;
	const int LINK_ID = 0xffff;
	const char LINK_TYPE = 'L';
	const char OVERLAY_TYPE = 'O';

	const int MAX_ALLOCATIONS_PER_STEP = 9;
	const int LOAD_MODIFIER = 4;

	const int MAX_EXPLOSIVES_PER_ROOM = 3;
	const int MAX_ARROWS_PER_ROOM = 1;
	const int MAX_CHUS = 0;
	const int MAX_ISOT = 0;
	const int MAX_BOMBS = 6;

	const bool smoke = true;
	const bool endAllocationStep = true;
	const bool postSSRoomChange = false;
	const bool breakRocks = false;
	const bool fins = false;
	const bool nut = false;
	const bool madScrub = false;

	bool initialLoad = true;

	Room* currentRoom;

	std::map<int, int> currentActorCount;
	std::vector<Node*> temporaryActors;
	std::vector<Node*> offspringToAllocate;

	std::vector<Node*> leaks;
	std::vector<Node*> singletons;

	std::vector<Node*> singletonsAttemptingToReallocate;
	std::vector<Node*> reallocatingTransitionActors;

	Node* carryActor = nullptr;

	int allocatedExplosiveCount;
	int allocatedChuCount;
	int allocatedISoTCount;
	int allocatedBombCount;
	int allocatedArrowCount;

	std::pair<Node*, int> heldActor;
};

