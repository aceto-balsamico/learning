#include <stdio.h>
#include <stdlib.h>
#include "common.h"

struct set
{
  int key;
  int data;
};
struct set *array;
int capacity;	//init_arrayで定義
int size = 0;
int pre_collision = 0;
int collision = 0;

unsigned int hashFunction(int x) 
{
	//MurmurHash
	x ^= x >> 16;
    x *= 0x7feb352d;
    x ^= x >> 15;
    x *= 0x846ca68b;
    x ^= x >> 16;
	return x % capacity;
}
int checkPrime(int n)
{
  int i;
  if (n == 1 || n == 0)
  {
  return 0;
  }
  for (i = 2; i < n / 2; i++)
  {
  if (n % i == 0)
  {
    return 0;
  }
  }
  return 1;
}
int getPrime(int n)
{
  if (n % 2 == 0)
  {
  n++;
  }
  while (!checkPrime(n))
  {
  n += 2;
  }
  return n;
}
void init_array(int array_capacity)
{
	capacity = getPrime(array_capacity);
	array = (struct set *)malloc(capacity * sizeof(struct set));
	for (int i = 0; i < capacity; i++)
	{
	array[i].key = 0;
	array[i].data = 0;
	}
}
void insert(int key, int data)
{
    int index = hashFunction(key);
    if (array[index].data == 0)
    {
        array[index].key = key;
        array[index].data = data;
        size++;
        // printf("\n Key (%d) has been inserted \n", key);
    }
      else if (array[index].key == key)
    {
      array[index].data = data;
    }
    else
    {
		//Open Address処理、10回で見つからなかったらCollision判定
		pre_collision++;
		for(int search_count = 1; search_count <= 10; search_count++)
		{
			if(array[(index+1) % capacity].data == 0)
			{
				array[(index+1) % capacity].key = key;
				array[(index+1) % capacity].data = data;
				size++;
				// printf("\nkey:%4d, search count:%2d", key, search_count);
				break;
			}
			index = ((index+1) % capacity);
			if(search_count == 10)
			{
				printf("\nCollision occured");
				collision++;
			}
		}
    }
}
void remove_element(int key)
{
  int index = hashFunction(key);
  if (array[index].data == 0)
  {
  printf("\n This key does not exist \n");
  }
  else
  {
  array[index].key = 0;
  array[index].data = 0;
  size--;
  printf("\n Key (%d) has been removed \n", key);
  }
}
void display()
{
  int i;
  for (i = 0; i < capacity; i++)
  {
  if (array[i].data == 0)
  {
    // printf("\n array[%d]: / ", i);
  }
  else
  {
    printf("\n key: %4d array[%3d]: %d \t", array[i].key, i, array[i].data);
  }
  }
}
int size_of_hashtable()
{
  return size;
}
int select_menu()
{
  int choice, key, data, n;
  int c = 0;
  init_array(10);

  do
  {
  printf("1.Insert item in the Hash Table"
     "\n2.Remove item from the Hash Table"
     "\n3.Check the size of Hash Table"
     "\n4.Display a Hash Table"
     "\n\n Please enter your choice: ");

  scanf("%d", &choice);
  switch (choice)
  {
  case 1:

    printf("Enter key -:\t");
    scanf("%d", &key);
    printf("Enter data -:\t");
    scanf("%d", &data);
    insert(key, data);

    break;

  case 2:

    printf("Enter the key to delete-:");
    scanf("%d", &key);
    remove_element(key);

    break;

  case 3:

    n = size_of_hashtable();
    printf("Size of Hash Table is-:%d\n", n);

    break;

  case 4:

    display();

    break;

  default:

    printf("Invalid Input\n");
  }

  printf("\nDo you want to continue (press 1 for yes): ");
  scanf("%d", &c);

  } while (c == 1);
  return 0;
}

int main()
{
	begin_time;

	init_array(100000);
	for(int num = 0; num < 10000; num++)
	{
		insert(num, num);
	}

	end_time;

	// display();
	printf("\nsize:%4d, empty:%4d, capacity:%4d\n", size, capacity-size, capacity);
	printf("Pre collision:%2d\n", pre_collision);
	printf("Collision:%2d\n", collision);

	free(array);

	return 0;
}