# battlesnake-cpp

A battlesnake agent written in C++ for my Individual Project for CS408.

Note: this project only supports building on linux.

## Dependencies
Depends on the Catch2 framework (v2) for unit tests and the Crow framework for setting up the http server.

Crow is included with this project as a git submodule.

To install the required dependencies run the corresponding command for your distro:

### Ubuntu based distros
`apt-get install build-essential catch2`

### Arch based distros
`pacman -S base-devel catch2`

## Building and Running

### Server

#### Replit

To run on replit:
- Make an account at https://replit.com.
- Click on the '+ Create' button to create a new repl.
- Import this project from github, this may require creating a fork.
- Run the command `git submodule update --init --recursive` in the console.
- Click the run button at the top of the page.

#### Self-Host

First clone the project and cd into the created directory with the following commands:
```
git clone --recurse-submodules https://github.com/db2018strath/battlesnake-cpp.git
cd battlesnake-cpp
```

Then run one of the following:

- Debug: `make server_debug`
- Release: `make server_release`

After building the resulting binary can be found in `./out/${BUILD_TYPE}/server` where `${BUILD_TYPE}` is either `debug` or `release` corresponding to the one which has been built.

The server is hosted on port 8080 which is the default for Battlesnake.

#### Playing Games

After setting up the server, through Replit or through self-hosting, to have the server play games:
- First create an account at https://play.battlesnake.com/
- Then follow the instructions in steps 4 and 5 provided here: https://docs.battlesnake.com/guides/getting-started#step-4-create-your-battlesnake

Note that the agent is only designed to play against atleast one other player so may not perform as well when playing alone.

### Local

First clone the project and cd into the created directory with the following commands:

```
git clone --recurse-submodules https://github.com/db2018strath/battlesnake-cpp.git
cd battlesnake-cpp
```

Then run one of the following commands:

- Debug: `make ai_run_debug`
- Release: `make ai_run_release`

After building the resulting binary can be found in `./out/${BUILD_TYPE}/ai_run` where `${BUILD_TYPE}` is either `debug` or `release` corresponding to the one which has been built.

### Tests

To build the unit tests run the following command: `make tests`

To run the tests after having built them run the command: `./out/tests/tests`

### All

To build all targets run the command: `make all`
