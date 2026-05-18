#include <iostream>
#include <string>
#include <cstring>
using namespace std;

// Contact structure
struct Contact
{
    string name;
    string phone;
    string email;
};

// Function declarations
void display_menu();
int get_user_choice();
void display_contact(const Contact &contact);
void display_all_contacts(Contact contacts[], int size);
bool add_contact(Contact contacts[], int &current_size, int max_size);
Contact *search_contact_by_name(Contact contacts[], int size, const string &name);
bool update_contact(Contact &contact, const string &new_phone);
bool update_contact(Contact &contact, const string &new_phone, const string &new_email);
int count_contacts_with_domain(Contact contacts[], int size, const string &domain, int index = 0);
int find_contact_index(Contact contacts[], int size, const string &name);
bool delete_contact(Contact contacts[], int &size, const string &name);
void get_contact_statistics(Contact contacts[], int size, int &total_contacts, int &contacts_with_email, int &contacts_with_phone);

int main()
{
    const int MAX_CONTACTS = 100;
    Contact contacts[MAX_CONTACTS];
    int current_size = 0;
    int choice;
    do
    {
        display_menu();
        choice = get_user_choice();
        switch (choice)
        {
        case 1:
        {
            if (add_contact(contacts, current_size, MAX_CONTACTS))
            {
                cout << "Contact added successfully!" << endl;
            }
            else
            {
                cout << "Failed to add contact. Array might be full." << endl;
            }
            break;
        }
        case 2:
        {
            display_all_contacts(contacts, current_size);
            break;
        }
        case 3:
        {
            string search_name;
            cout << "Enter name to search: ";
            cin.ignore();
            getline(cin, search_name);
            Contact *found = search_contact_by_name(contacts,
                                                    current_size, search_name);
            if (found != nullptr)
            {
                cout << "Contact found:" << endl;
                display_contact(*found);
            }
            else
            {
                cout << "Contact not found." << endl;
            }
            break;
        }
        case 4:
        {
            string search_name;
            cout << "Enter name of contact to update: ";
            cin.ignore();
            getline(cin, search_name);
            Contact *found = search_contact_by_name(contacts,
                                                    current_size, search_name);
            if (found != nullptr)
            {
                int update_choice;
                cout << "Update (1) Phone only or (2) Phone and Email: ";
                cin >> update_choice;
                cin.ignore();
                if (update_choice == 1)
                {
                    string new_phone;
                    cout << "Enter new phone: ";
                    getline(cin, new_phone);
                    if (update_contact(*found, new_phone))
                    {
                        cout << "Contact updated successfully!" << endl;
                    }
                }
                else if (update_choice == 2)
                {
                    string new_phone, new_email;
                    cout << "Enter new phone: ";
                    getline(cin, new_phone);
                    cout << "Enter new email: ";
                    getline(cin, new_email);
                    if (update_contact(*found, new_phone,
                                       new_email))
                    {
                        cout << "Contact updated successfully!" << endl;
                    }
                }
            }
            else
            {
                cout << "Contact not found." << endl;
            }
            break;
        }
        case 5:
        {
            string delete_name;
            cout << "Enter name of contact to delete: ";
            cin.ignore();
            getline(cin, delete_name);
            if (delete_contact(contacts, current_size, delete_name))
            {
                cout << "Contact deleted successfully!" << endl;
            }
            else
            {
                cout << "Contact not found." << endl;
            }
            break;
        }
        case 6:
        {
            string domain;
            cout << "Enter email domain to search (e.g., @gmail.com): ";
            cin.ignore();
            getline(cin, domain);
            int count = count_contacts_with_domain(contacts, current_size, domain);
            cout << "Number of contacts with domain " << domain << ": " << count << endl;
            break;
        }
        case 7:
        {
            int total, with_email, with_phone;
            get_contact_statistics(contacts, current_size, total, with_email, with_phone);
            break;
        }
        case 8:
        {
            cout << "Exiting..." << endl;
            break;
        }
        default:
        {
            cout << "Invalid choice. Please try again." << endl;
        }
        }
        if (choice != 8)
        {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    } while (choice != 8);
    return 0;
}

// Implement all your functions here
// Q1: Contact Structure and Display Functions [15 pts]
// Function to display one contact in console
void display_contact(const Contact &contact)
{

    std::cout << "Name: "  << contact.name  << "\n";
    std::cout << "Phone: " << contact.phone << "\n";
    std::cout << "Email: " << contact.email << "\n";
}

// function to display all contacts in an array
void display_all_contacts(Contact contacts[], int size)
{

    // if array is empty, error
    if (size <= 0)
    {
        std::cout << " the array contacts is empty" << "\n";
        return;
    }

    Contact *current = contacts;
    Contact *last = contacts + size;

    // if not, display all contacts of the array
    while (current < last)
    {
        display_contact(*current);
        std::cout << "\n";
        current++;
    }
}

// Q2: Adding and Searching Contacts [20 pts]
bool add_contact(Contact contacts[], int &current_size, int max_size)
{

    // 1) Check if the array has space
    if (current_size >= max_size)
    {
        return false;
    }

    // ignore cin , because of bug on main
    std::cin.ignore();

    // 2) Prompt the user to enter name, phone, and email using cin and getline
    std::string name;
    std::string phone;
    std::string email;

    // enter name
    std::cout << "Name: ";
    std::getline(std::cin, name);
    // enter phone
    std::cout << "Phone: ";
    std::getline(std::cin, phone);
    // enter email
    std::cout << "Email: ";
    std::getline(std::cin, email);

    // 3) Create a new Contact object
    // if contact already exists, modify it
    Contact *existing_contact = search_contact_by_name(contacts, current_size, name);
    if (existing_contact != nullptr)
    {
        // existing_contact->phone = phone;
        // existing_contact->email = email;
        update_contact(*existing_contact, phone, email);
        return true;
    }
    else
    {
        // else, add a new contact at position current_size
        Contact new_contact;
        new_contact.name = name;
        new_contact.phone = phone;
        new_contact.email = email;
        contacts[current_size] = new_contact;
    }

    // 4) increment current_size
    current_size++;

    return true;
}

// function to search for a contact using it's name
//  pdf doesn't have the star which I assume is a mistake.
Contact *search_contact_by_name(Contact contacts[], int size, const std::string &name)
{

    // if array is empty, return early, no need for message since it is a helper function
    if (size <= 0)
    {
        return nullptr;
    }

    // search for the contact with the same name
    Contact *current = contacts;
    Contact *last = contacts + size;
    while (current < last)
    {
        if (current->name == name)
        {
            return current; // returning pointer
        }
        current++;
    }

    // if not found
    return nullptr;
}

// Q3: Function Overloading for Contact Updates [20 pts]
// update only the phone number
bool update_contact(Contact &contact, const std::string &new_phone)
{

    // check if name is not emtpy
    if (contact.name == "")
    {
        return false;
    }

    // update phone number
    contact.phone = new_phone;
    return true;
}

// update phone and email
bool update_contact(Contact &contact, const std::string &new_phone, const std::string &new_email)
{
    // check if name is not emtpy
    if (contact.name == "")
    {
        return false;
    }

    // update phone number and email
    contact.phone = new_phone;
    contact.email = new_email;
    return true;
}

// Q4: Recursive Contact Search [15 pts]
int count_contacts_with_domain(Contact contacts[], int size, const string &domain, int index)
{

    // Base case: if index >= size, return 0
    if (index >= size)
    {
        return 0;
    }

    // Recursive case: check if the contact at index has the domain in its email, then recursively check the rest.
    Contact current_contact = contacts[index];
    if (current_contact.email.find(domain) != -1)
    { 
        return 1 + count_contacts_with_domain(contacts, size, domain, index + 1);
    }
    else
    { 
        return count_contacts_with_domain(contacts, size, domain, index + 1);
    }

    return 0;
}

// Q5: Pointer-Based Contact Operations [20 pts]

int find_contact_index(Contact contacts[], int size, const string &name)
{
    // 1. Search for the contact by name using pointer arithmetic.
    Contact *current = contacts;
    Contact *last = contacts + size;
    int index = 0;

    while (current < last)
    {
        if (current->name == name)
        {
            return index;
        }
        current++;
        index++;
    }

    return -1;
}

bool delete_contact(Contact contacts[], int &size, const string &name)
{
    // check if contact is there, if not , return early
    int index = find_contact_index(contacts, size, name);
    if (index == -1)
    {
        return false;
    }

    // If found, shift all contacts after the deleted one one position to the left using pointerarithmetic.
    Contact *current = contacts + index;
    Contact *next = current + 1;
    Contact *last = contacts + size;
    while (next < last)
    {
        *current = *next;
        current++;
        next++;
    }

    // Decrement the size to remove the repeated last contact
    size--;
    return true;
}

// Q6: Contact Statistics and Analysis [10 pts]
void get_contact_statistics(Contact contacts[], int size, int &total_contacts, int &contacts_with_email, int &contacts_with_phone)
{

    Contact *current = contacts;
    Contact *last = contacts + size;

    total_contacts = 0;
    contacts_with_email = 0;
    contacts_with_phone = 0;

    // if size <= 0, we skip
    if (size > 0)
    {
        while (current < last)
        {

            // Count total contacts (should equal size).
            total_contacts++;

            // Count contacts that have a non-empty email address.
            if (current->email != "")
            {
                contacts_with_email++;
            }

            // Count contacts that have a non-empty phone number.
            if (current->phone != "")
            {
                contacts_with_phone++;
            }

            current++;
        }
    }

    cout << "Contact Statistics:\n";
    cout << "Total contacts: " << total_contacts << "\n";
    cout << "Contacts with email: " << contacts_with_email << "\n";
    cout << "Contacts with phone: " << contacts_with_phone << "\n";
}

// extra functions so that main runs

void display_menu()
{

    cout << "1. Add a contact" << "\n";
    cout << "2. Display all contacts" << "\n";
    cout << "3. Search for a contact by name\n";
    cout << "4. Update a contact (phone only, or phone and email)\n";
    cout << "5. Delete a contact\n";
    cout << "6. Count contacts by email domain\n";
    cout << "7. Display contact statistics\n";
    cout << "8. Exit\n";
}

int get_user_choice()
{
    int choice = -1;
    cout << "Enter your choice: ";
    cin >> choice;
    return choice;
}