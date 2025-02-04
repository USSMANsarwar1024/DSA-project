#include <iostream>
#include <stdexcept> //exception handle
#include <sstream>
#include <string>

using namespace std;

struct RoomNode
{
    int roomID;
    string roomType;
    string roomStatus;
    RoomNode *next;
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
    RoomNode *roomHead;
    RoomNode *roomTail;
    int size;

public:
    Room() : roomHead(NULL), roomTail(NULL), size(0) {}

    void addRooms(int floorStartingID, int numberOfRooms)
    {
        if (floorStartingID <= 0 || numberOfRooms <= 0)
        {
            cout << "\nInvalid Room Details.\n";
            return;
        }
        else
        {
            for (int i = 0; i < numberOfRooms; i++)
            {
                RoomNode *newnode = new RoomNode;
                newnode->roomID = floorStartingID + i;
                newnode->roomStatus = "Ready";
                if (newnode->roomID >= floorStartingID)
                {
                    newnode->roomType = assignRoomType(newnode->roomID, floorStartingID);
                }
                newnode->next = NULL;

                if (roomHead == NULL)
                {
                    roomHead = newnode;
                    roomTail = newnode;
                }
                else
                {
                    roomTail->next = newnode;
                    roomTail = newnode;
                }
                size++;
            }
        }
    }

    string assignRoomType(int roomID, int startingID)
    {
        if (roomID >= startingID && roomID <= startingID + 4) //40%
        {
            return "Single Bed";
        }
        else if (roomID > startingID + 4 && roomID <= startingID + 7)//70%
        {
            return "Double Bed";
        }
        else
        {
            return "Suite Bed";
        }
    }

    void displayRooms()
    {
        if (roomHead == NULL)
        {
            cout << "\nRoom Link List is Empty " << endl;
            return;
        }
        cout << "\nTotal Number of Rooms on This Floor : " << size << endl;

        RoomNode *temp = roomHead;
        while (temp)
        {
            cout << "Room ID : " << temp->roomID << "\t| Room Type   : " << temp->roomType
                 << "\t| Room Status : " << temp->roomStatus << endl;
            temp = temp->next;
        }
        cout << "\n--------------------------------------------------------------------------\n";
    }

    int getsize()
    {
        return size;
    }

    void findRoomId(const string type) const
    {
        if (!roomHead)
        {
            cout << "No Rooms Available!\n";
            return;
        }

        RoomNode *temp = roomHead;
        bool found = false;

        // cout << type << "  Rooms Available \n";
        while (temp != NULL)
        {
            if (temp->roomType == type && temp->roomStatus == "Ready")
            {
                // cout<<"Booking Confirmed Successfully ";
                cout << "Room ID: " << temp->roomID << endl;

                found = true;
            }
            temp = temp->next;
        }

        if (!found)
        {
            cout << "No available rooms of type '" << type << "' found.\n";
        }
    }

    int findRoomIdandAssign(const string type) const
    {
        if (!roomHead)
        {
            cout << "No Rooms Available!\n";
            return -1;
        }

        RoomNode *temp = roomHead;
        bool found = false;

        // cout << type << "  Rooms Available \n";
        while (temp != NULL)
        {
            if (temp->roomType == type && temp->roomStatus == "Ready")
            {
                // cout<<"Booking Confirmed Successfully ";

                temp->roomStatus = "Occupied";
                found = true;
                return temp->roomID;
            }
            temp = temp->next;
        }

        if (!found)
        {
            cout << "No available rooms of type '" << type << "' found.\n";
        }
        return 0;
    }

    void updateRoomStatus(int roomID, const string newStatus)
    {
        if (!roomHead)
        {
            cout << "No Rooms Available!\n";
            return;
        }

        RoomNode *temp = roomHead;
        while (temp)
        {
            if (temp->roomID == roomID)
            {
                temp->roomStatus = newStatus;
                cout << "Room ID: " << roomID << " status updated to  " << newStatus << endl;
                return;
            }
            temp = temp->next;
        }

        cout << "Room ID: " << roomID << " not found.\n";
    }

    RoomNode *getRoomHead() const
    {
        return roomHead;
    }

    ~Room()
    {
        RoomNode *temp = roomHead;
        while (temp)
        {
            RoomNode *next = temp->next;
            delete temp;
            temp = next;
        }
        roomHead = NULL;
        roomTail = NULL;
    }
};

struct FloorNode
{
    int floorID;
    FloorNode *next;
    FloorNode *previous;
    Room room;
};

class Floor
{
    FloorNode *floorHead;
    FloorNode *floorTail;
    int size;

public:
    Floor() : floorHead(NULL), floorTail(NULL), size(0) {}

    void insertFloors(int numberOfFloors)
    {
        if (numberOfFloors <= 0)
        {
            cout << "\nInvalid Number of Floors ";
            return;
        }

        for (int i = 1; i <= numberOfFloors; i++)
        {
            FloorNode *newnode = new FloorNode;
            newnode->floorID = i;
            newnode->previous = NULL;
            newnode->next = NULL;

            if (floorHead == NULL)
            {
                floorHead = newnode;
                floorTail = newnode;
            }
            else
            {
                floorTail->next = newnode;
                newnode->previous = floorTail;
                floorTail = newnode;
            }

            size++;
        }
    }


    void addRoomsToFloors(int numberOfRooms)
    {

        FloorNode *temp = floorHead;
        while (temp)
        {

            int startingID = (temp->floorID) * 100 + 1;
            temp->room.addRooms(startingID, numberOfRooms);
            temp = temp->next;
        }
    }

    void displayFloors()
    {
        if (floorHead == NULL)
        {
            cout << "\nNo Floors Available ";
            return;
        }

        FloorNode *temp = floorHead;
        int totalRooms = 0;
        while (temp)
        {
            cout << "Floor ID : " << temp->floorID << endl;
            cout << "************";

            temp->room.displayRooms();
            totalRooms += temp->room.getsize();
            temp = temp->next;
        }

        cout << "Total Number of Floors " << size << endl;
        cout << "Total Number of Rooms " << totalRooms << endl;
    }

    void showAvailableRoomsByType(string roomtype)
    {
        if (floorHead == NULL)
        {
            cout << "\nNo Available Floors  ";
            return;
        }

        FloorNode *temp = floorHead;
        while (temp != NULL)
        {

            cout << "Floor ID : " << temp->floorID << endl;
            cout << "Checking Availability of " << roomtype << " Rooms ";

            temp->room.findRoomId(roomtype);
            temp = temp->next;
        }
    }

    int assignRoom(string roomtype)
    {
        FloorNode *temp = floorHead;
        while (temp)
        {
            int roomId = temp->room.findRoomIdandAssign(roomtype);

            if (roomId != -1)
            {
                return roomId;
            }

            temp = temp->next;
        }

        cout << "No rooms of type '" << roomtype << "' available on any floor.\n";
        return -1;
    }

    void updateStatusByID(int roomID, string status)
    {
        FloorNode *temp = floorHead;
        while (temp)
        {
            RoomNode *roomTemp = temp->room.getRoomHead(); // Access the room linked list
            while (roomTemp)
            {
                if (roomTemp->roomID == roomID)
                {
                    roomTemp->roomStatus = status;
                    cout << "Room ID " << roomID << " status updated to " << status << ".\n";
                    return;
                }
                roomTemp = roomTemp->next;
            }
            temp = temp->next;
        }
        cout << "Room ID " << roomID << " not found.\n";
    }

    ~Floor()
    {
        FloorNode *temp = floorHead;
        while (temp)
        {
            FloorNode *next = temp->next;
            delete temp;
            temp = next;
        }
        floorHead = NULL;
        floorTail = NULL;
    }
};
class BookingRoomHistory
{
    BookingHistory *top;
    int size;

public:
    BookingRoomHistory() : top(NULL), size(0) {}

    void storeBooking(string customerName, int stayDuration, int roomID, string roomType) // stack top!
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

    void deleteBookingHistory(string customername, int roomid)
    {
        if (top == NULL)
        {
            cout << "No Bookings Histories Yet ";
            return;
        }

        cout << "Booking History of " << top->customerName << " is removed ";
        BookingHistory *temp = top;
        top = top->next;
        delete temp;
        size--;
    }
    void pop(int roomId)
    {
        if (!top)
            return;

        if (top->roomID == roomId)
        {
            BookingHistory *temp = top;
            top = top->next;
            delete temp;
        }
        else
        {
            BookingHistory *prev = top;
            BookingHistory *current = top->next;

            while (current != NULL)
            {
                if (current->roomID == roomId)
                {
                    prev->next = current->next;
                    delete current;
                    return;
                }
                prev = current;
                current = current->next;
            }
        }
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
    BookingRoomHistory *bookinghistory;

public:
    BookingQueue() : bookHead(NULL), bookTail(NULL), numberOfRequests(0) {}

    void sendBookingRequest(int priorityID, string customerName, int stayDuration, string roomType)
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
            if (bookHead->priorityID != 1) // very dangerous case!!!
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

    void displayRequest()
    {
        if (!bookHead)
        {
            cout << "\nBooking Request is Empty \n";
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
            // linear search
        }
        cout << "\n**********************************************\n";
    }

    void processRequests(Floor &floor, BookingRoomHistory &bookingHistory) //dequeue
    {
        if (!bookHead)
        {
            cout << "No Booking Requests to Process\n";
            return;
        }

        BookingNode *current = bookHead;
        int assignedRoomID = floor.assignRoom(current->roomType);

        if (assignedRoomID != -1)
        {
            bookingHistory.storeBooking(current->customerName, current->stayDuration,
                                        assignedRoomID, current->roomType);
            cout << "Booking Successful for " << current->customerName
                 << ", Room ID: " << assignedRoomID << "\n";
        }
        else
        {
            cout << "Booking Failed for " << current->customerName << ": No Available Rooms\n";
        }

        bookHead = bookHead->next;
        delete current;
        numberOfRequests--;
    }

    void cancelBookingRequest(string customerName)
    {
        if (!bookHead)
        {
            cout << "No booking requests to cancel.\n";
            return;
        }

        BookingNode *current = bookHead;
        BookingNode *prev = NULL;

        while (current)
        {
            if (current->customerName == customerName)
            {
                if (prev)
                {
                    prev->next = current->next;
                }
                else
                {
                    bookHead = current->next;
                }

                if (current == bookTail)
                {
                    bookTail = prev;
                }

                delete current;
                numberOfRequests--;
                cout << "Booking request for \"" << customerName << "\" has been canceled.\n";
                return;
            }

            prev = current;
            current = current->next;
        }

        cout << "No matching booking request found for customer \"" << customerName << "\".\n";
    }

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
class InvalidInputException : public std::runtime_error
{
public:
    InvalidInputException() : runtime_error("Invalid input. Please enter a valid integer.") {}
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
    catch (const std::invalid_argument &)
    {
        throw InvalidInputException();
    }
    catch (const std::out_of_range &)
    {
        throw InvalidInputException();
    }
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
            cout << "Please try again: ";
        }
    }
}

class Admin
{
public:
    void addFloors(Floor &floor)
    {
        int floors;
        int rooms;
        do
        {
            cout << "Enter the number of floors to add: ";
            handleChoiceInput(floors);

            if (floors < 3 || floors > 30)
            {
                cout << "Invalid input for floors. The number of floors must be between 3 and 30. "
                        "Try again."
                     << endl;
                continue; // Skip the rest of the loop and prompt for floors again
            }

            cout << "Enter the number of rooms per floor to add: ";
            handleChoiceInput(rooms);

            if (rooms < 3 || rooms > 30)
            {
                cout << "Invalid input for rooms. The number of rooms per floor must be between 3 "
                        "and 30. Try again."
                     << endl;
                continue; // Skip the rest of the loop and prompt for rooms again
            }
        } while (floors < 3 || floors > 30 || rooms < 3 || rooms > 30);

        floor.insertFloors(floors);
        floor.addRoomsToFloors(rooms);
    }

    void processBookingRequests(BookingQueue &bookingQueue, Floor &floor,
                                BookingRoomHistory &bookingHistory)
    {
        bookingQueue.processRequests(floor, bookingHistory);
    }

    void displayBookingHistories(BookingRoomHistory &bookingHistory)
    {
        bookingHistory.displayHistories();
    }
    void displayBookingRequests(BookingQueue &bookingQueue)
    {
        bookingQueue.displayRequest();
    }
    void cancelRequest(BookingQueue &bookingQueue)
    {
        string customerName;
        cout << "Enter your name to cancel booking request: ";
        cin.ignore();
        getline(cin, customerName);
        bookingQueue.cancelBookingRequest(customerName);
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
            roomType = "Single Bed";
            break;
        case 2:
            roomType = "Double Bed";
            break;
        case 3:
            roomType = "Suit Bed";
            break;
        default:
            cout << "Invalid choice! Please enter 1, 2, or 3.\n";
            return;
        }

        floor.showAvailableRoomsByType(roomType);
    }
};

class Employee
{
public:
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
            roomType = "Single Bed";
            break;
        case 2:
            roomType = "Double Bed";
            break;
        case 3:
            roomType = "Suit Bed";
            break;
        default:
            cout << "Invalid choice! Please enter 1, 2, or 3.\n";
            return;
        }

        floor.showAvailableRoomsByType(roomType);
    }

    void processBookingRequests(BookingQueue &bookingQueue, Floor &floor,
                                BookingRoomHistory &bookingHistory)
    {
        bookingQueue.processRequests(floor, bookingHistory);
    }
    void displayBookingRequests(BookingQueue &bookingQueue)
    {
        bookingQueue.displayRequest();
    }

    void cancelRequest(BookingQueue &bookingQueue)
    {
        string customerName;
        cout << "Enter your name to cancel booking request: ";
        cin.ignore();
        getline(cin, customerName);
        bookingQueue.cancelBookingRequest(customerName);
    }
};

class Customer
{
public:
    void sendBookingRequest(BookingQueue &bookingQueue)
    {
        string name, roomType;
        cout << "Please enter your name: ";
        cin.ignore(); // Clear any leftover characters in the input buffer
        getline(cin, name); //cin why no!!!!

        // usman sarwar xyz abc shidoiqws qpwdiqpws qdiqpwd //each char 1 byte
        //cin usman

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
            handleChoiceInput(stayDuration); // Validate the stay duration input
            if (stayDuration < 1 || stayDuration > 30)
            {
                cout << "Invalid duration. Please try again (1-30): ";
            }
        } while (stayDuration < 1 || stayDuration > 30);

        int choice = 0;
        do
        {
            cout << "1. Single Bed-Rooms\n2. Double Bed-Rooms\n3. Suite Bed-Rooms\nChoice: ";
            handleChoiceInput(choice); // Validate the room type choice

            switch (choice)
            {
            case 1:
                roomType = "Single Bed";
                break;
            case 2:
                roomType = "Double Bed";
                break;
            case 3:
                roomType = "Suite Bed";
                break;
            default:
                cout << "Invalid choice. Try again.\n";
                break;
            }
        } while (choice <= 0 || choice > 3); // Repeat if the choice is invalid

        cout << "Dear Customer " << name << ", Request Successfully Received for " << roomType
             << " for " << stayDuration << " nights " << ((priority == 1) ? " | VIP | " : "")
             << endl;

        bookingQueue.sendBookingRequest(priority, name, stayDuration, roomType);
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
            roomType = "Single Bed";
            break;
        case 2:
            roomType = "Double Bed";
            break;
        case 3:
            roomType = "Suit Bed";
            break;
        default:
            cout << "Invalid choice! Please enter 1, 2, or 3.\n";
            return;
        }

        floor.showAvailableRoomsByType(roomType);
    }

    void cancelRequest(BookingQueue &bookingQueue)
    {
        string customerName;
        cout << "Enter your name to cancel booking request: ";
        cin.ignore();
        getline(cin, customerName);
        bookingQueue.cancelBookingRequest(customerName);
    }
};

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
    Credentials adminCredentials("admin", "123");
    Credentials employeeCredentials("employee", "emp");
    Credentials customerCredentials("customer", "cust");//users

    Floor floor;
    BookingQueue bookingQueue;
    BookingRoomHistory bookingHistory;

    //    void sendBookingRequest(int priorityID, string customername, int stayduration,
    //                            string roomtype) // push priority wise
    bookingQueue.sendBookingRequest(2, "Usman Sarwar ", 5, "Single Bed");
    bookingQueue.sendBookingRequest(2, "Bakir Raza Sb", 9, "Double Bed");
    bookingQueue.sendBookingRequest(1, "Mudassar Arif", 3, "Suite Bed");
    bookingQueue.sendBookingRequest(2, "Rafay Gill Sb", 6, "Single Bed");
    bookingQueue.sendBookingRequest(1, "Zaigham Abbas", 2, "Suite Bed");

    int choice;
    while (true)
    {
        system("clear");
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

        //cin --> string/ char
        //cin.fail()
        //cin.clear() --> buffer clear


        switch (choice)
        {
        case 1:
        {
            system("clear");
            if (Login(adminCredentials))
            {
                Admin admin;
                int adminChoice;
                while (true)
                {
                    cout << "\nAdmin Menu:\n";
                    cout << "1. Add Floors & Rooms\n";
                    cout << "2. Show all Floors & Rooms\n";
                    cout << "4. Process Booking Requests\n";
                    cout << "5. Display Booking Histories\n";
                    cout << "6. Cancel Booking Request\n";
                    cout << "7. Show Available Rooms by Type\n";
                    cout << "8. Change User-name\n";
                    cout << "9. change Password\n";
                    cout << "10. Display Booking Requests\n";
                    cout << "11. Log-out\n";
                    cout << "Enter your choice: ";
                    handleChoiceInput(adminChoice);

                    if (adminChoice == 11)
                    {
                        break;
                    }
                    switch (adminChoice)
                    {
                    case (1):
                    {
                        admin.addFloors(floor);
                        break;
                    }
                    case (2):
                    {
                        floor.displayFloors();
                        break;
                    }
                    case (3):
                    {
                        admin.displayBookingRequests(bookingQueue);
                        break;
                    }
                    case (4):
                    {
                        admin.processBookingRequests(bookingQueue, floor, bookingHistory);
                        break;
                    }
                    case (5):
                    {
                        admin.displayBookingHistories(bookingHistory);
                        break;
                    }
                    case (6):
                    {
                        admin.cancelRequest(bookingQueue);
                        break;
                    }
                    case (7):
                    {
                        admin.showAvailableRoomsByType(floor);
                        break;
                    }
                    case (8):
                    {
                        string newUserName;
                        cout << "Enter New user-name : ";
                        cin.ignore();
                        getline(cin, newUserName);
                        adminCredentials.changeUsername(newUserName);
                        break;
                    }
                    case (9):
                    {
                        string newPassword;
                        cout << "Enter New password : ";
                        cin.ignore();
                        getline(cin, newPassword);
                        adminCredentials.changePassword(newPassword);
                        break;
                    }
                    case (10):
                    {
                        admin.displayBookingRequests(bookingQueue);
                        break;
                    }
                    case (11):
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
        case 2:
        {
            system("clear");
            if (Login(employeeCredentials))
            {
                Employee employee;
                int employeeChoice;
                while (true)
                {
                    cout << "\nEmployee Menu:\n";
                    cout << "1. Show Available Rooms by Type\n";
                    cout << "2. Display all booking requests\n";
                    cout << "3. Process Booking Requests\n";
                    cout << "4. Cancel Booking Request\n";
                    cout << "5. Change User-name\n";
                    cout << "6. change Password\n";
                    cout << "7. Log-out\n";
                    cout << "Enter your choice: ";
                    handleChoiceInput(employeeChoice);

                    if (employeeChoice == 7)
                    {
                        break;
                    }
                    switch (employeeChoice)
                    {
                    case (1):
                    {
                        employee.showAvailableRoomsByType(floor);
                        break;
                    }
                    case (2):
                    {
                        employee.displayBookingRequests(bookingQueue);
                        break;
                    }
                    case (3):
                    {
                        employee.processBookingRequests(bookingQueue, floor,
                                                        bookingHistory);
                        break;
                    }
                    case (4):
                    {
                        employee.cancelRequest(bookingQueue);
                        break;
                    }
                    case (5):
                    {
                        string newUserName;
                        cout << "Enter New user-name : ";
                        cin.ignore();
                        getline(cin, newUserName);
                        employeeCredentials.changeUsername(newUserName);
                        break;
                    }
                    case (6):
                    {
                        string newPassword;
                        cout << "Enter New password : ";
                        cin.ignore();
                        getline(cin, newPassword);
                        employeeCredentials.changePassword(newPassword);
                        break;
                    }
                    case (7):
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
        case 3:
        {
            system("clear");
            if (Login(customerCredentials))
            {
                Customer customer;
                int customerChoice;
                while (true)
                {
                    cout << "\nCustomer Menu:\n";
                    cout << "1. Show Available Rooms by Type\n";
                    cout << "2. Send Booking Request\n";
                    cout << "3. Cancel Request\n";
                    cout << "4. Change User-name\n";
                    cout << "5. change Password\n";
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
                        customer.showAvailableRoomsByType(floor);
                        break;
                    }
                    case (2):
                    {
                        customer.sendBookingRequest(bookingQueue);
                        break;
                    }
                    case (3):
                    {
                        customer.cancelRequest(bookingQueue);
                        break;
                    }
                    case (4):
                    {
                        string newUserName;
                        cout << "Enter New user-name : ";
                        cin.ignore();
                        getline(cin, newUserName);
                        customerCredentials.changeUsername(newUserName);
                        break;
                    }
                    case (5):
                    {
                        string newPassword;
                        cout << "Enter New password : ";
                        cin.ignore();
                        getline(cin, newPassword);
                        customerCredentials.changePassword(newPassword);
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
    cout << "* Exitting Hotel Management System...." << endl;
    cout << "**************************************" << endl;
    return 0;
}
