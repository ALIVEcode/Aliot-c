/**
 * @file Vector.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2022-03-31
 *
 * @copyright Copyright (c) 2022
 *
 */

/** List template that can be used to store any type of data.
 *
 * @tparam T The type of data that will be stored in the vector.
 */
template <typename T>
class Vector
{
    // The vector's size.
    int size;
    // The vector's capacity.
    int capacity;
    // The vector's data.
    T *data;
    // The vector's current index.
    int index;

public:
    /** Constructs a vector with a given capacity.
     *
     * @param capacity The vector's capacity.
     */
    Vector(int capacity)
    {
        this->size = 0;
        this->capacity = capacity;
        this->data = new T[capacity];
        this->index = 0;
    }

    // add an element to the vector
    void add(T element)
    {
        if (this->size == this->capacity)
        {
            this->capacity *= 2;                 // double the capacity
            T *newData = new T[this->capacity];  // allocate new memory
            for (int i = 0; i < this->size; i++) // copy old data to new data
                newData[i] = this->data[i];
            delete[] this->data;  // free old data
            this->data = newData; // assign new data
        }
        this->data[this->size] = element; // add element to vector
        this->size++;                     // increase size
    }

    // remove an element from the vector
    void remove(T element)
    {
        for (int i = 0; i < this->size; i++)
        {
            if (this->data[i] == element) //
            {
                this->removeAt(i); // remove element at index i
                break;
            }
        }
    }

    // remove an element from the vector at a given index
    void removeAt(int index)
    {
        if (index < 0 || index >= this->size) // check if index is valid
            return;

        for (int i = index; i < this->size - 1; i++) // shift elements to the left
            this->data[i] = this->data[i + 1];

        this->size--; // decrease size
    }

    // get the element at a given index
    T get(int index)
    {
        if (index < 0 || index >= this->size) // check if index is valid
            return NULL;

        return this->data[index]; // return element at index
    }

    // get the size of the vector
    int getSize()
    {
        return this->size;
    }

    // get the capacity of the vector
    int getCapacity()
    {
        return this->capacity;
    }

    // remove all elements from the vector
    void clear()
    {
        this->size = 0; // reset size
    }

    // get the index of the first occurence of an element
    int indexOf(T element)
    {
        for (int i = 0; i < this->size; i++)
        {
            if (this->data[i] == element) //  if element is found
                return i;                 // return index
        }
        return -1; // element not found
    }
};
