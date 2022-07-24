/**
  ******************************************************************************
  * File Name          : puf_db_init.cpp
  * Description        : basic example of puf retreival from the board and storing in a DB.
  ******************************************************************************
  *
  * Copyright � 2016-present Blu5 Group <https://www.blu5group.com>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Lesser General Public
  * License as published by the Free Software Foundation; either
  * version 3 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  * Lesser General Public License for more details.
  *
  * You should have received a copy of the GNU Lesser General Public
  * License along with this library; if not, see <https://www.gnu.org/licenses/>.
  *
  ******************************************************************************
  */

/*! \file puf_db_init.cpp
 *  \brief This file is a simple example of how to access the board and retreive a number of pufs and storing them in a DB.
*	In this case the DB is a simple txt file.
 *  \date 24/07/2022
 */

#include "../sources/L1/L1.h"
#include <memory>
#include <thread>
#include <iostream>
#include <string.h>

using namespace std;

#define PUFNUM 10

static int writePUFS(uint32_t* pufs, int puf_num);

// RENAME THIS TO main()
int puf_db_init() {
	/* we recommend using smart pointers to manage L0 and L1 objects in order
	   to ensure proper memory management by their constructors and destructors. */
	unique_ptr<L0> l0 = make_unique<L0>();
	unique_ptr<L1> l1 = make_unique<L1>();

	array<uint8_t, L1Parameters::Size::PIN> pin_admin = {'t','e','s','t'}; // PIN to login as administrator on the SEcube (assuming the device was initialized with this PIN)

	cout << "Welcome to SEcube Hello World!" << endl;
	this_thread::sleep_for(chrono::milliseconds(1000)); // these delays are simply here to slow down the process making it more comprehensible
	cout << "Looking for SEcube devices...\n" << endl;
	this_thread::sleep_for(chrono::milliseconds(2000));

	vector<pair<string, string>> devices;
	int ret = l0->GetDeviceList(devices); // this API fills the vector with pairs including details about the devices (path and serial number)
	if(ret){
		cout << "Error while searching for SEcube devices! Quit." << endl;
		return -1;
	}

	int numdevices = l0->GetNumberDevices(); // this API checks how many SEcube devices are connected to the PC
	if(numdevices == 0){
		cout << "No SEcube devices found! Quit." << endl;
		return 0;
	}
	cout << "Number of SEcube devices found: " << numdevices << endl;
	cout << "List of SEcube devices (path, serial number):" << endl;
	int index = 0;
	for(pair<string, string> p : devices){
		cout << index << ") " << p.first << " - " << p.second << endl;
		index++;
	}

	int sel = 0;
//	cout << "\nEnter the number corresponding to the SEcube device you want to use..." << endl;
//	/* warning: if cin does not wait for input in debug mode with eclipse, open the launch configuration and select
//	 * the "use external console for inferior" checkbox under the debugger tab (see https://stackoverflow.com/questions/44283534/c-debug-mode-in-eclipse-causes-program-to-not-wait-for-cin)*/
//	if(!(cin >> sel)){
//		cout << "Input error...quit." << endl;
//		return -1;
//	}

	if((sel >= 0) && (sel < numdevices)){
		std::array<uint8_t, L0Communication::Size::SERIAL> sn = {0};
		if(devices.at(sel).second.length() > L0Communication::Size::SERIAL){
			cout << "Unexpected error...quit." << endl;
			return -1;
		} else {
			memcpy(sn.data(), devices.at(sel).second.data(), devices.at(sel).second.length());
		}
		l1->L1SelectSEcube(sn);
		cout << "\nDevice " << devices.at(sel).first << " - " << devices.at(sel).second << " selected." << endl;
		cout << "Attempting login..." << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
		try{
			l1->L1Login(pin_admin, SE3_ACCESS_ADMIN, true); // provide pin, access privilege and boolean to force logout before login (in case there was already an active login on the device)
			/*
			 * notice that when the login is executed, the SEcube device enters a specific loop to handle requests coming from the host.
			 * if you close your program without performing the logout, the SEcube device will still be in that loop the next time you
			 * will attempt the login (provided that you do not unplug the SEcube from the PC, removing power). this is the reason why
			 * you have to provide the boolean as third parameter of the L1Login function, because you need to force the logout from
			 * previous pending sessions. if you do not force the logout, you won't be able to communicate with the SEcube (because the
			 * session is still open on the SEcube side, but it doesn't exist anymore on your host).
			 */
		} catch (...) { // catch any kind of exception (login will throw if the password is wrong or if any error happens)
			cout << "Login error. Quit." << endl;
			return 0;
		}
		if(!l1->L1GetSessionLoggedIn()){ // check if login was ok
			cout << "Login error. Quit." << endl;
			return 0;
		}
		cout << "\nHello World! You are now logged in. You can use the full potential of the SEcube." << endl;
		this_thread::sleep_for(chrono::milliseconds(1000));
		cout << "\nLogging out..." << endl;
		try{
			l1->L1Logout();
		} catch (...) {
			cout << "Logout error. Quit." << endl;
			return 0;
		}
		if(l1->L1GetSessionLoggedIn()){ // check if logout was ok
			cout << "Logout error. Quit." << endl;
			return 0;
		}
		cout << "You are now logged out." << endl;
		cout << "\nBasic SEcube test completed successfully." << endl;
		//cout << "\nBasic SEcube test completed successfully.\nPress q to quit." << endl;
		//while(cin.get() != 'q'){};
	} else {
		cout << "You entered an invalid number. Quit." << endl;
		return 0;
	}





	/*Code added for the PUF management*/

	printf("\n\n\n\n Print reset \n---------\n");
	uint32_t PUF_list[PUFNUM];
	for(int i=0; i<PUFNUM; i++){
		PUF_list[i] = 0;
		//printf("%d->%d \n",i,PUF_list[i]);
	}



	l1->L1GetPUFS((uint32_t*)PUF_list);  // request all PUFS one by one. Maybe it could be used also for the challenge

	printf("\n\n\n\n Print PUFs \n---------\n");
	for(int i=0; i<PUFNUM; i++)
		printf("%d->%X \n",i,PUF_list[i]);
	writePUFS(PUF_list, PUFNUM);


	return 0;
}




















static int writePUFS(uint32_t* pufs, int puf_num){

	char str_puf[32];

	FILE *fp = fopen("PUFS.txt", "w");
	if(fp == NULL){
		printf("Can't open PUF file, PUFS were not stored\n");
		return 1;
	}
	else{
		itoa((int)pufs[0],str_puf,10);
		strcat(str_puf, "\n");
		fprintf(fp,str_puf);			//write to file

		fclose(fp);
		FILE *fp = fopen("PUFS.txt", "a");

		for(int i=1; i<puf_num; i++){
			itoa((int)pufs[i],str_puf,10);
			strcat(str_puf, "\n");
			fprintf(fp,str_puf);		//write to file
		}
	}


	fclose(fp);


	return 0;
}
