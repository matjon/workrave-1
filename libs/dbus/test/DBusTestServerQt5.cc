// DBusTestServerQt5.cc
//
// Copyright (C) 2013 Rob Caelers & Raymond Penners
// All rights reserved.
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <QtCore>
#include <QtDBus>

#include "debug.hh"

#include "DBusTestServerQt5.hh"

#include "dbus/IDBus.hh"

#include "DBusQt5.hh"

#define  WORKRAVE_TEST_PATH "/org/workrave/Workrave/Test"
#define  WORKRAVE_TEST_INTERFACE "org.workrave.TestInterface"
#define  WORKRAVE_TEST_SERVICE "org.workrave.Test"

using namespace std;

//! Constructor.
DBusTestServerQt5::DBusTestServerQt5() 
{
}


//! Destructor.
DBusTestServerQt5::~DBusTestServerQt5()
{
}

int main(int argc, char **argv)
{
  DBusTestServerQt5 s;
  s.run(argc, argv);
}

void
DBusTestServerQt5::run(int argc, char **argv)
{
#ifdef TRACING
  Debug::name(std::string("server"));
#endif

  try
    {
      app = new QCoreApplication(argc, argv);

      qDBusRegisterMetaType<DBusTestData::StructWithAllBasicTypes>();
      qDBusRegisterMetaType<DBusTestData::Data>();
      qDBusRegisterMetaType<QList<DBusTestData::Data>>();
      qDBusRegisterMetaType<QMap<QString, DBusTestData::Data>>();

      workrave::dbus::IDBus::Ptr dbus = workrave::dbus::DBusQt5::create();
     
      dbus->init();
        
      extern void init_DBusTestQt(workrave::dbus::IDBus::Ptr dbus);
      init_DBusTestQt(dbus);

      dbus->connect(WORKRAVE_TEST_PATH, WORKRAVE_TEST_INTERFACE, this);
      dbus->register_object_path(WORKRAVE_TEST_PATH);
      dbus->register_service(WORKRAVE_TEST_SERVICE);

      app->exec();        
    }
  catch (workrave::dbus::DBusException &)
    {
      std::cerr << "server failed" <<std::endl;
    }
}