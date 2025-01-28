#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Appointment {
    int id;
    char date[11];
    char patientName[50];
    char doctorName[50];
    struct Appointment* next;
};

struct Medicine {
    int id;
    char name[50];
    int quantity;
    struct Medicine* next;
};

struct Room {
    int id;
    char type[50];
    int isAvailable;
    struct Room* next;
};

struct Patient {
    int id;
    char name[50];
    int age;
    char address[100];
    struct Doctor* assignedDoctor;
    struct Medicine* prescribedMedicines; 
    struct Room* assignedRoom;  
    struct Patient* next;
    
};

struct Doctor {
    int id;
    char name[50];
    char specialization[50];
    struct Doctor* next;
};

struct PharmacyItem {
    int id;
    char name[50];
    int quantity;
    struct PharmacyItem* next;
};

struct PrescribedMedicine {
    int medicineId;
    char medicineName[50];
    int quantity;
    struct PrescribedMedicine *next;
};

typedef struct {
    struct Appointment* front;
    struct Appointment* rear;
} Queue;

void initializeQueue(Queue* q) {
    q->front = q->rear = NULL;
}

struct Appointment* createAppointment(int id, const char* date, const char* patientName, const char* doctorName) {
    struct Appointment* newAppointment = (struct Appointment*)malloc(sizeof(struct Appointment));
    newAppointment->id = id;
    strcpy(newAppointment->date, date);
    strcpy(newAppointment->patientName, patientName);
    strcpy(newAppointment->doctorName, doctorName);
    newAppointment->next = NULL;
    return newAppointment;
}

void enqueueAppointment(Queue* q, int id, const char* date, const char* patientName, const char* doctorName) {
    struct Appointment* newAppointment = createAppointment(id, date, patientName, doctorName);
    if (q->rear == NULL) {
        q->front = q->rear = newAppointment;
    } else {
        q->rear->next = newAppointment;
        q->rear = newAppointment;
    }
    printf("Appointment ID %d for %s with Dr. %s on %s has been scheduled.\n", id, patientName, doctorName, date);
}

void completeAppointment(Queue* q, int id) {
    if (q->front == NULL) {
        printf("No appointments scheduled.\n");
        return;
    }

    struct Appointment* temp = q->front;
    struct Appointment* prev = NULL;

    while (temp != NULL && temp->id != id) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) {
        printf("Appointment ID %d not found.\n", id);
        return;
    }
    if (prev == NULL) { 
        q->front = temp->next;
    } else {
        prev->next = temp->next;
    }
    if (temp == q->rear) { 
        q->rear = prev;
    }

    printf("Appointment ID %d with Dr. %s for %s on %s has been marked as completed.\n", temp->id, temp->doctorName, temp->patientName, temp->date);
    free(temp);
}

void displayAppointments(Queue* q) {
    if (q->front == NULL) {
        printf("No appointments scheduled.\n");
        return;
    }
    struct Appointment* temp = q->front;
    printf("Scheduled Appointments:\n");
    while (temp != NULL) {
        printf("Appointment ID: %d, Patient: %s, Doctor: %s, Date: %s\n", temp->id, temp->patientName, temp->doctorName, temp->date);
        temp = temp->next;
    }
}

void displayDoctors(struct Doctor* head) {
    if (head == NULL) {
        printf("No doctors registered.\n");
        return;
    }
    struct Doctor* temp = head;
    printf("Available Doctors:\n");
    while (temp != NULL) {
        printf("Doctor ID: %d, Name: %s, Specialization: %s\n", temp->id, temp->name, temp->specialization);
        temp = temp->next;
    }
}

struct Doctor* findDoctorById(struct Doctor* head, int id) {
    struct Doctor* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            return temp;
        }
        temp = temp->next;
    }
    return NULL;
}

void appointmentsMenu(Queue* q, struct Doctor* doctorHead) {
    int choice, id, doctorId;
    char date[11], patientName[50];

    while (1) {
        printf("\nAppointments Menu:\n");
        printf("1. Schedule a new appointment\n");
        printf("2. View all appointments\n");
        printf("3. Mark appointment as completed\n");
        printf("4. Back to main menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
            printf("Enter Appointment ID: ");
            scanf("%d", &id);

            printf("Enter Appointment Date (YYYY-MM-DD): ");
            scanf("%10s", date);


            getchar(); 
            
            printf("Enter Patient Name: ");
            fgets(patientName, sizeof(patientName), stdin);
            patientName[strcspn(patientName, "\n")] = '\0'; 

            displayDoctors(doctorHead);
            printf("Enter Doctor ID for the appointment: ");
            scanf("%d", &doctorId);

            struct Doctor* selectedDoctor = findDoctorById(doctorHead, doctorId);
            if (selectedDoctor != NULL) {
                enqueueAppointment(q, id, date, patientName, selectedDoctor->name);
            } else {
                printf("Invalid Doctor ID. Please try again.\n");
            }
            break;

            case 2:
                displayAppointments(q);
                break;

            case 3:
                printf("Enter Appointment ID to mark as completed: ");
                scanf("%d", &id);
                completeAppointment(q, id);
                break;

            case 4:
                return;

            default:
                printf("Invalid choice! Please select a valid option.\n");
                break;
        }
    }
}

struct PharmacyItem* createPharmacyItem(int id, const char* name, int quantity) {
    struct PharmacyItem* newItem = (struct PharmacyItem*)malloc(sizeof(struct PharmacyItem));
    newItem->id = id;
    strcpy(newItem->name, name);
    newItem->quantity = quantity;
    newItem->next = NULL;
    return newItem;
}

void addPharmacyItem(struct PharmacyItem** head, int id, const char* name, int quantity) {
    struct PharmacyItem* newItem = createPharmacyItem(id, name, quantity);
    newItem->next = *head;
    *head = newItem;
    printf("Pharmacy item %s has been added with quantity %d.\n", name, quantity);
}

void viewPharmacyItems(struct PharmacyItem* head) {
    if (head == NULL) {
        printf("No items in the pharmacy.\n");
        return;
    }
    struct PharmacyItem* temp = head;
    printf("Pharmacy Inventory:\n");
    while (temp != NULL) {
        printf("Item ID: %d, Name: %s, Quantity: %d\n", temp->id, temp->name, temp->quantity);
        temp = temp->next;
    }
}

void reduceStock(struct PharmacyItem* head, int id, int quantity) {
    struct PharmacyItem* temp = head;
    while (temp != NULL) {
        if (temp->id == id) {
            if (temp->quantity < quantity) {
                printf("Not enough stock for %s. Only %d left.\n", temp->name, temp->quantity);
            } else {
                temp->quantity -= quantity;
                printf("Reduced stock of %s by %d. Remaining: %d\n", temp->name, quantity, temp->quantity);
                if (temp->quantity == 0) {
                    printf("Stock of %s is now empty! Please order new stock.\n", temp->name);
                }
            }
            return;
        }
        temp = temp->next;
    }
    printf("Item with ID %d not found in pharmacy.\n", id);
}

void pharmacyMenu(struct PharmacyItem** pharmacyHead) {
    int choice, id, quantity;
    char name[50];

    while (1) {
        printf("\nPharmacy Menu:\n");
        printf("1. Add a new item\n");
        printf("2. View all items\n");
        printf("3. Reduce item stock\n");
        printf("4. Back to main menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Item ID: ");
                scanf("%d", &id);
                printf("Enter Item Name: ");
                scanf(" %[^\n]", name);
                printf("Enter Quantity: ");
                scanf("%d", &quantity);

                addPharmacyItem(pharmacyHead, id, name, quantity);
                break;

            case 2:
                viewPharmacyItems(*pharmacyHead);
                break;

            case 3:
                printf("Enter Item ID to reduce stock: ");
                scanf("%d", &id);
                printf("Enter quantity to reduce: ");
                scanf("%d", &quantity);

                reduceStock(*pharmacyHead, id, quantity);
                break;

            case 4:
                return;

            default:
                printf("Invalid choice! Please select a valid option.\n");
                break;
        }
    }
}

void addMedicineToPatient(struct Patient* patient, int id, const char* name, int quantity) {
    struct Medicine* newMedicine = (struct Medicine*)malloc(sizeof(struct Medicine));
    newMedicine->id = id;
    strcpy(newMedicine->name, name);
    newMedicine->quantity = quantity;
    newMedicine->next = patient->prescribedMedicines;
    patient->prescribedMedicines = newMedicine;
}

void displayPrescribedMedicines(struct Medicine* head) {
    struct Medicine* temp = head;
    printf("Prescribed Medicines:\n");
    while (temp != NULL) {
        printf("Medicine ID: %d, Name: %s, Quantity: %d\n", temp->id, temp->name, temp->quantity);
        temp = temp->next;
    }
}

void addRoom(struct Room** roomHead, int id, const char* type) {
    struct Room* newRoom = (struct Room*)malloc(sizeof(struct Room));
    newRoom->id = id;
    strcpy(newRoom->type, type);
    newRoom->isAvailable = 1;  
    newRoom->next = *roomHead;
    *roomHead = newRoom;
    printf("Room ID %d of type %s has been added and is available.\n", id, type);
}

void viewAvailableRooms(struct Room* roomHead) {
    if (roomHead == NULL) {
        printf("No rooms available.\n");
        return;
    }
    struct Room* temp = roomHead;
    printf("Available Rooms:\n");
    while (temp != NULL) {
        if (temp->isAvailable) {
            printf("Room ID: %d, Type: %s\n", temp->id, temp->type);
        }
        temp = temp->next;
    }
}

struct Room* assignRoomToPatient(struct Room* roomHead) {
    struct Room* temp = roomHead;
    while (temp != NULL) {
        if (temp->isAvailable) {
            temp->isAvailable = 0;  
            return temp;
        }
        temp = temp->next;
    }
    printf("No available rooms for assignment.\n");
    return NULL;
}

void roomsMenu(struct Room** roomHead) {
    int choice, id;
    char type[50];

    while (1) {
        printf("\nRooms Menu:\n");
        printf("1. Add a new room\n");
        printf("2. View available rooms\n");
        printf("3. Back to main menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Room ID: ");
                scanf("%d", &id);
                printf("Enter Room Type: ");
                scanf(" %[^\n]", type);
                addRoom(roomHead, id, type);
                break;

            case 2:
                viewAvailableRooms(*roomHead);
                break;

            case 3:
                return;

            default:
                printf("Invalid choice! Please select a valid option.\n");
                break;
        }
    }
}
void viewPatientRecord(struct Patient* patient) {
    if (patient == NULL) {
        printf("Patient not found.\n");
        return;
    }

    printf("\nPatient Record:\n");
    printf("ID: %d\n", patient->id);
    printf("Name: %s\n", patient->name);
    printf("Age: %d\n", patient->age);
    printf("Address: %s\n", patient->address);

    if (patient->assignedDoctor != NULL) {
        printf("Assigned Doctor: Dr. %s\n", patient->assignedDoctor->name);
    } else {
        printf("No doctor assigned.\n");
    }

    if (patient->assignedRoom != NULL) {
        printf("Assigned Room ID: %d\n", patient->assignedRoom->id);
        printf("Room Type: %s\n", patient->assignedRoom->type);
    } else {
        printf("No room assigned.\n");
    }

    if (patient->prescribedMedicines != NULL) {
        printf("Prescribed Medicines:\n");
        struct PrescribedMedicine* temp = patient->prescribedMedicines;
        while (temp != NULL) {
            printf("  - Medicine ID: %d, Name: %s, Quantity: %d\n", temp->medicineId, temp->medicineName, temp->quantity);
            temp = temp->next;
        }
    } else {
        printf("No medicines prescribed.\n");
    }
}
void patientsMenu(struct Patient** patientHead, struct Doctor* doctorHead, struct PharmacyItem* pharmacyHead, struct Room* roomHead) {
    int choice, id, age, doctorId, itemId, quantity;
    char name[50], address[100];
    char addMedicine;

    while (1) {
        printf("\nPatients Menu:\n");
        printf("1. Add a new patient\n");
        printf("2. View all patients\n");
        printf("3. View Patient Record\n");
        printf("4. Back to main menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Patient ID: ");
                scanf("%d", &id);

                getchar(); 

                printf("Enter Patient Name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = '\0'; 

                printf("Enter Age: ");
                scanf("%d", &age);

                getchar(); 

                printf("Enter Address: ");
                fgets(address, sizeof(address), stdin);
                address[strcspn(address, "\n")] = '\0'; 

                displayDoctors(doctorHead);  
                printf("Enter Doctor ID to assign to this patient: ");
                scanf("%d", &doctorId);

                struct Doctor* assignedDoctor = findDoctorById(doctorHead, doctorId);
                if (assignedDoctor == NULL) {
                    printf("Invalid Doctor ID. Patient will be added without an assigned doctor.\n");
                }

                struct Patient* newPatient = (struct Patient*)malloc(sizeof(struct Patient));
                newPatient->id = id;
                strcpy(newPatient->name, name);
                newPatient->age = age;
                strcpy(newPatient->address, address);
                newPatient->assignedDoctor = assignedDoctor;
                newPatient->prescribedMedicines = NULL; 
                newPatient->next = *patientHead;
                *patientHead = newPatient;

                printf("Available rooms:\n");
                viewAvailableRooms(roomHead);

                int roomId;
                printf("Enter Room ID to assign to this patient (or 0 to skip): ");
                scanf("%d", &roomId);

                if (roomId != 0) {
                    struct Room* selectedRoom = roomHead;
                    while (selectedRoom != NULL) {
                        if (selectedRoom->id == roomId && selectedRoom->isAvailable) {
                            selectedRoom->isAvailable = 0;  
                            newPatient->assignedRoom = selectedRoom;
                            printf("Room ID %d assigned to patient %s.\n", selectedRoom->id, newPatient->name);
                            break;
                        }
                        selectedRoom = selectedRoom->next;
                    }
                } else {
                    newPatient->assignedRoom = NULL;
                }

                printf("Would you like to select medicine(s) for this patient? (y/n): ");
                scanf(" %c", &addMedicine);

                while (addMedicine == 'y' || addMedicine == 'Y') {
                    viewPharmacyItems(pharmacyHead);  
                    printf("Enter Medicine Item ID to select: ");
                    scanf("%d", &itemId);
                    printf("Enter quantity: ");
                    scanf("%d", &quantity);

                    reduceStock(pharmacyHead, itemId, quantity);  

                    addMedicineToPatient(newPatient, itemId, pharmacyHead->name, quantity);

                    printf("Would you like to add more medicine(s) for this patient? (y/n): ");
                    scanf(" %c", &addMedicine);
                }
                break;

            case 2:
                if (*patientHead == NULL) {
                    printf("No patients registered.\n");
                } else {
                    struct Patient* temp = *patientHead;
                    printf("Registered Patients:\n");
                    while (temp != NULL) {
                        printf("Patient ID: %d, Name: %s, Age: %d, Address: %s\n", temp->id, temp->name, temp->age, temp->address);
                        if (temp->assignedRoom != NULL) {
                            printf("Assigned Room ID: %d, Type: %s\n", temp->assignedRoom->id, temp->assignedRoom->type);
                        } else {
                            printf("No room assigned.\n");
                        }
                        temp = temp->next;
                    }
                }
                break;

            case 3:
                printf("Enter Patient ID to view record: ");
                scanf("%d", &id);
                struct Patient* temp = *patientHead;
                while (temp != NULL && temp->id != id) {
                    temp = temp->next;
                }
                viewPatientRecord(temp);
                break;

            case 4:
                return;

            default:
                printf("Invalid choice! Please select a valid option.\n");
                break;
        }
    }
}


struct Doctor* createDoctor(int id, const char* name, const char* specialization) {
    struct Doctor* newDoctor = (struct Doctor*)malloc(sizeof(struct Doctor));
    newDoctor->id = id;
    strcpy(newDoctor->name, name);
    strcpy(newDoctor->specialization, specialization);
    newDoctor->next = NULL;
    return newDoctor;
}

void doctorsMenu(struct Doctor** doctorHead) {
    int choice, id;
    char name[50], specialization[50];

    while (1) {
        printf("\nDoctors Menu:\n");
        printf("1. Add a new doctor\n");
        printf("2. View all doctors\n");
        printf("3. Back to main menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Doctor ID: ");
                scanf("%d", &id);
                printf("Enter Doctor Name: ");
                scanf(" %[^\n]", name); 
                printf("Enter Specialization: ");
                scanf(" %[^\n]", specialization);

                struct Doctor* newDoctor = createDoctor(id, name, specialization);
                newDoctor->next = *doctorHead;
                *doctorHead = newDoctor;
                printf("Doctor %s has been added successfully.\n", name);
                break;

            case 2:
                displayDoctors(*doctorHead);
                break;

            case 3:
                return;

            default:
                printf("Invalid choice! Please select a valid option.\n");
                break;
        }
    }
}

int login() {
    char username[50], password[50];
    const char validUsername[] = "admin"; 
    const char validPassword[] = "admin123"; 

    printf("===== Login =====\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (strcmp(username, validUsername) == 0 && strcmp(password, validPassword) == 0) {
        printf("Login successful! Welcome, %s.\n", username);
        return 1;
    } else {
        printf("Invalid username or password. Please try again.\n");
        return 0;
    }
}

int main() {
    Queue appointmentQueue;
    initializeQueue(&appointmentQueue);

    struct Patient* patientHead = NULL;
    struct Doctor* doctorHead = NULL;
    struct Room* roomHead = NULL;
    struct PharmacyItem* pharmacyHead = NULL;

    int choice;

    while (!login()) {
    }

    while (1) {
        printf("\nMain Menu:\n");
        printf("1. Appointments\n");
        printf("2. Patients\n");
        printf("3. Doctors\n");
        printf("4. Pharmacy Menu\n");
        printf("5. Rooms Menu\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                appointmentsMenu(&appointmentQueue, doctorHead);
                break;
            case 2:
                patientsMenu(&patientHead, doctorHead, pharmacyHead, roomHead);
                break;
            case 3:
                doctorsMenu(&doctorHead);
                break;
            case 4:
                pharmacyMenu(&pharmacyHead);
                break;
            case 5:
                roomsMenu(&roomHead);
                break;
            case 6:
                printf("Exiting...\n");
                return 0;
            default:
                printf("Invalid choice! Please select a valid option.\n");
                break;
        }
    }
}
