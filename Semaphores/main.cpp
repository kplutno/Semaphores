#include <iostream>
#include "monitor.h"
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include "process.h" //definicja klasy bufor
#include <thread>

using namespace std;

int numOfProdEvenWaiting = 0;
int numOfProdOddWaiting = 0;
int numOfConsEvenWaiting = 0;
int numOfConsOddWaiting = 0;

int numOdd = 1;
int numEven = 0;

const int CONST_TIME = 100;

Semaphore Mutex(1);     //globalny mutex realizujacy strefe krytyczna
Semaphore pe_Mutex(0);  //ProdEven
Semaphore po_Mutex(0);  //ProdOdd
Semaphore ce_Mutex(0);  //ConsEven
Semaphore co_Mutex(0);  //ConsOdd

Buffer FIFO;      //tworzenie bufora FIFO

void prodEven();  //A1
void prodOdd();   //A2
void consEven();  //B1
void consOdd();   //B2

bool canProdEven();
bool canProdOdd();
bool canConsEven();
bool canConsOdd();

int genEvenNumber();
int genOddNumber();

int main()
{
	thread pe(prodEven);
	thread po(prodOdd);
	thread ce(consEven);
	thread co(consOdd);
	thread pe1(prodEven);
	thread po1(prodOdd);
	thread ce1(consEven);
	thread co1(consOdd);

	pe.join();
	po.join();
	ce.join();
	co.join();
	pe1.join();
	po1.join();
	ce1.join();
	co1.join();

}

/*


Funkcje opisujace procesy.


*/
void prodEven()
{
	srand(time(NULL));

	while (1)
	{
		Mutex.p(); // zajmowanie sekcji krytycznej

		if (!canProdEven())
		{
			++numOfProdEvenWaiting; //inkrementacja w kolejce
			Mutex.v();              //usypianie i opuszczanie sekcji krytycznej
			pe_Mutex.p();           //zajmowanie mutexu odppowiedniego dla danego procesu
			--numOfProdEvenWaiting; //usuwanie procesu z kolejki
		}
		
		int prod = genEvenNumber();
		FIFO.put(prod);
		system("cls");
		cout << FIFO;

		if (numOfProdOddWaiting > 0 && canProdOdd())
		{
			po_Mutex.v();
		}
		else if (numOfConsEvenWaiting > 0 && canConsEven())
		{
			ce_Mutex.v();
		}
		else if (numOfConsOddWaiting > 0 && canConsOdd())
		{
			co_Mutex.v();
		}
		else
		{
			Mutex.v();
		}
		Sleep(CONST_TIME + (rand() % 500));

	}//while
}

void prodOdd()
{
	srand(time(NULL));

	while (1)
	{
		Mutex.p(); // zajmowanie sekcji krytycznej

		if (!canProdOdd())
		{
			++numOfProdOddWaiting; //inkrementacja w kolejce
			Mutex.v();              //usypianie i opuszczanie sekcji krytycznej
			po_Mutex.p();           //zajmowanie mutexu odppowiedniego dla danego procesu
			--numOfProdOddWaiting; //usuwanie procesu z kolejki
		}

		int prod = genOddNumber();
		FIFO.put(prod);
		system("cls");
		cout << FIFO;

		if (numOfProdEvenWaiting > 0 && canProdEven())
		{
			pe_Mutex.v();
		}
		else if (numOfConsOddWaiting > 0 && canConsOdd())
		{
			co_Mutex.v();
		}
		else if (numOfConsEvenWaiting > 0 && canConsEven())
		{
			ce_Mutex.v();
		}
		else
		{
			Mutex.v();
		}
		Sleep(CONST_TIME + (rand() % 500));

	}//while
}

void consEven()
{
	srand(time(NULL));

	while (1)
	{
		Mutex.p(); //zajmowanie sekcji krytycznej

		if (!canConsEven())
		{
			++numOfConsEvenWaiting; //inkrementacja w kolejce
			Mutex.v();              //usypianie i opuszczanie sekcji krytycznej
			ce_Mutex.p();           //zajmowanie mutexu odppowiedniego dla danego procesu
			--numOfConsEvenWaiting; //usuwanie procesu z kolejki
		}

		FIFO.get();
		system("cls");
		cout << FIFO;

		if (numOfConsOddWaiting > 0 && canConsOdd())
		{
			co_Mutex.v();
		}
		else if (numOfProdEvenWaiting > 0 && canProdEven())
		{
			pe_Mutex.v();
		}
		else if (numOfProdOddWaiting > 0 && canProdOdd())
		{
			po_Mutex.v();
		}
		else
		{
			Mutex.v();
		}
		Sleep(CONST_TIME + (rand() % 500));

	}//while
}

void consOdd()
{
	srand(time(NULL));

	while (1)
	{
		Mutex.p(); //zajmowanie sekcji krytycznej

		if (!canConsOdd())
		{
			++numOfConsOddWaiting; //inkrementacja w kolejce
			Mutex.v();              //usypianie i opuszczanie sekcji krytycznej
			co_Mutex.p();           //zajmowanie mutexu odppowiedniego dla danego procesu
			--numOfConsOddWaiting; //usuwanie procesu z kolejki
		}

		FIFO.get();
		system("cls");
		cout << FIFO;

		if (numOfConsEvenWaiting > 0 && canConsEven())
		{
			ce_Mutex.v();
		}
		else if (numOfProdOddWaiting > 0 && canProdOdd())
		{
			po_Mutex.v();
		}
		else if (numOfProdEvenWaiting > 0 && canProdEven())
		{
			pe_Mutex.v();
		}
		else
		{
			Mutex.v();
		}
		Sleep(CONST_TIME + (rand() % 500));

	}//while
}
/*


Funkcje bool'owskie sprawdzajace czy fifo spelnia wymagania procesów.


*/
bool canProdEven()
{
	if (FIFO.countEN() < 10)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool canProdOdd()
{
	if (FIFO.countON() < FIFO.countEN())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool canConsEven()
{
	if (FIFO.count() > 3 && (FIFO.peek() % 2 == 0))
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool canConsOdd()
{
	if (FIFO.count() > 7 && (FIFO.peek() % 2 == 1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*


Generacja liczb parzystych i nieparzystych


*/
int genEvenNumber()
{
	int buf = numEven;
	numEven = (numEven + 2) % 50;
	return buf;
}
int genOddNumber()
{
	int buf = numOdd;
	numOdd = (numOdd + 2) % 50;
	return buf;
}