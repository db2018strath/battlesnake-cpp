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

First clone the project into its own directory and cd into it:

```
git clone --recurse-submodules https://github.com/db2018strath/battlesnake-cpp.git
cd battlesnake-cpp
```

### Server

#### Replit

To run on replit first import the project from github, this may require creating a fork of the project.

Then run the command `git submodule update --init --recursive` in the console.

And then click the run button at the top of the screen.

#### Self-Host

To build the http server run one of the following:

- Debug: `make server_debug`
- Release: `make server_release`

After building the resulting binary can be found in `./out/${BUILD_TYPE}/server` where `${BUILD_TYPE}` is either `debug` or `release` corresponding to the one which has been built.

The server is hosted on port 8080 which is the default for battlesnake.

### Local

To build the local simulator run one of the following:

- Debug: `make ai_run_debug`
- Release: `make ai_run_release`

After building the resulting binary can be found in `./out/${BUILD_TYPE}/ai_run` where `${BUILD_TYPE}` is either `debug` or `release` corresponding to the one which has been built.

### Tests

To build the unit tests run the following command: `make tests`

To run the tests after having built them run the command: `./out/tests/tests`

### All

To build all targets run the command: `make all`
