coin: 
	g++ coinminer.cpp -o coinminer -Ofast -lcrypto -lssl -Wall -Werror

packages: 
	sudo apt install libssl-dev