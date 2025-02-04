#include <iostream>
#include <stdexcept>
#include <windows.h>

using namespace std;

struct RoomNode
{
    int roomID;
    int height;
    string roomType;
    string roomStatus;
    RoomNode *left;
    RoomNode *right;

    RoomNode(int roomid, string roomtype, string roomstatus) : roomID(roomid), roomType(roomtype), roomStatus(roomstatus), left(NULL), right(NULL), height(0) {}
};

struct BookingNode
{
    int stayDuration;
    int priorityID;
    string customerName;
    string roomType;
    BookingNode *next;
};

struct BookingHistory
{
    int stayDuration;
    int roomID;
    string roomType;
    string customerName;
    BookingHistory *next;
};

class Room
{
    RoomNode *root;
    int size;

public:
    Room() : root(NULL), size(0) {}

    int getHeight(RoomNode *tree)
    {
        if (tree == NULL)
        {
            return 0;
        }
        else
        {
            return tree->height;
        }
    }

    int getBalance(RoomNode *tree)
    {
        return getHeight(tree->left) - getHeight(tree->right);
    }

    RoomNode *rightRotation(RoomNode *tree) // Corrected: Right Rotation
    {
        RoomNode *child = tree->left;
        RoomNode *childRight = child->right;

        child->right = tree;
        tree->left = childRight;

        tree->height = 1 + max(getHeight(tree->left), getHeight(tree->right));
        child->height = 1 + max(getHeight(child->left), getHeight(child->right));

        return child;
    }

    RoomNode *leftRotation(RoomNode *tree) // Corrected: Left Rotation
    {
        RoomNode *child = tree->right;
        RoomNode *childLeft = child->left;

        child->left = tree;
        tree->right = childLeft;

        tree->height = 1 + max(getHeight(tree->left), getHeight(tree->right));
        child->height = 1 + max(getHeight(child->left), getHeight(child->right));

        return child;
    }

    RoomNode *insert(RoomNode *tree, int roomid, string roomtype, string roomstatus)
    {
        if (tree == NULL)
        {
            return new RoomNode(roomid, roomtype, roomstatus);
        }

        if (roomid < tree->roomID)
        {
            tree->left = insert(tree->left, roomid, roomtype, roomstatus);
        }
        else if (roomid > tree->roomID)
        {
            tree->right = insert(tree->right, roomid, roomtype, roomstatus);
        }
        else
        {
            return tree; // Room already exists
        }

        tree->height = 1 + max(getHeight(tree->left), getHeight(tree->right));

        int balance = getBalance(tree);

        if (balance > 1 && roomid < tree->left->roomID) // LL Case
        {
            return rightRotation(tree);
        }
        if (balance > 1 && roomid > tree->left->roomID) // LR Case
        {
            tree->left = leftRotation(tree->left);
            return rightRotation(tree);
        }
        if (balance < -1 && roomid > tree->right->roomID) // RR Case
        {
            return leftRotation(tree);
        }
        if (balance < -1 && roomid < tree->right->roomID) // RL Case
        {
            tree->right = rightRotation(tree->right);
            return leftRotation(tree);
        }

        return tree;
    }

    void addRooms(int floorStartingID, int numberOfRooms)
    {
        if (floorStartingID <= 0 || numberOfRooms <= 0)
        {
            cout << "\nInvalid Room Details.\n";
            return;
        }

        for (int i = 0; i < numberOfRooms; i++)
        {
            int roomID = floorStartingID + i;
            string roomType = assignRoomType(i);
            root = insert(root, roomID, roomType, "Ready");
        }
    }
    int getSize() const
    {
        return size;
    }

    bool isValidRoomID(RoomNode *node, int roomID) const
    {
        if (!node)
            return false;

        if (node->roomID == roomID)
            return true;

        if (roomID < node->roomID)
            return isValidRoomID(node->left, roomID);
        else
            return isValidRoomID(node->right, roomID);
    }

    bool isValidRoomID(int roomID) const
    {
        return isValidRoomID(root, roomID);
    }

    void countRoomStatuses(RoomNode *node, int &total, int &booked, int &available, int &unavailable, int &occupied) const
    {
        if (!node)
            return;

        total++;
        if (node->roomStatus == "Booked")
            booked++;
        else if (node->roomStatus == "Ready")
            available++;
        else if (node->roomStatus == "Unavailable")
            unavailable++;
        else if (node->roomStatus == "Occupied")
            occupied++;

        countRoomStatuses(node->left, total, booked, available, unavailable, occupied);
        countRoomStatuses(node->right, total, booked, available, unavailable, occupied);
    }

    void countRoomStatuses(int &total, int &booked, int &available, int &unavailable, int &occupied) const
    {
        total = booked = available = unavailable = occupied = 0;
        countRoomStatuses(root, total, booked, available, unavailable, occupied);
    }

    string assignRoomType(int index)
    {
        if (index % 3 == 0)
            return "Single";
        else if (index % 3 == 1)
            return "Double";
        return "Suite";
    }

    void inOrder(RoomNode *node) const
    {
        if (node)
        {
            inOrder(node->left);
            cout << "Room ID: " << node->roomID << "\t| Room Type: " << node->roomType << "\t| Room Status: " << node->roomStatus << endl;
            inOrder(node->right);
        }
    }

    void displayRooms() const
    {
        if (!root)
        {
            cout << "No rooms available." << endl;
            return;
        }

        cout << "\nRooms List:\n";
        inOrder(root);
    }

    RoomNode *findAvailableRoom(RoomNode *node, const string &type) const
    {
        if (!node)
            return NULL;

        if (node->roomType == type && node->roomStatus == "Ready")
            return node;

        RoomNode *leftResult = findAvailableRoom(node->left, type);
        return leftResult ? leftResult : findAvailableRoom(node->right, type);
    }

    int findRoomAndAssign(const string &type)
    {
        RoomNode *availableRoom = findAvailableRoom(root, type);
        if (availableRoom)
        {
            availableRoom->roomStatus = "Booked";
            return availableRoom->roomID;
        }

        cout << "No available rooms of type '" << type << "' found.\n";
        return -1;
    }

    void updateRoomStatus(int roomID, const string &newStatus)
    {
        RoomNode *room = findRoomById(root, roomID);
        if (room)
        {
            room->roomStatus = newStatus;
            cout << "Room ID: " << roomID << " status updated to " << newStatus << endl;
        }
        else
        {
            cout << "Room ID: " << roomID << " not found." << endl;
        }
    }

    RoomNode *findRoomById(RoomNode *node, int roomID) const
    {
        if (!node || node->roomID == roomID)
            return node;

        return roomID < node->roomID ? findRoomById(node->left, roomID) : findRoomById(node->right, roomID);
    }

    bool checkAvailability(RoomNode *node, const string &type) const
    {
        if (!node)
            return false;

        return (node->roomType == type && node->roomStatus == "Ready") ||
               checkAvailability(node->left, type) ||
               checkAvailability(node->right, type);
    }

    bool checkAvailabilityofRoom(const string &type) const
    {
        if (!root)
        {
            cout << "No Rooms Available!\n";
            return false;
        }

        bool available = checkAvailability(root, type);
        if (!available)
        {
            cout << "No available rooms of type '" << type << "' found.\n";
        }
        return available;
    }

    void findRoomId(RoomNode *node, const string &type) const
    {
        if (!node)
            return;

        findRoomId(node->left, type);

        if (node->roomType == type && node->roomStatus == "Ready")
        {
            cout << "Room ID: " << node->roomID << endl;
        }

        findRoomId(node->right, type);
    }

    void findAvaialbleRoomID(const string &roomtype)
    {
        findRoomId(root, roomtype);
    }
};
struct FloorNode
{
    int floorID;
    Room room;
    FloorNode *left;
    FloorNode *right;
    int height;

    FloorNode(int floorid) : floorID(floorid), left(NULL), right(NULL), height(0) {}
};

class Floor
{
    FloorNode *root;
    int size;

public:
    Floor() : root(NULL), size(0) {}

    int getheight(FloorNode *tree)
    {
        if (tree == NULL)
        {
            return 0;
        }
        else
        {
            return tree->height;
        }
    }

    int getBalance(FloorNode *tree)
    {
        return getheight(tree->left) - getheight(tree->right);
    }

    FloorNode *rightRotation(FloorNode *tree) // For Left Rotation
    {
        FloorNode *child = tree->left;
        FloorNode *childRight = child->right;

        child->right = tree;
        tree->left = childRight;

        tree->height = 1 + max(getheight(tree->left), getheight(tree->right));
        child->height = 1 + max(getheight(child->left), getheight(child->right));

        return child;
    }

    FloorNode *leftRotation(FloorNode *tree) // For Right Rotation
    {
        FloorNode *child = tree->right;
        FloorNode *childLeft = child->left;

        child->left = tree;
        tree->right = childLeft;

        tree->height = 1 + max(getheight(tree->left), getheight(tree->right));
        child->height = 1 + max(getheight(child->left), getheight(child->right));

        return child;
    }

    FloorNode *insert(FloorNode *tree, int floorid)
    {
        if (tree == NULL)
        {
            FloorNode *node = new FloorNode(floorid);
            return node;
        }
        else if (floorid < tree->floorID)
        {
            tree->left = insert(tree->left, floorid);
        }
        else if (floorid > tree->floorID)
        {
            tree->right = insert(tree->right, floorid);
        }
        else
        {
            return tree;
        }

        tree->height = 1 + max(getheight(tree->left), getheight(tree->right));

        int balance = getBalance(tree);

        // Rotation 4 Cases

        if (balance > 1 && floorid < tree->left->floorID) // LL Case
        {
            return rightRotation(tree);
        }
        else if (balance > 1 && floorid > tree->left->floorID) // LR Case
        {
            tree->left = leftRotation(tree->left);
            return rightRotation(tree);
        }
        else if (balance < -1 && floorid > tree->right->floorID) // RR Case
        {
            return leftRotation(tree);
        }
        else if (balance < -1 && floorid < tree->right->floorID) // RL Case
        {
            tree->right = rightRotation(tree->right);
            return leftRotation(tree);
        }
        else
        {
            return tree;
        }
    }

    void insertFloors(int numberOfFloors)
    {
        if (numberOfFloors <= 0)
        {
            cout << "\nInvalid Number of Floors";
            return;
        }

        for (int i = 1; i <= numberOfFloors; ++i)
        {
            root = insert(root, i);
            size++;
        }
    }

    void addRoomsToFloors(FloorNode *node, int numberOfRooms)
    {
        if (node == NULL)
        {
            return;
        }

        int startingID = (node->floorID) * 100 + 1;
        node->room.addRooms(startingID, numberOfRooms);

        addRoomsToFloors(node->left, numberOfRooms);
        addRoomsToFloors(node->right, numberOfRooms);
    }

    void addRoomsToFloors(int numberOfRooms)
    {
        addRoomsToFloors(root, numberOfRooms);
    }

    void displayFloors(FloorNode *node)
    {
        if (node == NULL)
        {
            return;
        }

        displayFloors(node->left);

        cout << "Floor ID : " << node->floorID << endl;
        cout << "************";

        node->room.displayRooms();

        displayFloors(node->right);
    }

    void displayFloors()
    {
        if (!root)
        {
            cout << "\nNo Floors Available ";
            return;
        }

        displayFloors(root);
        cout << "Total Number of Floors: " << size << endl;
        cout << "Total Number of Rooms: " << getTotalRooms(root) << endl;
    }

    void showAvailableRoomsByType(FloorNode *node, const string &roomType)
    {
        if (node == NULL)
        {
            return;
        }

        showAvailableRoomsByType(node->left, roomType);

        cout << "\nFloor ID : " << node->floorID << endl;
        cout << "Checking Availability of " << roomType << " Rooms on Floor " << node->floorID << endl;
        node->room.findAvaialbleRoomID(roomType);

        showAvailableRoomsByType(node->right, roomType);
    }

    void showAvailableRoomsByType(const string &roomType)
    {
        if (root == NULL)
        {
            cout << "\nNo Available Floors";
            return;
        }

        showAvailableRoomsByType(root, roomType);
    }

    void updateStatusByID(int roomID, const string &newStatus)
    {
        if (!root)
        {
            std::cout << "No Floors Available\n";
            return;
        }

        updateStatusByID(root, roomID, newStatus);
    }

    FloorNode *findFloor(FloorNode *node, int floorID)
    {
        if (!node || node->floorID == floorID)
            return node;
        if (floorID < node->floorID)
            return findFloor(node->left, floorID);
        return findFloor(node->right, floorID);
    }

    FloorNode *findFloor(int floorID)
    {
        return findFloor(root, floorID);
    }

    void updateStatusByID(FloorNode *node, int roomID, const string &newStatus)
    {
        if (node == NULL)
            return;

        // Traverse left subtree
        updateStatusByID(node->left, roomID, newStatus);

        // Check the current floor's room manager
        node->room.updateRoomStatus(roomID, newStatus);

        // Traverse right subtree
        updateStatusByID(node->right, roomID, newStatus);
    }

    int assignRoom(FloorNode *node, const string &roomType)
    {
        if (!node)
            return -1;

        // Check the current floor for an available room
        int roomId = node->room.findRoomAndAssign(roomType);
        if (roomId != -1)
            return roomId;

        // Recursively check left and right subtrees
        int leftResult = assignRoom(node->left, roomType);
        if (leftResult != -1)
            return leftResult;

        return assignRoom(node->right, roomType);
    }

    int assignRoom(string &roomType)
    {
        if (root == NULL)
        {
            cout << "No floors available.\n";
            return -1;
        }

        int roomId = assignRoom(root, roomType);
        if (roomId == -1)
        {
            cout << "No rooms of type '" << roomType << "' available on any floor.\n";
        }

        return roomId;
    }
    int getRoomsPerFloor(int floorID)
    {
        FloorNode *floor = findFloor(floorID);
        return floor ? floor->room.getSize() : 0;
    }

    bool checkAvailabilityOfRooms(int floorID, const string &roomType)
    {
        FloorNode *floor = findFloor(floorID);
        if (!floor)
            return false;

        int total, booked, available, unavailable, occupied;
        floor->room.countRoomStatuses(total, booked, available, unavailable, occupied);
        return available > 0;
    }

    FloorNode *getRootOfFloor()
    {
        return root;
    }

    // Helper function to check room availability in the AVL tree of floors
    bool checkAvailabilityOfRooms(FloorNode *node, const string &roomType) const
    {
        if (!node)
        {
            return false;
        }

        // Check the current floor for room availability
        if (node->room.checkAvailabilityofRoom(roomType))
        {
            return true;
        }

        // Recursively check left and right subtrees
        return checkAvailabilityOfRooms(node->left, roomType) || checkAvailabilityOfRooms(node->right, roomType);
    }

    // Public interface to check room availability
    bool checkAvailabilityOfRooms(const string &roomType) const
    {
        if (root == NULL)
        {
            cout << "No floors available.\n";
            return false;
        }

        return checkAvailabilityOfRooms(root, roomType);
    }

    int getTotalRooms(FloorNode *node)
    {
        if (!node)
        {
            return 0; // Base case: no floors
        }

        // Get the number of rooms in the current floor and add to the sum of subtrees
        return node->room.getSize() + getTotalRooms(node->left) + getTotalRooms(node->right);
    }

    bool isValidRoomID(FloorNode *node, int roomID) const
    {
        if (!node)
            return false;

        if (node->room.isValidRoomID(roomID))
            return true;

        return isValidRoomID(node->left, roomID) || isValidRoomID(node->right, roomID);
    }

    bool isValidRoomID(int roomID) const
    {
        return isValidRoomID(root, roomID);
    }
    int getRoomsPerFloor() const
    {
        if (!root)
        {
            return 0; // No floors exist
        }
        return root->room.getSize(); // Return the number of rooms in the first floor
    }
    

    void appendFloorsAndRooms(int newFloors)
    {
        if (!root)
        {
            cout << "Error: No existing floors found. Use 'Add Initial Floors & Rooms' first.\n";
            return;
        }

        // Count existing floors
        int existingFloors = getTreeSize(root);
        int roomsPerFloor = getRoomsPerFloor(); // Use the new getRoomsPerFloor function

        if (roomsPerFloor == 0)
        {
            cout << "Error: Unable to determine rooms per floor.\n";
            return;
        }

        // Ensure total floors do not exceed 30
        int maxNewFloors = 30 - existingFloors;
        if (newFloors <= 0 || newFloors > maxNewFloors)
        {
            cout << "Cannot add " << newFloors << " floors. Maximum limit of 30 floors reached.\n";
            return;
        }

        // Insert new floors maintaining AVL balance
        for (int i = existingFloors + 1; i <= existingFloors + newFloors; i++)
        {
            root = insert(root, i);
            findFloor(root, i)->room.addRooms(i * 100 + 1, roomsPerFloor);
        }

        cout << "Successfully added " << newFloors << " floors with " << roomsPerFloor << " rooms each.\n";
    }

    void countRoomStatuses(FloorNode *node, int &total, int &booked, int &available, int &unavailable, int &occupied) const
    {
        if (!node)
            return;

        int floorTotal = 0, floorBooked = 0, floorAvailable = 0, floorUnavailable = 0, floorOccupied = 0;
        node->room.countRoomStatuses(floorTotal, floorBooked, floorAvailable, floorUnavailable, floorOccupied);

        total += floorTotal;
        booked += floorBooked;
        available += floorAvailable;
        unavailable += floorUnavailable;
        occupied += floorOccupied;

        countRoomStatuses(node->left, total, booked, available, unavailable, occupied);
        countRoomStatuses(node->right, total, booked, available, unavailable, occupied);
    }

    int getTreeSize(FloorNode *node) const
    {
        if (!node)
            return 0;
        return 1 + getTreeSize(node->left) + getTreeSize(node->right);
    }

    void displayRoomStatusReport() const
    {
        if (!root)
        {
            cout << "No floors available.\n";
            return;
        }

        int totalRooms = 0, bookedRooms = 0, availableRooms = 0, unavailableRooms = 0, occupiedRooms = 0;
        countRoomStatuses(root, totalRooms, bookedRooms, availableRooms, unavailableRooms, occupiedRooms);

        cout << "\n************ Room Status Report ************\n";
        cout << "Total Rooms        : " << totalRooms << endl;
        cout << "Booked Rooms       : " << bookedRooms << endl;
        cout << "Available Rooms    : " << availableRooms << endl;
        cout << "Unavailable Rooms  : " << unavailableRooms << endl;
        cout << "Occupied Rooms     : " << occupiedRooms << endl;
        cout << "*******************************************\n";
    }
};

class BookingRoomHistory
{
    BookingHistory *top;
    int size;

public:
    BookingRoomHistory() : top(NULL), size(0) {}

    void storeBooking(string customerName, int stayDuration, int roomID, string roomType)
    {
        BookingHistory *newnode = new BookingHistory;
        newnode->customerName = customerName;
        newnode->stayDuration = stayDuration;
        newnode->roomID = roomID;
        newnode->roomType = roomType;
        newnode->next = top;

        top = newnode;
        size++;
        cout << "Booking Stored for " << customerName << "\n";
    }

    void cancelBooking(string customername, Floor &floor)
    {
        if (top == NULL)
        {
            cout << "No Bookings Histories Yet ";
            return;
        }

        cout << "Booking History of " << top->customerName << " is removed ";
        BookingHistory *temp = top;
        floor.updateStatusByID(top->roomID, "Ready");
        top = top->next;
        delete temp;
        size--;
    }

    void displayHistories()
    {
        if (!top)
        {
            cout << "No Booking Histories Yet\n";
            return;
        }

        BookingHistory *temp = top;
        cout << "Booking Histories:\n";
        while (temp)
        {
            cout << "Customer Name: " << temp->customerName << " | Room ID: " << temp->roomID
                 << " | Room Type: " << temp->roomType << " | Stay Duration: " << temp->stayDuration
                 << " Nights\n";
            temp = temp->next;
        }
        cout << "\n**********************************************\n";
    }

    void checkReservation(string customerName, string roomType, BookingNode *bookingQueueHead)
    {
        bool found = false;

        if (top == NULL)
        {
            cout << "No booking history yet.\n";
        }
        else
        {
            BookingHistory *temp = top;
            while (temp)
            {
                if (temp->customerName == customerName && temp->roomType == roomType)
                {
                    cout << "Found in booking history:\n";
                    cout << "Customer Name: " << temp->customerName
                         << " | Room ID: " << temp->roomID
                         << " | Room Type: " << temp->roomType
                         << " | Stay Duration: " << temp->stayDuration << " Nights\n";
                    found = true;
                    break; // Break after finding the first match
                }
                temp = temp->next;
            }
        }

        // Check in the booking queue (linked list) if not found in the history
        if (!found)
        {
            bool foundInQueue = false;
            BookingNode *temp = bookingQueueHead;

            while (temp)
            {
                if (temp->customerName == customerName && temp->roomType == roomType)
                {
                    if (!foundInQueue)
                    {
                        cout << "Found in booking queue:\n";
                    }
                    cout << "Customer Name: " << temp->customerName
                         << " | Room Type: " << temp->roomType
                         << " | Stay Duration: " << temp->stayDuration << " Nights"
                         << " | Priority ID: " << temp->priorityID << " (Pending)\n";
                    foundInQueue = true;
                }
                temp = temp->next;
            }

            if (!foundInQueue)
            {
                cout << "No reservations found for \"" << customerName << "\" with room type \"" << roomType << "\".\n";
            }
        }
    }

    BookingHistory *getTop()
    {
        return top;
    }
    ~BookingRoomHistory()
    {
        while (top)
        {
            BookingHistory *temp = top;
            top = top->next;
            delete temp;
        }
    }
};

class BookingQueue
{
    BookingNode *bookHead;
    BookingNode *bookTail;
    int numberOfRequests;

public:
    BookingQueue() : bookHead(NULL), bookTail(NULL), numberOfRequests(0) {}

    BookingNode *getHead() const
    {
        return bookHead;
    }
    void sendBookingRequest(int priorityID, string customerName, int stayDuration, string roomType, Floor &floor, BookingRoomHistory &bookingHistory)
    {
        if (numberOfRequests == 10)
        {
            processRequests(floor, bookingHistory);
        }
        else
        {
            if (stayDuration > 30 || stayDuration <= 0)
            {
                cout << "\nInvalid Stay Duration Period ";
                return;
            }

            BookingNode *newnode = new BookingNode;
            newnode->customerName = customerName;
            newnode->roomType = roomType;
            newnode->stayDuration = stayDuration;
            newnode->priorityID = priorityID;
            newnode->next = NULL;

            if (bookHead == NULL)
            {
                bookHead = bookTail = newnode;
            }
            else if (priorityID == 1)
            {
                if (!bookHead || bookHead->priorityID != 1) // very dangerous case!!!
                {
                    newnode->next = bookHead;
                    bookHead = newnode;
                }
                else
                {
                    BookingNode *current = bookHead;
                    while (current->next && current->next->priorityID == 1)
                    {
                        current = current->next;
                    }
                    newnode->next = current->next;
                    current->next = newnode;
                }
            }
            else
            {
                bookTail->next = newnode;
                bookTail = newnode;
            }
            numberOfRequests++;
        }
    }

    void displayRequest()
    {
        if (!bookHead)
        {
            cout << "\nNo Booking Requests is Recieved yet \n";
            return;
        }

        cout << "\nTotal Number of Requests Pending: " << numberOfRequests << endl;
        BookingNode *temp = bookHead;
        while (temp)
        {
            cout << "Customer Name: " << temp->customerName
                 << " | Priority ID: " << temp->priorityID
                 << " | Request for Room Type: " << temp->roomType
                 << " | Stay Duration: " << temp->stayDuration << " Nights\n";
            temp = temp->next;
        }
        cout << "\n**********************************************\n";
    }

    void processRequests(Floor &floor, BookingRoomHistory &bookingHistory)
    {
        if (!bookHead)
        {
            cout << "No Booking Requests to Process\n";
            return;
        }

        // Process all requests in the queue
        while (bookHead)
        {
            BookingNode *current = bookHead;

            // Check availability of the requested room type across all floors
            int assignedRoomID = floor.assignRoom(current->roomType);

            if (assignedRoomID != -1)
            {
                // If a room is available, store the booking in history
                bookingHistory.storeBooking(current->customerName, current->stayDuration,
                                            assignedRoomID, current->roomType);
                cout << "Booking Successful for " << current->customerName
                     << ", Room ID: " << assignedRoomID << "\n";
            }
            else
            {
                // If no rooms are available, display an error message
                cout << "Booking Failed for " << current->customerName
                     << ": No Available Rooms of type '" << current->roomType << "' on any floor.\n";
            }

            // Remove the processed request from the queue and push into stack
            bookHead = bookHead->next;
            delete current;
            numberOfRequests--;
        }

        cout << "All booking requests have been processed.\n";
    }

    // void cancelBookingRequest(string customerName)
    // {
    //     if (!bookHead)
    //     {
    //         cout << "No booking requests to cancel.\n";
    //         return;
    //     }

    //     BookingNode *current = bookHead;
    //     BookingNode *prev = NULL;

    //     while (current)
    //     {
    //         if (current->customerName == customerName)
    //         {
    //             if (prev)
    //             {
    //                 prev->next = current->next;
    //             }
    //             else
    //             {
    //                 bookHead = current->next;
    //             }

    //             if (current == bookTail)
    //             {
    //                 bookTail = prev;
    //             }

    //             delete current;
    //             numberOfRequests--;
    //             cout << "Booking request for \"" << customerName << "\" has been canceled.\n";
    //             return;
    //         }

    //         prev = current;
    //         current = current->next;
    //     }

    //     cout << "No matching booking request found for customer \"" << customerName << "\".\n";
    // }

    ~BookingQueue()
    {
        while (bookHead)
        {
            BookingNode *temp = bookHead;
            bookHead = bookHead->next;
            delete temp;
        }
        bookTail = NULL;
    }
};

// This is class only for Exception Handling Cases

class InvalidInputException : public runtime_error
{
public:
    InvalidInputException() : runtime_error("Invalid input. Please enter a valid input.") {}
};

void getChoice(int &choice)
{
    string input;
    cin >> input;

    try
    {
        size_t pos;
        choice = stoi(input, &pos);

        // Ensure the entire input is valid
        if (pos != input.size())
        {
            throw InvalidInputException();
        }
    }
    catch (const invalid_argument &)
    {
        throw InvalidInputException();
    }
    catch (const out_of_range &)
    {
        throw InvalidInputException();
    }

    // cin.fail() if this will be called -> infinite loop
    // to avoid it we -> cin.clear()
}

void clearInputBuffer()
{
    cin.clear();              // Clear the error state of the cin object
    while (cin.get() != '\n') // Discard characters until a newline
        ;
}

void handleChoiceInput(int &choice)
{
    bool validInput = false;
    while (!validInput)
    {
        try
        {
            getChoice(choice); // Get the choice from user input
            validInput = true; // If no exception, input is valid
        }
        catch (const InvalidInputException &e)
        {
            cout << e.what() << endl;
            clearInputBuffer(); // Clear the invalid input
            cout << "Please Enter again : ";
        }
    }
}

class User
{
public:
    // Send Booking Request to Enqeue into Booking Queue
    void sendBookingRequest(BookingQueue &bookingQueue, Floor &floor, BookingRoomHistory &bookingHistory)
    {
        string name, roomType;
        cout << "Please enter your name: ";
        cin.ignore();
        getline(cin, name);

        int priority;
        cout << "Enter priority (1 for High, 2 for Standard): ";
        handleChoiceInput(priority); // Use handleChoiceInput to validate the priority input

        while (priority != 1 && priority != 2)
        {
            cout << "Invalid Priority. Please try again.\n";
            cout << "Enter priority (1 for High, 2 for Standard): ";
            handleChoiceInput(priority); // Re-prompt for priority using handleChoiceInput
        }

        int stayDuration;
        cout << "Enter stay duration (1-30): ";
        do
        {
            handleChoiceInput(stayDuration);
            if (stayDuration < 1 || stayDuration > 30)
            {
                cout << "Invalid duration. Please try again (1-30): ";
            }
        } while (stayDuration < 1 || stayDuration > 30);

        bool found = false;
        while (!found)
        {
            int choice = 0;
            do
            {
                cout << "1. Single Bed-Rooms\n2. Double Bed-Rooms\n3. Suite Bed-Rooms\nChoice: ";
                handleChoiceInput(choice); // Validate the room type choice

                switch (choice)
                {
                case 1:
                    roomType = "Single";
                    break;
                case 2:
                    roomType = "Double";
                    break;
                case 3:
                    roomType = "Suite";
                    break;
                default:
                    cout << "Invalid choice. Try again.\n";
                    break;
                }

            } while (choice <= 0 || choice > 3); // Repeat if the choice is invalid

            found = floor.checkAvailabilityOfRooms(roomType); // Check all floors for availability
            if (!found)
            {
                cout << "Sorry, this type of room is already booked. Please choose another room type.\n";
            }
        }

        bookingQueue.sendBookingRequest(priority, name, stayDuration, roomType, floor, bookingHistory);

        cout << "Dear Customer " << name << ", Request Successfully Received for " << roomType
             << " Bed Room for " << stayDuration << " nights " << ((priority == 1) ? " | VIP | " : "")
             << endl;
    }

    void processBookingRequests(BookingQueue &bookingQueue, Floor &floor, BookingRoomHistory &bookingHistory)
    {
        bookingQueue.processRequests(floor, bookingHistory);
    }

    // Display Booking Requests stored into Queue

    void displayBookingRequests(BookingQueue &bookingQueue)
    {
        bookingQueue.displayRequest();
    }
    void checkReservationByEmployee(BookingNode *bookingQueue, BookingRoomHistory &bookingHistory)
    {
        checkReservation(bookingQueue, bookingHistory);
    }

    void countRoomsByStatus(Floor &floor)
    {
        floor.displayRoomStatusReport();
    }

    void addFloors(Floor &floor)
    {
        const int MAX_FLOORS = 30; // Configurable maximum floors
        const int MIN_FLOORS = 3;  // Configurable minimum floors
        const int MAX_ROOMS = 30;  // Configurable maximum rooms per floor
        const int MIN_ROOMS = 3;   // Configurable minimum rooms per floor

        // Check if floors already exist
        if (floor.getTreeSize(floor.getRootOfFloor()) > 0)
        { // Use AVL tree size to check if floors exist
            cout << "Floors are already set up.\n";
            cout << "Do you want to append more floors? (1 for Yes, 0 for No): ";
            int choice;
            handleChoiceInput(choice);

            if (choice == 1)
            {
                // Get current floor count using AVL tree size
                int existingFloors = floor.getTreeSize(floor.getRootOfFloor());

                // Check if adding new floors exceeds the limit
                int maxNewFloors = MAX_FLOORS - existingFloors;
                if (maxNewFloors <= 0)
                {
                    cout << "Error: Cannot add more floors. Maximum limit of " << MAX_FLOORS << " reached.\n";
                    return;
                }

                // Prompt for the number of additional floors
                int newFloors;
                cout << "Enter the number of additional floors to append (max " << maxNewFloors << "): ";
                handleChoiceInput(newFloors);

                if (newFloors < 1 || newFloors > maxNewFloors)
                {
                    cout << "Invalid number of floors. Try again.\n";
                    return;
                }

                // Get the number of rooms per floor from the first floor (AVL tree)
                int roomsPerFloor = floor.getRoomsPerFloor();
                if (roomsPerFloor == 0)
                {
                    cout << "Error: Unable to determine rooms per floor.\n";
                    return;
                }

                // Append new floors with the same room count using AVL tree
                floor.appendFloorsAndRooms(newFloors);
                cout << "Successfully added " << newFloors << " floors with " << roomsPerFloor << " rooms each.\n";
            }
            else
            {
                cout << "No changes made.\n";
            }
            return;
        }

        // If no floors exist, proceed with initial setup
        int floors, rooms;
        do
        {
            cout << "Enter the number of floors to add (between " << MIN_FLOORS << " and " << MAX_FLOORS << "): ";
            handleChoiceInput(floors);

            if (floors < MIN_FLOORS || floors > MAX_FLOORS)
            {
                cout << "Invalid input for floors. Try again.\n";
                continue;
            }

            cout << "Enter the number of rooms per floor (between " << MIN_ROOMS << " and " << MAX_ROOMS << "): ";
            handleChoiceInput(rooms);

            if (rooms < MIN_ROOMS || rooms > MAX_ROOMS)
            {
                cout << "Invalid input for rooms. Try again.\n";
                continue;
            }

        } while (floors < MIN_FLOORS || floors > MAX_FLOORS || rooms < MIN_ROOMS || rooms > MAX_ROOMS);

        // Create the initial floors using AVL tree
        floor.insertFloors(floors);
        floor.addRoomsToFloors(rooms);
        cout << "Successfully set up " << floors << " floors with " << rooms << " rooms per floor.\n";
    }

    void checkReservation(BookingNode *bookingQueue, BookingRoomHistory &bookingHistory)
    {
        string customername, roomtype;

        cout << "Please enter your name for checking your Reservation: ";
        cin.ignore(); // Clear any leftover characters in the input buffer
        getline(cin, customername);

        int roomChoice;
        cout << "Select room type:\n";
        cout << "1. Single Bed\n";
        cout << "2. Double Bed\n";
        cout << "3. Suite Bed\n";
        cout << "Enter your choice (1, 2, or 3): ";
        handleChoiceInput(roomChoice);

        switch (roomChoice)
        {
        case 1:
            roomtype = "Single";
            break;
        case 2:
            roomtype = "Double";
            break;
        case 3:
            roomtype = "Suite";
            break;
        default:
            cout << "Invalid choice! Please enter 1, 2, or 3.\n";
            return; // Exit the function if invalid input
        }

        // Call the checkReservationByCustomer function to check the reservation
        bookingHistory.checkReservation(customername, roomtype, bookingQueue);
    }
    void updateRoomStatus(Floor &floor)
    {
        int roomID, choice;

        // Prompt the user to enter the Room ID
        cout << "Enter Room ID to update status: ";
        cin >> roomID;

        if (!floor.isValidRoomID(roomID)) // Use the isValidRoomID method
        {
            cout << "Invalid Room ID. Please try again with a valid Room ID.\n";
            return; // Exit if the room ID is invalid
        }

        cout << "Select new status for Room ID " << roomID << ":\n";
        cout << "1. Occupied\n";
        cout << "2. Unavailable\n";
        cout << "Enter your choice (1 or 2): ";
        cin >> choice;

        string status;

        switch (choice)
        {
        case 1:
            status = "Occupied";
            break;
        case 2:
            status = "Unavailable";
            break;
        default:
            cout << "Invalid choice. Please select 1 or 2.\n";
            return;
        }

        // Call updateStatusByID from Floor class to update the room status
        floor.updateStatusByID(roomID, status);
    }

    void showAvailableRoomsByType(Floor &floor)
    {
        int choice;
        string roomType;

        cout << "Select room type:\n";
        cout << "1. Single Bed\n";
        cout << "2. Double Bed\n";
        cout << "3. Suit Bed\n";
        cout << "Enter your choice (1, 2, or 3): ";
        handleChoiceInput(choice);

        switch (choice)
        {
        case 1:
            roomType = "Single";
            break;
        case 2:
            roomType = "Double";
            break;
        case 3:
            roomType = "Suite";
            break;
        default:
            cout << "Invalid choice! Please enter 1, 2, or 3.\n";
            return;
        }

        floor.showAvailableRoomsByType(roomType);
    }
    void cancelRequest(BookingQueue &bookingQueue, Floor &floor, BookingRoomHistory &bookingHistory)
    {
        string customerName;
        cout << "Enter  name to cancel booking request: ";
        cin.ignore();
        getline(cin, customerName);

        if (bookingHistory.getTop() != NULL)
        {
            bookingHistory.cancelBooking(customerName, floor);
        }
        else
        {
            cout << "Booking History is Empty...  After Processing of Request your are Eligible for Cancel Requests ";
        }
    }

    // Display Booking Histories stored into Stack

    void displayBookingHistories(BookingRoomHistory &bookingHistory)
    {
        bookingHistory.displayHistories();
    }
};

// Credentials Class for Storing  login information of Admin,Employee and Customer

class Credentials
{
private:
    string username;
    string password;

public:
    Credentials(string name, string pass) : username(name), password(pass) {}

    void changeUsername(string username)
    {
        this->username = username;
    }
    void changePassword(string password)
    {
        this->password = password;
    }
    string getusername() const
    {
        return username;
    }
    string getpassword() const
    {
        return password;
    }
};

// Check Entered Username or Password is Valid or Invalid

bool Login(const Credentials &credentials)
{
    string inputUsername, inputPassword;
    cout << "Enter username: ";
    cin >> inputUsername;
    cout << "Enter password: ";
    cin >> inputPassword;

    return (inputUsername == credentials.getusername() &&
            inputPassword == credentials.getpassword());
    // return true or false!
}

int main(void)
{
    Credentials adminCredentials("admin", "adm");
    Credentials employeeCredentials("employee", "emp");
    Credentials customerCredentials("customer", "cust");

    Floor floor;
    BookingQueue bookingQueue;
    BookingRoomHistory bookingHistory;
    User user;

    int choice;
    while (true)
    {

        cout << "\n**************************************";
        cout << "\n* Welcome to Hotel Management System *";
        cout << "\n**************************************\n";
        cout << "1. Admin Login\n";
        cout << "2. Employee Login\n";
        cout << "3. Customer Login\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        handleChoiceInput(choice);
        if (choice == 4)
        {
            break;
        }

        switch (choice)
        {
        case 1:
        {
            system("cls");
            if (Login(adminCredentials))
            {

                int adminChoice;
                while (true)
                {
                    // system("cls");

                    cout << "\n********************************";
                    cout << "\n*         Admin Menu:          *";
                    cout << "\n********************************\n";
                    cout << "1. Add Floors & Rooms\n";
                    cout << "2. Show all Floors & Rooms\n";
                    cout << "3. Process Booking Requests\n";
                    cout << "4. Display Booking Requests\n";
                    cout << "5. Cancel Booking Request\n";
                    cout << "6. Show Available Rooms by Type\n";
                    cout << "7. Display Booking Histories\n";
                    cout << "8. Update Room Status By ID \n";
                    cout << "9. Change User-name\n";
                    cout << "10.Change Password\n";
                    cout << "11.Check Reservation \n";
                    cout << "12.Check Rooms Report\n";
                    cout << "13.Log-out\n";

                    cout << "Enter your choice: ";
                    handleChoiceInput(adminChoice);

                    if (adminChoice == 13)
                    {
                        system("cls");
                        break;
                    }
                    switch (adminChoice)
                    {
                    case (1):
                    {
                        user.addFloors(floor);
                        Sleep(1000);
                        system("cls");
                        break;
                    }
                    case (2):
                    {
                        floor.displayFloors();
                        break;
                    }
                    case (3):
                    {
                        user.processBookingRequests(bookingQueue, floor, bookingHistory);
                        Sleep(1000);
                        system("cls");
                        break;
                    }
                    case (4):
                    {
                        user.displayBookingRequests(bookingQueue);
                        break;
                    }
                    case (5):
                    {
                        user.cancelRequest(bookingQueue, floor, bookingHistory);

                        break;
                    }
                    case (6):
                    {
                        user.showAvailableRoomsByType(floor);
                        // floor.showAvailableRoomsByType("Single");
                        break;
                    }
                    case (7):
                    {
                        user.displayBookingHistories(bookingHistory);
                        break;
                    }
                    case (8):
                    {
                        user.updateRoomStatus(floor);
                        Sleep(2000);
                        system("cls");
                        break;
                    }
                    case (9):
                    {
                        string newUserName;
                        cout << "Enter New user-name : ";
                        cin.ignore();
                        getline(cin, newUserName);
                        adminCredentials.changeUsername(newUserName);
                        cout << "Username Changed Successfully ...";
                        Sleep(1500);
                        system("cls");
                        break;
                    }
                    case (10):
                    {
                        string newPassword;
                        cout << "Enter New password : ";
                        cin.ignore();
                        getline(cin, newPassword);
                        adminCredentials.changePassword(newPassword);
                        cout << "Password Changed Successfully ...";
                        Sleep(1500);
                        system("cls");
                        break;
                    }

                    case (11):
                    {
                        user.checkReservation(bookingQueue.getHead(), bookingHistory);
                        Sleep(2000);
                        system("cls");
                        break;
                    }
                    case (12):
                    {
                        user.countRoomsByStatus(floor);
                        Sleep(3000);
                        system("cls");

                        break;
                    }
                    case (13):
                    {
                        system("cls");
                        break;
                    }
                    default:
                    {
                        cout << "Invalid choice. Please try again.\n";
                    }
                    }
                }
            }
            else
            {
                cout << "Invalid username or password.\n";
            }
            break;
        }
        case 2:
        {
            system("cls");
            if (Login(employeeCredentials))
            {

                int employeeChoice;
                while (true)
                {
                    // system("cls");
                    cout << "\n********************************";
                    cout << "\n*         Employee Menu:       *";
                    cout << "\n********************************\n";
                    cout << "1. Show Available Rooms by Type\n";
                    cout << "2. Display all booking requests\n";
                    cout << "3. Process Booking Requests\n";
                    cout << "4. Display Booking Histories\n";
                    cout << "5. Cancel Booking Request\n";
                    cout << "6. Show all Floors & Rooms\n";
                    cout << "7. Update Status By RoomID\n";
                    cout << "8. Check Reservation\n";
                    cout << "9. Change User-name\n";
                    cout << "10.Change Password\n";
                    cout << "11.Check Rooms Report\n";
                    cout << "12.Log-out\n";
                    cout << "Enter your choice: ";
                    handleChoiceInput(employeeChoice);

                    if (employeeChoice == 12)
                    {
                        break;
                    }
                    switch (employeeChoice)
                    {
                    case (1):
                    {
                        user.showAvailableRoomsByType(floor);
                        break;
                    }
                    case (2):
                    {
                        user.displayBookingRequests(bookingQueue);
                        break;
                    }
                    case (3):
                    {
                        user.processBookingRequests(bookingQueue, floor,
                                                    bookingHistory);
                        break;
                    }
                    case (4):
                    {
                        user.displayBookingHistories(bookingHistory);
                        break;
                    }
                    case (5):
                    {
                        user.cancelRequest(bookingQueue, floor, bookingHistory);
                        break;
                    }
                    case (6):
                    {
                        floor.displayFloors();
                        break;
                    }
                    case (7):
                    {
                        user.updateRoomStatus(floor);
                        break;
                    }
                    case (8):
                    {
                        user.checkReservationByEmployee(bookingQueue.getHead(), bookingHistory);
                        Sleep(2000);
                        system("cls");
                        break;
                    }
                    case (9):
                    {
                        string newUserName;
                        cout << "Enter New user-name : ";
                        cin.ignore();
                        getline(cin, newUserName);
                        employeeCredentials.changeUsername(newUserName);
                        cout << "Username Changed Successfully ...";
                        Sleep(1500);
                        system("cls");
                        break;
                    }
                    case (10):
                    {
                        string newPassword;
                        cout << "Enter New password : ";
                        cin.ignore();
                        getline(cin, newPassword);
                        employeeCredentials.changePassword(newPassword);
                        cout << "Password Changed Successfully ...";
                        Sleep(1500);
                        system("cls");

                        break;
                    }
                    case (11):
                    {
                        user.countRoomsByStatus(floor);
                        Sleep(2000);
                        system("cls");

                        break;
                    }
                    case (12):
                    {
                        system("cls");
                        break;
                    }
                    default:
                    {
                        cout << "Invalid choice. Please try again.\n";
                    }
                    }
                }
            }
            else
            {
                cout << "Invalid username or password.\n";
            }
            break;
        }
        case 3:
        {
            system("cls");
            if (Login(customerCredentials))
            {

                int customerChoice;
                while (true)
                {
                    cout << "\n********************************";
                    cout << "\n*         Customer Menu:       *";
                    cout << "\n********************************\n";
                    cout << "1. Send Booking Request\n";
                    cout << "2. Cancel Request\n";
                    cout << "3. Change User-name\n";
                    cout << "4. change Password\n";
                    cout << "5. Check Reservation\n";
                    cout << "6. Log-out\n";
                    cout << "Enter your choice: ";
                    handleChoiceInput(customerChoice);

                    if (customerChoice == 6)
                    {
                        break;
                    }
                    switch (customerChoice)
                    {
                    case (1):
                    {
                        user.sendBookingRequest(bookingQueue, floor, bookingHistory);
                        break;
                    }
                    case (2):
                    {
                        user.cancelRequest(bookingQueue, floor, bookingHistory);
                        break;
                    }
                    case (3):
                    {
                        string newUserName;
                        cout << "Enter New user-name : ";
                        cin.ignore();
                        getline(cin, newUserName);
                        customerCredentials.changeUsername(newUserName);
                        cout << "Username Changed Successfully ...";
                        Sleep(1500);
                        system("cls");
                        break;
                    }
                    case (4):
                    {
                        string newPassword;
                        cout << "Enter New password : ";
                        cin.ignore();
                        getline(cin, newPassword);
                        customerCredentials.changePassword(newPassword);
                        cout << "Password Changed Successfully ...";
                        Sleep(1500);
                        system("cls");
                        break;
                    }
                    case (5):
                    {
                        user.checkReservation(bookingQueue.getHead(), bookingHistory);
                        Sleep(3000);
                        system("cls");
                        break;
                    }
                    case (6):
                    {
                        break;
                    }
                    default:
                    {
                        cout << "Invalid choice. Please try again.\n";
                    }
                    }
                }
            }
            else
            {
                cout << "Invalid username or password.\n";
            }
            break;
        }
        }
    }

    cout << "**************************************" << endl;
    cout << "* Exiting Hotel Management System...." << endl;
    cout << "**************************************" << endl;
    return 0;
}
