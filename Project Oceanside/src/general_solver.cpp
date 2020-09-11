void Heap::Solve()
{
	//Seed the RNG.
	unsigned int seed = (unsigned int)time(NULL);
	srand(seed);

	//Initialize.
	unsigned int totalSolutions = 0;
	uint64_t totalPermutations = 0;
	unsigned int overlayOffsetAddress = 0;
	uint16_t allowableSRMedActorAddressSuffixes[] = { 0x4DC0, 0x4F80, 0x50C0, 0x5200, 0x5400 };
	
	//Draw FPM
	int actorOffset = 0x80;

	//Chest SRM
	//int actorOffset = 0x160;
	
	//Observatory
	/*uint16_t heldActorID = 0x82;
	uint16_t srmedActorID = 0x283;
	int overlayOffset = 0x400;
	unsigned int planeNumber = 0;*/

	//Woods
	int overlayOffset = 0x1A34;
	uint16_t heldActorID = 0x90;
	uint16_t srmedActorID = 0x90;
	unsigned int planeNumber = 0;
	
	unsigned int overlayOffsetMaxAddress = 0x420000;
	unsigned int overlayOffsetMinAddress = 0x400000;
	unsigned int numberOfActorsToSave = 0;
	bool deallocateAllActorsInInitialRoom = false;

	//Declare.
	std::vector<std::pair<int, int>> solution;

	//Print to terminal.
	std::cout << "Seed: " << seed << std::endl;
	std::cout << "Solving..." << std::endl;

	//Create main output folder.
	auto newContainerFolder = "C:\\Users\\rylie\\Documents\\docs\\code\\cpp\\Heap_Manip_Outputs\\";
	_mkdir(newContainerFolder);

	//Create folder for this seed.
	auto newSubFolder = newContainerFolder + std::to_string(seed) + "\\";
	_mkdir(newSubFolder.c_str());


	//Start main loop.
	while (true)
	{
		//Perform necessary initial tasks prior to looping through general steps.

		//Load the initial room.
		LoadInitialRoom(INITIAL_ROOM_NUMBER);
		solution.push_back(std::make_pair(LOAD_INITIAL_ROOM, INITIAL_ROOM_NUMBER));

		//If necessary, destroy everything in the initial room and load the next room.
		if (deallocateAllActorsInInitialRoom)
		{
			//Get the number of deallocatable actors in this room.
			size_t currentRoomDeallocations = currentRoom->GetDeallocatableActors().size();

			//Randomly deallocate actors until they are all deallocated.
			for (int j = 0; j < currentRoomDeallocations; j++)
			{
				solution.push_back(DeallocateRandomActor());
			}

			//After all the deallocations are done, we need to change rooms.

			//Guess that the next room should be room 0.
			int nextRoom = 0;

			//If the guess was wrong, just change it.
			if (currentRoomNumber == 0)
			{
				nextRoom = 1;
			}

			//Actually perform the room change using the chosen room and plane.
			ChangeRoom(nextRoom, planeNumber, false);
			solution.push_back(std::make_pair(CHANGE_ROOM, nextRoom));
			solution.push_back(std::make_pair(SPAWNERS, false));
			solution.push_back(std::make_pair(USE_PLANE, planeNumber));

		}

		//Randomly determine the number of steps to do.
		int roomLoads = ( 2*( (rand() % LOAD_MODIFIER) + 1) - deallocateAllActorsInInitialRoom);

		//Randomly modify the heap each step for the random number of steps.
		for (int i = 0; i < roomLoads; i++)
		{
			//Randomly determine the number of general deallocations to do on this step.
			int deallocations = 0;
			size_t currentRoomDeallocations = currentRoom->GetDeallocatableActors().size();
			if (currentRoomDeallocations)
			{
				deallocations = rand() % (currentRoomDeallocations + 1 - numberOfActorsToSave);
			}

			//Randomly deallocate actors until the random number of general deallocations is reached.
			for (int j = 0; j < deallocations; j++)
			{
				solution.push_back(DeallocateRandomActor());
			}

			//Randomly determine whether to allocate smoke (if it's allowed).
			if (smoke)
			{
				char smokeRNG = rand() % 2;
				if (smokeRNG)
				{
					AllocateTemporaryActor(0xA2);
					solution.push_back(std::make_pair(ALLOCATE, 0xA2));
				}
			}
			//Randomly determine the number of general allocations to do this step.
			int allocations = 0;
			if (MAX_ALLOCATIONS_PER_STEP > 0)
			{
				allocations = rand() % (MAX_ALLOCATIONS_PER_STEP + 1);
			}

			//Randomly allocate actors until the random number of general allocations is reached.
			for (int j = 0; j < allocations; j++)
			{
				solution.push_back(std::make_pair(ALLOCATE, AllocateRandomActor()));
			}

			//Allocate up to one actor that must be carried across the plane.
			if (endAllocationStep)
			{
				char rng = rand() % 3;

				switch (rng)
				{
				case 0:
					break;
				case 1:
					AllocateTemporaryActor(0x35);
					solution.push_back(std::make_pair(ALLOCATE, 0x35));
					break;
				case 2:
					AllocateTemporaryActor(0x3D);
					solution.push_back(std::make_pair(ALLOCATE, 0x3D));
					break;
				default:
					break;
				}
			}

			//after all allocations are done, we need to change rooms.
			
			//Guess that the next room should be room 0.
			int nextRoom = 0;

			//If the guess was wrong, just change it.
			if (currentRoomNumber == 0)
			{
				nextRoom = 1;
			}

			//actually perform room change using chosen room and plane
			ChangeRoom(nextRoom, planeNumber, false);
			solution.push_back(std::make_pair(CHANGE_ROOM, nextRoom));
			solution.push_back(std::make_pair(SPAWNERS, false));
			solution.push_back(std::make_pair(USE_PLANE, planeNumber));
		}

		//Break a random combination of the other held actors before doing the superslide.
		char heldActorBreakRNG = rand() % 7;
		switch (heldActorBreakRNG)
		{
		case 0:
			Deallocate(heldActorID, 0);
			Deallocate(heldActorID, 1);
			solution.push_back(std::make_pair(heldActorID, 0));
			solution.push_back(std::make_pair(heldActorID, 1));
			break;
		case 1:
			Deallocate(heldActorID, 0);
			Deallocate(heldActorID, 2);
			solution.push_back(std::make_pair(heldActorID, 0));
			solution.push_back(std::make_pair(heldActorID, 2));
			break;
		case 2:
			Deallocate(heldActorID, 1);
			Deallocate(heldActorID, 2);
			solution.push_back(std::make_pair(heldActorID, 1));
			solution.push_back(std::make_pair(heldActorID, 2));
			break;
		case 3:
			Deallocate(heldActorID, 0);
			solution.push_back(std::make_pair(heldActorID, 0));
			break;
		case 4:
			Deallocate(heldActorID, 1);
			solution.push_back(std::make_pair(heldActorID, 1));
			break;
		case 5:
			Deallocate(heldActorID, 2);
			solution.push_back(std::make_pair(heldActorID, 2));
			break;
		case 6:
			break;
		default:
			break;
		}

		//Find all superslidable actors in room 1.
		std::vector<std::pair<int, int>> superslidableActors = GetAddressesAndPrioritiesOfType(heldActorID, 'A');

		//Superslide into room 0 to get a stale reference.
		AllocateTemporaryActor(0xA2);
		ChangeRoom(0, planeNumber, false);
		solution.push_back(std::make_pair(SUPERSLIDE, 0));

		//Return to room 1 with the stale reference.
		ChangeRoom(1, planeNumber, false);
		solution.push_back(std::make_pair(CHANGE_ROOM, 1));
		solution.push_back(std::make_pair(SPAWNERS, false));
		solution.push_back(std::make_pair(USE_PLANE, planeNumber));

		//Find the SRMable actors in room 1.
		std::vector<std::pair<int, int>> srmableActors = GetAddressesAndPrioritiesOfType(srmedActorID, 'A');

		//Initialize.
		bool solutionFound = false;
		int instanceType = srmedActorID;
		int overlayAddress = GetOverlayAddress(srmedActorID);

		//Declare.
		std::pair<std::pair<int, int>, std::pair<int, int>> solutionPair;

		//Check for all the superslidable and SRMable actors to see if any meet the conditions.
		for (auto heldActor : superslidableActors)
		{
			for (auto srmedActor : srmableActors)
			{
				//if (std::any_of(std::begin(allowableSRMedActorAddressSuffixes), std::end(allowableSRMedActorAddressSuffixes), [srmedActor](int suffix) {return srmedActor.first % 0x10000 == suffix;}))
				//{

					//Print info to the screen if a partial solution was found.
					//std::cout << std::hex << "SRMed actor = " << srmedActor.first << "\n";
					//std::cout << std::hex << "Held actor = " << heldActor.first << "\n";
					//std::cout << std::hex << "Overlay = " << overlayAddress << "\n\n";

					if (heldActor.first - srmedActor.first == actorOffset)
					{
						overlayOffsetAddress = overlayAddress + overlayOffset;
						if (overlayOffsetAddress < overlayOffsetMaxAddress && overlayOffsetAddress >= overlayOffsetMinAddress )
						{
							solutionFound = true;
							solutionPair = std::make_pair(heldActor, srmedActor);
						}
					}
				//}
			}

		}


		//Create an output file if a solution is found.
		if (solutionFound)
		{
			std::cout << "SOLUTION FOUND\n";
			totalSolutions++;

			std::ofstream outputFile;
			std::string outputFilename = newSubFolder + "\\solution" + std::to_string(totalSolutions) + "_seed_" + std::to_string(seed) + ".txt";
			outputFile.open(outputFilename);

			outputFile << std::hex << "Held Actor: Address = " << solutionPair.first.first << " | Priority = " << solutionPair.first.second << std::endl <<
				"SRMed Actor Instance: ID = " << instanceType << " | Address = " << solutionPair.second.first << " | Priority = " << solutionPair.second.second << std::endl <<
				"SRMed Actor Overlay: Address = " << overlayAddress << " | Address of Offset = " << overlayOffsetAddress << std::endl;

			for (auto step : solution)
			{
				if (step.first == LOAD_INITIAL_ROOM)
				{
					outputFile << std::hex << "Load initial room: " << step.second << std::endl;
				}
				else if (step.first == CHANGE_ROOM)
				{
					outputFile << std::hex << "Load room: " << step.second;
				}
				else if (step.first == SPAWNERS)
				{
					outputFile << " | Spawners: " << step.second;
				}
				else if (step.first == USE_PLANE)
				{
					outputFile << " | Use plane: " << step.second << std::endl;
				}
				else if (step.first == ALLOCATE)
				{
					if (step.second != 0x0)
					{
						outputFile << std::hex << "Allocate: " << step.second << std::endl;
					}
				}
				else if (step.first == SUPERSLIDE)
				{
					outputFile << std::hex << "Superslide into room " << step.second << " with smoke still loaded using plane " << planeNumber <<"." << std::endl;
				}
				else if (step.first == 0)
				{
					;
				}
				else
				{
					outputFile << std::hex << "Dealloc: " << std::setw(2) << step.first << ", " << step.second << std::endl;
				}

			}
			outputFile.close();
		}


		//Cleanup from this iteration of the main loop.
		ResetHeap();
		solution.clear();
		totalPermutations++;

		//Print a message to the screen every so often to let the user know how it's doing.
		if (totalPermutations % 100000 == 0)
		{
			std::cout << std::dec << "Total permutations: " << totalPermutations << " | Total Solutions: " << totalSolutions << std::endl;
		}

	}
}