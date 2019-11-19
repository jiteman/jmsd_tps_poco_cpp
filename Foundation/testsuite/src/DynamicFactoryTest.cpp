//
// DynamicFactoryTest.cpp
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "DynamicFactoryTest.h"
#include "Poco/CppUnit/TestCaller.h"
#include "Poco/CppUnit/TestSuite.h"
#include "Poco/DynamicFactory.h"
#include "Poco/Exception.h"
#include <memory>


using Poco::DynamicFactory;
using Poco::Instantiator;


namespace
{
	class Base
	{
	public:
		Base()
		{
		}

		virtual ~Base()
		{
		}
	};

	class A: public Base
	{
	};

	class B: public Base
	{
	};
}


DynamicFactoryTest::DynamicFactoryTest(const std::string& name): CppUnit::TestCase(name)
{
}


DynamicFactoryTest::~DynamicFactoryTest()
{
}


void DynamicFactoryTest::testDynamicFactory()
{
	DynamicFactory<Base> dynFactory;

	dynFactory.registerClass<A>("A");
	dynFactory.registerClass<B>("B");

	assert (dynFactory.isClass("A"));
	assert (dynFactory.isClass("B"));

	assert (!dynFactory.isClass("C"));

#ifndef POCO_ENABLE_CPP11
	std::auto_ptr<A> a(dynamic_cast<A*>(dynFactory.createInstance("A")));
	std::auto_ptr<B> b(dynamic_cast<B*>(dynFactory.createInstance("B")));
#else
	std::unique_ptr<A> a(dynamic_cast<A*>(dynFactory.createInstance("A")));
	std::unique_ptr<B> b(dynamic_cast<B*>(dynFactory.createInstance("B")));
#endif // POCO_ENABLE_CPP11

	assertFalse( a.get() == nullptr );
	assertFalse( b.get() == nullptr );

	try
	{
		dynFactory.registerClass<A>("A");
		failmsg("already registered - must throw");
	}
	catch (Poco::ExistsException&)
	{
	}

	dynFactory.unregisterClass("B");
	assert (dynFactory.isClass("A"));
	assert (!dynFactory.isClass("B"));

	try
	{
#ifndef POCO_ENABLE_CPP11
		std::auto_ptr<B> b(dynamic_cast<B*>(dynFactory.createInstance("B")));
#else
		std::unique_ptr<B> b(dynamic_cast<B*>(dynFactory.createInstance("B")));
#endif // POCO_ENABLE_CPP11
		failmsg("unregistered - must throw");
	}
	catch (Poco::NotFoundException&)
	{
	}
}


void DynamicFactoryTest::setUp()
{
}


void DynamicFactoryTest::tearDown()
{
}


CppUnit::Test* DynamicFactoryTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DynamicFactoryTest");

	CppUnit_addTest(pSuite, DynamicFactoryTest, testDynamicFactory);

	return pSuite;
}
