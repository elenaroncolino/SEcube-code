<img src="https://www.secube.blu5group.com/site/templates/dist/img/logo.png" style="color:#ffffff" width="400px" />

# SEcube™ Software Development Kit (version 1.5.2)
Copyright (C) 2021 Blu5 Labs Ltd.

## Licence
All SEcube releases published on this website are Open Source - GPL 3.0 and are developed by the Academia Community.

## Terms of use
By downloading the software from this page, you agree to the specified terms.

The software is provided to you "as is" and we make no express or implied warranties whatsoever with respect to its functionality, operability, or use, including, without limitation, any implied warranties of merchantability, fitness for a particular purpose, or infringement. We expressly disclaim any liability whatsoever for any direct, indirect, consequential, incidental or special damages, including, without limitation, loss revenues, lost profits, losses resulting from business interruption or loss of data, regardless of the form of action or legal thereunder which the liability may be asserted, even if advised of the possibility likelihood of such damages.

# PUF 
This is a project made for the course of Cybersecurity for embedded systems of the Masters on embedded system in Politecnico di Torino.

The purpose of this project is to extract the RAM PUFs from the memory of the SEcube storing them on a DB, in this case a simple txt file, and perform also a challenge on the board.

## Instructions to run the project

### Import the project
Instructions on how to import the project are the same as the ones for the original project provided in the wiki

### Run the project
the steps to run the project are the following

1. Erase the flash memory of the SEcube
![erase_flash]
2. Flash the SEcube
![chip_flash]
3. Run on the host the "puf_db_init.cpp"
![puf_db_init]
4. Run on the host the "puf_challenge.cpp"
![puf_challenge]












[erase_flash]: images/erase_flash.png
[chip_flash]: images/chip_flash.png
[puf_db_init]: images/puf_db_init.png
[puf_challenge]: images/puf_challenge.png