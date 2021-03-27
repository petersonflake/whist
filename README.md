# The Game of Whist
> Play the game of whist at your terminal.

## Table of contents
* [General info](#general-info)
* [Setup](#setup)
* [Features](#features)
* [Status](#status)
* [Contact](#contact)

## General info
The game of whist, written in C.  For making moves, you type either the first
letter of the rank and suit of the card you wish to play.  So `ac` or `ca`
will both play the ace of clubs, `H2` or `2h` the two of hearts, and so on.
Spaces are ignored.  Clear the screen with control L, and see your hand and
the current trick by typing anything that is not a valid card, e.g., "ls".

## Setup
`make` and `sudo make install`, or else manually copy the resulting executable
to somewhere in your path.

## Code Examples
`whist` to play a game.  The player is North, and the trump suit and first
player are both chosen at random when the game starts.

## Features
* Play the game of whist against computer opponents on your terminal.

To-do list:
* Clean up code.  Some of it is copied from other projects I was working on so
  their is some that should be eliminated.

## Status
Project is: _in progress_, still need to clean up the code.

## Contact
<mbfmitchell@gmail.com>
