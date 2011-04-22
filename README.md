# social sqncr

## Description
social sqncr is an interactive audio-visual installation that aims to make people more aware of the act of creating their public identity.
To create their ‘identity’, participants use their bodies to physically interact with a pseudo-musical instrument projected in a physical space to create virtual musical creatures based on their movements. The instrument consists of eight zones that participants interact with which influence the shape, movement, and aural properties of the resulting entity.

Once complete, the system captures an image of the participant to attach to the creature which is then set free into an eco-system inhabited by similar entities. The entities react to one another, much like human beings in a social network. Participants can cause environmental disturbances by physically interacting with the projection.

The physical movements required of participants ensures a high level of investment in the process, causing a heightened awareness of their creation, and thus themselves. Watching their entity interact with other entities is a reflection of their own interaction in their social networks.

## Technical Detail
The visuals for this project were coded using [openFrameworks](http://www.openframeworks.cc/), “an open source C++ toolkit for creative coding,” and [openGL](http://www.opengl.org/) (Open Graphics Library). Some of the code for flocking, particle interaction, and [openCV](http://opencv.willowgarage.com/wiki/) (Open Source Computer Vision) tracking is based code from [Zachary Lieberman](http://thesystemis.com/).

The audio is coded in [SuperCollider](http://supercollider.sourceforge.net/), “a real time audio synthesis programming language”. Some of the code for effects, scales and synth definitions are based on examples on the site and the community.

Two Macintosh Intel G5 towers were used power the installation. The two computers were hardwired to a closed network with static IP addresses for communication and debugging remotely. One computer ran the SuperCollider application and the other ran the openFrameworks application. OSC (Open Sound Control) is used to communicate between the two applications.

## Other Stuff

1. social sqncr consists of 2 applications that are connected via Open Sound Control.
2. The source code for openFrameworks application is inside the "openframeworks" folder. Use xcode to build the app per usual. It was built in version 0.06 and will probably not compile on other versions.
3. The source code for the SuperCollider application is inside the "supercollider" folder. Dump the "socialsqncrv0.8" folder inside "SCClassLibrary" in SuperCollider. Load up main.sc to run the application.