#include "pch.h"
#include "CppUnitTest.h"
#include "../Semaphores/monitor.h"
#include "../Semaphores/process.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestsemaphores
{
	TEST_CLASS(UnitTestsemaphores)
	{
	public:
		
		TEST_METHOD(Bufor_test)
		{
			Buffer FIFO;
			FIFO.put(3);
			FIFO.put(5);
			FIFO.put(7);
			FIFO.put(45);
			
			Assert::AreEqual(FIFO.countEN(), 0);
			Assert::AreEqual(FIFO.countON(), 4);
			Assert::AreEqual(FIFO.count(), 4);

			Assert::AreEqual(FIFO.get(), 3);
			Assert::AreEqual(FIFO.get(), 5);

			Assert::AreEqual(FIFO.peek(), 7);
			Assert::AreEqual(FIFO.peek(), 7);
		}

	};
}
