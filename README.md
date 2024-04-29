# Warehouse Management System

## ABOUT:
Welcome to the most advanced warehouse management system on the internet!
This C++ project includes an advanced process of warehouse that takes orders and delivers it to the clients. 
In here you can be your preferred kind of volunteer such as delivering guy or a processing guy, or you just can order a package for your loved ones!
It has features such as:
1. Volunteers - the warehouse workers(but they work for free!). there are 2 kinds of volunteers:
     a. collectors: there job is to get orders from client and handle it.
     b. drivers: there job is to get the order to the client who ordered it.
2. Customers - the warehosue clients. They purchases orders and gets them to the front door of their houses. there are 2 kinds of customers:
     a. civilian
     b. soldier
   There is an option to add costumer while we run the warehouse.
4. Actions - handling orders in the warehouse.
5. backup and restore - there is a backup option the the warehouse, so we can restore it any time we want!

## The project flow:
A customer puts an order to the warehouse. A free collector volunteer gets the order and processes it. After finished, the collector sends the order to a free driver that ables to get to this specific customer(each driver has a driving distance limit). The driver brings the order to the customer and we all happy!


## HOW TO RUN:
This is a C++ project supports "rule-of-5" concept.
1. in Terminal: make
2. in Terminal: bin/warehouse "****.txt" (this is the config File the program gets as an input)

   note about the config file: this is the starting file that gets as an input the starting volunteers and costumers.
