#!/bin/bash

gcc ./*.c -lpthread -lcurl -o redirect_server
chmod 755 ./redirect_server
