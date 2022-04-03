#include <iostream>
#include "Person.h"
#include "Dog.h"
#include "SerializedObject.h"
#include <fstream>
#include <list>
using namespace std;

int main()
{
    Dog* d1 = Dog::NewDog();
    Dog* d2 = Dog::NewDog();
    Dog* d3 = Dog::NewDog();
    Person* p1 = Person::NewPerson();
    Person* p2 = Person::NewPerson();
    Person* p3 = Person::NewPerson();

    //Serializing each new object
    SerializedObject* dog1 = d1->Serialize();
    SerializedObject* dog2 = d2->Serialize();
    SerializedObject* dog3 = d3->Serialize();
    SerializedObject* person1 = p1->Serialize();
    SerializedObject* person2 = p2->Serialize();
    SerializedObject* person3 = p3->Serialize();

    //Creating stream to send serialized objects into file. 
    fstream InNOut;
    InNOut.open("FindMe.txt", ios::in | ios::binary);
    if (InNOut.fail())
    {
        cout << "Cannot open file!" << endl;
        return 1;
    }

    //Putting each searialized object into file with its data
    for (int i = 0; i < person1->mBufferSize; i++)
        InNOut.put(person1->mBuffer[i]);

    for (int i = 0; i < person2->mBufferSize; i++)
        InNOut.put(person2->mBuffer[i]);

    for (int i = 0; i < person3->mBufferSize; i++)
        InNOut.put(person3->mBuffer[i]);

    for (int i = 0; i < dog1->mBufferSize; i++)
        InNOut.put(dog1->mBuffer[i]);

    for (int i = 0; i < dog2->mBufferSize; i++)
        InNOut.put(dog2->mBuffer[i]);

    for (int i = 0; i < dog3->mBufferSize; i++)
        InNOut.put(dog3->mBuffer[i]);

    InNOut.close();

    //Modify the file on disk
    // Correctly find the 2nd dog and change its age to 999
    int tAge = 999;
    int search = (person1->mFullSize * 3) + dog1->mBufferSize + (sizeof(int) * 3); //Creating veriable of location of dog2

    InNOut.open("FindMe.txt", ios::in | ios::binary | ios::out);
   //char* tArr = new char[dog1->mBufferSize]; //Creating array to hold new age info
    /*tArr[0] = (tAge >> 24) & 0xFF;
    tArr[1] = (tAge >> 16) & 0xFF;
    tArr[2] = (tAge >> 8) & 0xFF;
    tArr[3] = tAge & 0xFF;*/

    InNOut.seekp(search + 12, ios::beg);
    //for (int i = 0; i < 4; ++i)
    InNOut.put(tAge >> 24);
    InNOut.put(tAge >> 16);
    InNOut.put(tAge >> 8);

    InNOut.close(); //Closing App
    //delete[] tArr;
    // Delete all existing objects through the factories just in case
    d1->DeleteAll();
    p1->DeleteAll();
    dog1 = nullptr;
    dog2 = nullptr;
    dog3 = nullptr;
    person1 = nullptr;
    person2 = nullptr;
    person3 = nullptr;
   
    // Read the file to see the next type
    // Create a SO, fill it out, and send it to right factory
    SerializedObject* tSo = new SerializedObject; //Creating new searlized object to hold dog info 
    char* NewmBuffer = new char[sizeof(int)]; //Creating new buffer
    InNOut.open("FindMe.txt", ios:: in | ios:: out |ios::binary);
    InNOut.seekg(search + 12, ios::beg ); //Finding location of dog2 in file
    InNOut.read(NewmBuffer, sizeof(int)); //Pulling info from file 
    tSo->mBuffer = NewmBuffer;
    Dog* AdjustedDog = AdjustedDog->CreateFromSO(tSo); //Creating new dog
    cout << AdjustedDog->mAge << '\n';

    delete[] NewmBuffer;
    delete tSo;
    delete AdjustedDog;
}
