warehouse: bin/Action.o bin/Order.o bin/Warehouse.o bin/Volunteer.o bin/Customer.o bin/main.o
	g++ -g -Wall -Wextra -Iinclude -o bin/warehouse bin/Action.o bin/Order.o bin/Warehouse.o bin/Volunteer.o bin/Customer.o bin/main.o

bin/Action.o: src/Action.cpp include/Action.h
	g++ -g -Wall -Wextra -Iinclude -c -o bin/Action.o src/Action.cpp

bin/Order.o: src/Order.cpp include/Order.h
	g++ -g -Wall -Wextra -Iinclude -c -o bin/Order.o src/Order.cpp

bin/Warehouse.o: src/Warehouse.cpp include/Warehouse.h include/Action.h include/Order.h include/Volunteer.h include/Customer.h
	g++ -g -Wall -Wextra -Iinclude -c -o bin/Warehouse.o src/Warehouse.cpp

bin/Volunteer.o: src/Volunteer.cpp include/Volunteer.h
	g++ -g -Wall -Wextra -Iinclude -c -o bin/Volunteer.o src/Volunteer.cpp

bin/Customer.o: src/Customer.cpp include/Customer.h
	g++ -g -Wall -Wextra -Iinclude -c -o bin/Customer.o src/Customer.cpp

bin/main.o: src/main.cpp include/Warehouse.h
	g++ -g -Wall -Wextra -Iinclude -c -o bin/main.o src/main.cpp

clean:
	rm -f bin/*.o bin/warehouse
