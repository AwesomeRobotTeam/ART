#!/bin/bash

#Nvidia xorg hold
sudo apt-mark hold xserver-xorg-core

#hold package
sudo apt-add-repository universe
sudo apt-get update

#Package install
sudo apt-get install vim build-essential linux-tools-common linux-tools-generic cmake

#Command Line
sudo apt-get install bash-completion command-not-found
sudo apt-file update &>/dev/null
sudo update-command-not-found &>/dev/null

#GUI
gsettings set com.canonical.Unity.Lenses disabled-scopes "['more_suggestions-amazon.scope', \
    'more_suggestions-u1ms.scope', 'more_suggestions-populartracks.scope', 'music-musicstore.scope', \
    'more_suggestions-ebay.scope', 'more_suggestions-ubuntushop.scope', 'more_suggestions-skimlinks.scope']"


